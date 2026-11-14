#include "konsola.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>

using namespace std;

namespace {
string nazwaStatusu(StatusZamowienia status) {
    switch (status) {
        case StatusZamowienia::NOWE:
            return "Nowe";
        case StatusZamowienia::W_REALIZACJI:
            return "W realizacji";
        case StatusZamowienia::WYSLANE:
            return "Wyslane";
        case StatusZamowienia::DOSTARCZONE:
            return "Dostarczone";
    }
    return "Nieznany";
}
}  // namespace

Konsola::Konsola(SystemSklepu& system) : system_(system) {}

string Konsola::wczytajTekst(const string& pytanie) {
    cout << pytanie;
    string tekst;
    if (!getline(cin, tekst)) {
        throw runtime_error("Koniec wejscia.");
    }
    auto poczatek = tekst.find_first_not_of(" \t\r\n");
    if (poczatek == string::npos) {
        return "";
    }
    auto koniec = tekst.find_last_not_of(" \t\r\n");
    return tekst.substr(poczatek, koniec - poczatek + 1);
}
int64_t Konsola::wczytajLiczbe(const string& pytanie) {
    istringstream wejscie(wczytajTekst(pytanie));
    int64_t liczba = 0;
    char nadmiar = '\0';
    if (!(wejscie >> liczba) || (wejscie >> nadmiar)) {
        throw invalid_argument("Wpisz liczbe calkowita.");
    }
    return liczba;
}
int Konsola::wczytajInt(const string& pytanie) {
    auto liczba = wczytajLiczbe(pytanie);
    if (liczba < numeric_limits<int>::min() ||
        liczba > numeric_limits<int>::max()) {
        throw invalid_argument("Liczba poza zakresem typu int.");
    }
    return static_cast<int>(liczba);
}
void Konsola::pokazKwote(int64_t grosze) const {
    cout << grosze / 100 << ',' << setfill('0') << setw(2) << grosze % 100
         << setfill(' ') << " zl";
}
void Konsola::pokazNaglowek(const string& tytul) const {
    cout << '\n'
         << string(78, '-') << '\n'
         << tytul << '\n'
         << string(78, '-') << '\n';
}
void Konsola::pokazProdukty(const vector<Produkt>& produkty) const {
    pokazNaglowek("PRODUKTY");
    if (produkty.empty()) {
        cout << "Brak produktow.\n";
        return;
    }
    cout << " ID | Nazwa                  | Kategoria    | Cena (zl) | "
            "Stan | Oferta\n";
    for (const auto& produkt : produkty) {
        ostringstream cena;
        auto grosze = produkt.pobierzCeneGrosze();
        cena << grosze / 100 << ',' << setfill('0') << setw(2) << grosze % 100;
        cout << right << setw(3) << produkt.pobierzId() << " | " << left
             << setw(22) << produkt.pobierzNazwe() << " | " << setw(12)
             << produkt.pobierzKategorie() << " | " << right << setw(9)
             << cena.str() << " | " << setw(4)
             << produkt.pobierzStanMagazynowy() << " | "
             << (produkt.czyAktywny() ? "Aktywny" : "WYCOFANY") << '\n';
    }
}
string Konsola::wybierzKategorie() {
    vector<string> kategorie;
    for (const auto& produkt : system_.wyszukajProdukty("", "")) {
        const auto& kategoria = produkt.pobierzKategorie();
        if (find(kategorie.begin(), kategorie.end(), kategoria) ==
            kategorie.end()) {
            kategorie.push_back(kategoria);
        }
    }
    sort(kategorie.begin(), kategorie.end());
    pokazNaglowek("KATEGORIE");
    cout << "0. Wszystkie kategorie\n";
    int numer = 1;
    for (const auto& kategoria : kategorie) {
        cout << numer << ". " << kategoria << '\n';
        ++numer;
    }
    int wybor = wczytajInt("Kategoria: ");
    if (wybor < 0 || wybor >= numer) {
        throw invalid_argument("Nieznana kategoria.");
    }
    return wybor == 0 ? "" : kategorie[wybor - 1];
}
void Konsola::dodajZListy(int id_klienta, const vector<Produkt>& produkty) {
    if (produkty.empty()) {
        return;
    }
    int id_produktu = wczytajInt("Id produktu do koszyka (0 = powrot): ");
    if (id_produktu == 0) {
        return;
    }
    for (const auto& produkt : produkty) {
        if (produkt.pobierzId() == id_produktu) {
            int liczba = wczytajInt("Liczba sztuk: ");
            system_.dodajDoKoszyka(id_klienta, id_produktu, liczba);
            cout << "[OK] Dodano: " << produkt.pobierzNazwe() << " x " << liczba
                 << '\n';
            return;
        }
    }
    throw invalid_argument("Wybierz ID z wyswietlonej listy.");
}
void Konsola::wyszukaj(const Uzytkownik& uzytkownik, bool dodawanie) {
    auto kategoria = wybierzKategorie();
    auto nazwa = wczytajTekst("Fragment nazwy (Enter = wszystkie): ");
    auto produkty = system_.wyszukajProdukty(nazwa, kategoria);
    // Wybor z listy oznacza dokladna kategorie, nie fragment jej nazwy.
    vector<Produkt> wyniki;
    for (const auto& produkt : produkty) {
        if (kategoria.empty() || produkt.pobierzKategorie() == kategoria) {
            wyniki.push_back(produkt);
        }
    }
    pokazProdukty(wyniki);
    if (dodawanie && !uzytkownik.mozeZarzadzacSklepem()) {
        dodajZListy(uzytkownik.pobierzId(), wyniki);
    }
}
void Konsola::pokazKoszyk(int id_klienta) const {
    pokazNaglowek("KOSZYK");
    const auto& koszyk = system_.pobierzKlienta(id_klienta).pobierzKoszyk();
    if (koszyk.czyPusty()) {
        cout << "Koszyk jest pusty.\n";
        return;
    }
    for (const auto& pozycja : koszyk.pobierzPozycje()) {
        const auto& produkt =
            system_.pobierzProdukt(pozycja.pobierzIdProduktu());
        cout << produkt.pobierzId() << " | " << produkt.pobierzNazwe() << " x "
             << pozycja.pobierzLiczbe() << " = ";
        pokazKwote(pozycja.obliczWartosc(produkt.pobierzCeneGrosze()));
        if (!produkt.czyDostepny(pozycja.pobierzLiczbe())) {
            cout << " [niedostepny w tej liczbie]";
        }
        cout << '\n';
    }
    cout << "Razem: ";
    pokazKwote(system_.obliczWartoscKoszyka(id_klienta));
    cout << '\n';
}
void Konsola::pokazZamowienia(const Uzytkownik& uzytkownik) const {
    pokazNaglowek("ZAMOWIENIA");
    auto zamowienia = system_.pobierzZamowienia(uzytkownik);
    if (zamowienia.empty()) {
        cout << "Brak zamowien.\n";
    }
    for (const auto& zamowienie : zamowienia) {
        cout << "Zamowienie #" << zamowienie.pobierzId()
             << " | klient: " << zamowienie.pobierzIdKlienta() << " | "
             << nazwaStatusu(zamowienie.pobierzStatus()) << " | "
             << (zamowienie.pobierzPlatnosc().czyOplacona() ? "OPLACONE"
                                                            : "NIEOPLACONE")
             << '\n';
        for (const auto& pozycja : zamowienie.pobierzPozycje()) {
            cout << "  " << pozycja.pobierzNazweProduktu() << " x "
                 << pozycja.pobierzLiczbe() << " = ";
            pokazKwote(pozycja.obliczWartosc());
            cout << '\n';
        }
        cout << "  Razem: ";
        pokazKwote(zamowienie.obliczWartosc());
        cout << '\n';
    }
}
void Konsola::pokazMenu(bool administrator) const {
    cout << "\n1. Katalog\n2. Wyszukiwanie\n";
    if (administrator) {
        cout << "3. Dodaj produkt\n4. Edytuj produkt\n"
             << "5. Uzupelnij stan\n6. Wycofaj produkt\n"
             << "7. Wszystkie zamowienia\n8. Zmien status\n";
    } else {
        cout << "3. Dodaj do koszyka\n4. Pokaz koszyk\n"
             << "5. Zmien liczbe sztuk\n6. Usun z koszyka\n"
             << "7. Zloz zamowienie\n8. Moje zamowienia\n"
             << "9. Oplac zamowienie (symulacja)\n";
    }
    cout << "0. Zmien uzytkownika\n";
}
int Konsola::wybierzPozycjeKoszyka(int id_klienta) {
    pokazKoszyk(id_klienta);
    const auto& koszyk = system_.pobierzKlienta(id_klienta).pobierzKoszyk();
    if (koszyk.czyPusty()) {
        return 0;
    }
    int id_produktu = wczytajInt("Id produktu z koszyka (0 = powrot): ");
    if (id_produktu == 0) {
        return 0;
    }
    for (const auto& pozycja : koszyk.pobierzPozycje()) {
        if (pozycja.pobierzIdProduktu() == id_produktu) {
            return id_produktu;
        }
    }
    throw invalid_argument("Wybierz ID produktu z koszyka.");
}
int Konsola::wybierzZamowienie(const Uzytkownik& uzytkownik) {
    pokazZamowienia(uzytkownik);
    auto zamowienia = system_.pobierzZamowienia(uzytkownik);
    if (zamowienia.empty()) {
        return 0;
    }
    int id_zamowienia = wczytajInt("Id zamowienia z listy (0 = powrot): ");
    if (id_zamowienia == 0) {
        return 0;
    }
    for (const auto& zamowienie : zamowienia) {
        if (zamowienie.pobierzId() == id_zamowienia) {
            return id_zamowienia;
        }
    }
    throw invalid_argument("Wybierz ID zamowienia z wyswietlonej listy.");
}
int Konsola::wybierzProdukt(const Uzytkownik& administrator) {
    const auto& produkty = system_.pobierzProdukty(administrator);
    pokazProdukty(produkty);
    if (produkty.empty()) {
        return 0;
    }
    int id_produktu = wczytajInt("Id produktu z listy (0 = powrot): ");
    if (id_produktu == 0) {
        return 0;
    }
    system_.pobierzProdukt(id_produktu);
    return id_produktu;
}
void Konsola::edytujKoszyk(int id_klienta, bool usun) {
    int id_produktu = wybierzPozycjeKoszyka(id_klienta);
    if (id_produktu == 0) {
        return;
    }
    if (usun) {
        system_.usunZKoszyka(id_klienta, id_produktu);
        cout << "[OK] Usunieto produkt z koszyka.\n";
    } else {
        int liczba = wczytajInt("Nowa liczba sztuk: ");
        system_.zmienLiczbeWKoszyku(id_klienta, id_produktu, liczba);
        cout << "[OK] Zmieniono liczbe sztuk.\n";
    }
}
void Konsola::obsluzKlienta(int wybor, int id_klienta) {
    const auto& klient = system_.pobierzUzytkownika(id_klienta);
    switch (wybor) {
        case 3:
            wyszukaj(klient, true);
            return;
        case 4:
            pokazKoszyk(id_klienta);
            return;
        case 5:
            edytujKoszyk(id_klienta, false);
            return;
        case 6:
            edytujKoszyk(id_klienta, true);
            return;
        case 7: {
            int id_zamowienia = system_.zlozZamowienie(id_klienta);
            cout << "[OK] Utworzono zamowienie #" << id_zamowienia << '\n';
            return;
        }
        case 8:
            pokazZamowienia(klient);
            return;
        case 9: {
            int id_zamowienia = wybierzZamowienie(klient);
            if (id_zamowienia != 0) {
                system_.oplacZamowienie(id_klienta, id_zamowienia);
                cout << "[OK] Oplacono zamowienie #" << id_zamowienia << '\n';
            }
            return;
        }
        default:
            throw invalid_argument("Nieznana opcja.");
    }
}
void Konsola::zapiszProdukt(bool nowy, const Uzytkownik& administrator) {
    int id_produktu = nowy ? 0 : wybierzProdukt(administrator);
    if (!nowy && id_produktu == 0) {
        return;
    }
    auto nazwa = wczytajTekst("Nazwa: ");
    auto kategoria = wczytajTekst("Kategoria: ");
    auto cena = wczytajLiczbe("Cena w groszach (4999 = 49,99 zl): ");
    if (nowy) {
        int stan = wczytajInt("Stan poczatkowy: ");
        id_produktu =
            system_.dodajProdukt(administrator, nazwa, kategoria, cena, stan);
        cout << "[OK] Dodano produkt #" << id_produktu << '\n';
    } else {
        system_.zmienProdukt(administrator, id_produktu, nazwa, kategoria,
                             cena);
        cout << "[OK] Zmieniono dane produktu.\n";
    }
}
void Konsola::zmienStatus(const Uzytkownik& administrator) {
    int id_zamowienia = wybierzZamowienie(administrator);
    if (id_zamowienia == 0) {
        return;
    }

    const auto& zamowienie = system_.pobierzZamowienie(id_zamowienia);
    cout << "Aktualny status: " << nazwaStatusu(zamowienie.pobierzStatus())
         << '\n';

    cout << "1. W realizacji\n2. Wyslane\n3. Dostarczone\n";
    int wybor = wczytajInt("Nowy status: ");
    StatusZamowienia status = StatusZamowienia::NOWE;
    switch (wybor) {
        case 1:
            status = StatusZamowienia::W_REALIZACJI;
            break;
        case 2:
            status = StatusZamowienia::WYSLANE;
            break;
        case 3:
            status = StatusZamowienia::DOSTARCZONE;
            break;
        default:
            throw invalid_argument("Nieznany status.");
    }
    system_.zmienStatusZamowienia(administrator, id_zamowienia, status);
    cout << "[OK] Nowy status: " << nazwaStatusu(status) << '\n';
}
void Konsola::obsluzAdministratora(int wybor, const Uzytkownik& administrator) {
    switch (wybor) {
        case 3:
            zapiszProdukt(true, administrator);
            return;
        case 4:
            zapiszProdukt(false, administrator);
            return;
        case 5:
        case 6: {
            int id_produktu = wybierzProdukt(administrator);
            if (id_produktu == 0) {
                return;
            }
            if (wybor == 5) {
                int liczba = wczytajInt("Dodawana liczba sztuk: ");
                system_.uzupelnijStanProduktu(administrator, id_produktu,
                                              liczba);
                cout << "[OK] Uzupelniono stan produktu.\n";
            } else {
                system_.wycofajProdukt(administrator, id_produktu);
                cout << "[OK] Wycofano produkt.\n";
            }
            return;
        }
        case 7:
            pokazZamowienia(administrator);
            return;
        case 8:
            zmienStatus(administrator);
            return;
        default:
            throw invalid_argument("Nieznana opcja.");
    }
}
void Konsola::sesja(int id_uzytkownika) {
    const auto& uzytkownik = system_.pobierzUzytkownika(id_uzytkownika);
    while (cin) {
        try {
            pokazNaglowek(" MENU | " + uzytkownik.pobierzNazwe() +
                          (uzytkownik.mozeZarzadzacSklepem()
                               ? " | Administrator"
                               : " | Klient"));
            pokazMenu(uzytkownik.mozeZarzadzacSklepem());
            int wybor = wczytajInt("Wybor: ");
            if (wybor == 0) {
                return;
            }
            pokazNaglowek(" WYNIK OPERACJI");
            if (wybor == 1) {
                pokazProdukty(uzytkownik.mozeZarzadzacSklepem()
                                  ? system_.pobierzProdukty(uzytkownik)
                                  : system_.wyszukajProdukty("", ""));
            } else if (wybor == 2) {
                wyszukaj(uzytkownik, false);
            } else if (uzytkownik.mozeZarzadzacSklepem()) {
                obsluzAdministratora(wybor, uzytkownik);
            } else {
                obsluzKlienta(wybor, id_uzytkownika);
            }
        } catch (const exception& blad) {
            if (cin) {
                cout << "[BLAD] " << blad.what() << '\n';
            }
        }
    }
}
void Konsola::uruchom() {
    pokazNaglowek(" SKLEP INTERNETOWY");
    //cout << "Dane w pamieci | Platnosci symulowane | Wybor + Enter\n";
    while (cin) {
        try {
            pokazNaglowek(" WYBOR UZYTKOWNIKA");
            cout << "1. Anna (klient)\n2. Jan (klient)\n"
                 << "3. Administrator\n0. Zakoncz\n";
            int id_uzytkownika = wczytajInt("Uzytkownik: ");
            if (id_uzytkownika == 0) {
                return;
            }
            sesja(id_uzytkownika);
        } catch (const exception& blad) {
            if (cin) {
                cout << "[BLAD] " << blad.what() << '\n';
            }
        }
    }
}
