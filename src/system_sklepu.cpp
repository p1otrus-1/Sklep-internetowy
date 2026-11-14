#include "system_sklepu.h"
#include <limits>
#include <stdexcept>
#include <utility>

using namespace std;

namespace {
string maleLiteryAscii(string tekst) {
    for (char& znak : tekst) {
        if (znak >= 'A' && znak <= 'Z') {
            znak = static_cast<char>(znak - 'A' + 'a');
        }
    }
    return tekst;
}
}  // namespace

SystemSklepu::SystemSklepu()
    : nastepne_id_produktu_(1), nastepne_id_zamowienia_(1) {}
SystemSklepu::~SystemSklepu() = default;

void SystemSklepu::dodajUzytkownika(unique_ptr<Uzytkownik> uzytkownik) {
    if (!uzytkownik) {
        throw invalid_argument("Brak uzytkownika.");
    }
    for (const auto& istniejacy : uzytkownicy_) {
        if (istniejacy->pobierzId() == uzytkownik->pobierzId()) {
            throw invalid_argument("Id uzytkownika juz istnieje.");
        }
    }
    uzytkownicy_.push_back(move(uzytkownik));
}
const Uzytkownik& SystemSklepu::pobierzUzytkownika(int id_uzytkownika) const {
    for (const auto& uzytkownik : uzytkownicy_) {
        if (uzytkownik->pobierzId() == id_uzytkownika) {
            return *uzytkownik;
        }
    }
    throw invalid_argument("Nie znaleziono uzytkownika.");
}
Klient& SystemSklepu::pobierzKlienta(int id_klienta) {
    for (auto& uzytkownik : uzytkownicy_) {
        if (uzytkownik->pobierzId() == id_klienta) {
            auto* klient = dynamic_cast<Klient*>(uzytkownik.get());
            if (klient != nullptr) {
                return *klient;
            }
        }
    }
    throw invalid_argument("Nie znaleziono klienta.");
}
const Klient& SystemSklepu::pobierzKlienta(int id_klienta) const {
    const auto& uzytkownik = pobierzUzytkownika(id_klienta);
    const auto* klient = dynamic_cast<const Klient*>(&uzytkownik);
    if (klient == nullptr) {
        throw invalid_argument("Uzytkownik nie jest klientem.");
    }
    return *klient;
}
const Produkt& SystemSklepu::pobierzProdukt(int id_produktu) const {
    for (const auto& produkt : produkty_) {
        if (produkt.pobierzId() == id_produktu) {
            return produkt;
        }
    }
    throw invalid_argument("Nie znaleziono produktu.");
}
Produkt& SystemSklepu::znajdzProdukt(int id_produktu) {
    for (auto& produkt : produkty_) {
        if (produkt.pobierzId() == id_produktu) {
            return produkt;
        }
    }
    throw invalid_argument("Nie znaleziono produktu.");
}
const Zamowienie& SystemSklepu::pobierzZamowienie(int id_zamowienia) const {
    for (const auto& zamowienie : zamowienia_) {
        if (zamowienie.pobierzId() == id_zamowienia) {
            return zamowienie;
        }
    }
    throw invalid_argument("Nie znaleziono zamowienia.");
}
Zamowienie& SystemSklepu::znajdzZamowienie(int id_zamowienia) {
    for (auto& zamowienie : zamowienia_) {
        if (zamowienie.pobierzId() == id_zamowienia) {
            return zamowienie;
        }
    }
    throw invalid_argument("Nie znaleziono zamowienia.");
}
void SystemSklepu::sprawdzAdministratora(const Uzytkownik& uzytkownik) const {
    const auto& zapisany = pobierzUzytkownika(uzytkownik.pobierzId());
    if (&zapisany != &uzytkownik || !zapisany.mozeZarzadzacSklepem()) {
        throw logic_error("Wymagane uprawnienia administratora.");
    }
}
vector<Produkt> SystemSklepu::wyszukajProdukty(const string& nazwa,
                                               const string& kategoria) const {
    vector<Produkt> wyniki;
    for (const auto& produkt : produkty_) {
        if (produkt.czyAktywny() &&
            maleLiteryAscii(produkt.pobierzNazwe())
                    .find(maleLiteryAscii(nazwa)) != string::npos &&
            maleLiteryAscii(produkt.pobierzKategorie())
                    .find(maleLiteryAscii(kategoria)) != string::npos) {
            wyniki.push_back(produkt);
        }
    }
    return wyniki;
}
const vector<Produkt>& SystemSklepu::pobierzProdukty(
    const Uzytkownik& uzytkownik) const {
    sprawdzAdministratora(uzytkownik);
    return produkty_;
}
vector<Zamowienie> SystemSklepu::pobierzZamowienia(
    const Uzytkownik& uzytkownik) const {
    const auto& zapisany = pobierzUzytkownika(uzytkownik.pobierzId());
    if (&zapisany != &uzytkownik) {
        throw logic_error("Nieznany uzytkownik.");
    }
    vector<Zamowienie> wyniki;
    for (const auto& zamowienie : zamowienia_) {
        if (zapisany.mozeZarzadzacSklepem() ||
            zamowienie.pobierzIdKlienta() == zapisany.pobierzId()) {
            wyniki.push_back(zamowienie);
        }
    }
    return wyniki;
}
void SystemSklepu::dodajDoKoszyka(int id_klienta, int id_produktu, int liczba) {
    auto& koszyk = pobierzKlienta(id_klienta).pobierzKoszyk();
    const auto& produkt = pobierzProdukt(id_produktu);
    int obecna_liczba = 0;
    for (const auto& pozycja : koszyk.pobierzPozycje()) {
        if (pozycja.pobierzIdProduktu() == id_produktu) {
            obecna_liczba = pozycja.pobierzLiczbe();
        }
    }
    if (!produkt.czyDostepny(liczba) ||
        obecna_liczba > produkt.pobierzStanMagazynowy() - liczba) {
        throw invalid_argument("Niepoprawna liczba lub brak towaru.");
    }
    koszyk.dodajPozycje(id_produktu, liczba);
}
void SystemSklepu::zmienLiczbeWKoszyku(int id_klienta, int id_produktu,
                                       int liczba) {
    auto& koszyk = pobierzKlienta(id_klienta).pobierzKoszyk();
    if (!pobierzProdukt(id_produktu).czyDostepny(liczba)) {
        throw invalid_argument("Niepoprawna liczba lub brak towaru.");
    }
    koszyk.zmienLiczbe(id_produktu, liczba);
}
void SystemSklepu::usunZKoszyka(int id_klienta, int id_produktu) {
    pobierzKlienta(id_klienta).pobierzKoszyk().usunPozycje(id_produktu);
}
int64_t SystemSklepu::obliczWartoscKoszyka(int id_klienta) const {
    return pobierzKlienta(id_klienta).pobierzKoszyk().obliczWartosc(produkty_);
}
int SystemSklepu::zlozZamowienie(int id_klienta) {
    auto& koszyk = pobierzKlienta(id_klienta).pobierzKoszyk();
    if (koszyk.czyPusty()) {
        throw logic_error("Koszyk jest pusty.");
    }
    if (nastepne_id_zamowienia_ == numeric_limits<int>::max()) {
        throw overflow_error("Wyczerpano identyfikatory zamowien.");
    }
    vector<PozycjaZamowienia> pozycje;
    for (const auto& pozycja : koszyk.pobierzPozycje()) {
        const auto& produkt = pobierzProdukt(pozycja.pobierzIdProduktu());
        if (!produkt.czyDostepny(pozycja.pobierzLiczbe())) {
            throw logic_error("Brak towaru: " + produkt.pobierzNazwe());
        }
        pozycje.emplace_back(produkt.pobierzId(), produkt.pobierzNazwe(),
                             produkt.pobierzCeneGrosze(),
                             pozycja.pobierzLiczbe());
    }
    // Wszystkie walidacje i alokacje poprzedzaja zmiane stanu magazynu.
    zamowienia_.emplace_back(nastepne_id_zamowienia_, id_klienta, pozycje);
    for (const auto& pozycja : pozycje) {
        znajdzProdukt(pozycja.pobierzIdProduktu())
            .zmniejszStan(pozycja.pobierzLiczbe());
    }
    koszyk.wyczysc();
    int id_zamowienia = nastepne_id_zamowienia_;
    ++nastepne_id_zamowienia_;
    return id_zamowienia;
}
void SystemSklepu::oplacZamowienie(int id_klienta, int id_zamowienia) {
    pobierzKlienta(id_klienta);
    auto& zamowienie = znajdzZamowienie(id_zamowienia);
    if (zamowienie.pobierzIdKlienta() != id_klienta) {
        throw logic_error("Nie mozna oplacic cudzego zamowienia.");
    }
    zamowienie.oplac();
}
int SystemSklepu::dodajProdukt(const Uzytkownik& uzytkownik,
                               const string& nazwa, const string& kategoria,
                               int64_t cena_grosze, int stan_magazynowy) {
    sprawdzAdministratora(uzytkownik);
    if (nastepne_id_produktu_ == numeric_limits<int>::max()) {
        throw overflow_error("Wyczerpano identyfikatory produktow.");
    }
    produkty_.emplace_back(nastepne_id_produktu_, nazwa, kategoria, cena_grosze,
                           stan_magazynowy);
    int id_produktu = nastepne_id_produktu_;
    ++nastepne_id_produktu_;
    return id_produktu;
}
void SystemSklepu::zmienProdukt(const Uzytkownik& uzytkownik, int id_produktu,
                                const string& nazwa, const string& kategoria,
                                int64_t cena_grosze) {
    sprawdzAdministratora(uzytkownik);
    znajdzProdukt(id_produktu).zmienDane(nazwa, kategoria, cena_grosze);
}
void SystemSklepu::uzupelnijStanProduktu(const Uzytkownik& uzytkownik,
                                         int id_produktu, int liczba) {
    sprawdzAdministratora(uzytkownik);
    znajdzProdukt(id_produktu).uzupelnijStan(liczba);
}
void SystemSklepu::wycofajProdukt(const Uzytkownik& uzytkownik,
                                  int id_produktu) {
    sprawdzAdministratora(uzytkownik);
    znajdzProdukt(id_produktu).wycofaj();
}
void SystemSklepu::zmienStatusZamowienia(const Uzytkownik& uzytkownik,
                                         int id_zamowienia,
                                         StatusZamowienia nowy_status) {
    sprawdzAdministratora(uzytkownik);
    znajdzZamowienie(id_zamowienia).zmienStatus(nowy_status);
}
