#include "administrator.h"
#include "system_sklepu.h"
#include <cassert>
#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>

using namespace std;

namespace {
template <typename Operacja>
void oczekujBledu(Operacja operacja, const string& komunikat = "") {
    bool odrzucono = false;
    try {
        operacja();
    } catch (const logic_error& blad) {
        assert(string(blad.what()).find(komunikat) != string::npos);
        odrzucono = true;
    } catch (const overflow_error& blad) {
        assert(string(blad.what()).find(komunikat) != string::npos);
        odrzucono = true;
    }
    assert(odrzucono);
}

void sprawdzKomunikatyStatusow() {
    Zamowienie zamowienie(1, 1, {PozycjaZamowienia(1, "Mysz", 4999, 1)});
    oczekujBledu(
        [&] { zamowienie.zmienStatus(StatusZamowienia::W_REALIZACJI); },
        "Najpierw klient musi oplacic");
    zamowienie.oplac();
    oczekujBledu([&] { zamowienie.zmienStatus(StatusZamowienia::WYSLANE); },
                 "Nie mozna cofac ani pomijac");
    zamowienie.zmienStatus(StatusZamowienia::W_REALIZACJI);
    oczekujBledu(
        [&] { zamowienie.zmienStatus(StatusZamowienia::W_REALIZACJI); },
        "ma juz wybrany status");
    assert(zamowienie.pobierzStatus() == StatusZamowienia::W_REALIZACJI);
    cout << "OK: rozne przyczyny odrzucenia zmiany statusu\n";
}

void sprawdzZakupy() {
    SystemSklepu system;
    system.dodajUzytkownika(make_unique<Klient>(1, "Anna"));
    system.dodajUzytkownika(make_unique<Klient>(2, "Jan"));
    system.dodajUzytkownika(make_unique<Administrator>(3, "Admin"));
    const auto& admin = system.pobierzUzytkownika(3);
    const auto& klient = system.pobierzUzytkownika(1);
    assert(admin.mozeZarzadzacSklepem());
    assert(!klient.mozeZarzadzacSklepem());
    int mysz = system.dodajProdukt(admin, "Mysz", "Akcesoria", 4999, 5);
    int ekran = system.dodajProdukt(admin, "Monitor", "Monitory", 10000, 2);
    assert(system.wyszukajProdukty("Mysz", "").size() == 1);
    assert(system.wyszukajProdukty("", "Monitory").size() == 1);
    assert(system.wyszukajProdukty("", "").size() == 2);
    assert(system.wyszukajProdukty("brak", "").empty());
    assert(system.wyszukajProdukty("mYsZ", "AKCESORIA").size() == 1);
    assert(system.wyszukajProdukty("MON", "mon").size() == 1);
    assert(system.wyszukajProdukty("Mysz", "Monitory").empty());
    oczekujBledu([&] { system.zlozZamowienie(1); });
    system.dodajDoKoszyka(1, mysz, 1);
    system.dodajDoKoszyka(1, mysz, 1);
    assert(system.pobierzKlienta(1).pobierzKoszyk().pobierzPozycje().size() ==
           1);
    assert(system.obliczWartoscKoszyka(1) == 9998);
    oczekujBledu([&] { system.dodajDoKoszyka(1, mysz, 4); });
    oczekujBledu([&] { system.dodajDoKoszyka(1, mysz, 0); });
    oczekujBledu([&] { system.zmienLiczbeWKoszyku(1, mysz, -1); });
    oczekujBledu([&] { system.zmienLiczbeWKoszyku(1, ekran, 1); });
    assert(system.obliczWartoscKoszyka(1) == 9998);
    system.zmienLiczbeWKoszyku(1, mysz, 3);
    system.dodajDoKoszyka(1, ekran, 1);
    system.usunZKoszyka(1, ekran);
    assert(system.obliczWartoscKoszyka(1) == 14997);
    system.zmienProdukt(admin, mysz, "Mysz nowa", "Akcesoria", 5000);
    assert(system.obliczWartoscKoszyka(1) == 15000);
    int id = system.zlozZamowienie(1);
    assert(id == 1);
    assert(system.pobierzKlienta(1).pobierzKoszyk().czyPusty());
    assert(system.pobierzProdukt(mysz).pobierzStanMagazynowy() == 2);
    const auto& zamowienie = system.pobierzZamowienie(id);
    assert(zamowienie.obliczWartosc() == 15000);
    assert(zamowienie.pobierzPlatnosc().pobierzKwoteGrosze() == 15000);
    assert(zamowienie.pobierzStatus() == StatusZamowienia::NOWE);
    system.zmienProdukt(admin, mysz, "Inna nazwa", "Inna", 6000);
    assert(zamowienie.pobierzPozycje()[0].pobierzNazweProduktu() ==
           "Mysz nowa");
    assert(zamowienie.obliczWartosc() == 15000);
    oczekujBledu([&] { system.oplacZamowienie(2, id); });
    oczekujBledu([&] {
        system.zmienStatusZamowienia(admin, id, StatusZamowienia::W_REALIZACJI);
    });
    system.oplacZamowienie(1, id);
    oczekujBledu([&] { system.oplacZamowienie(1, id); });
    oczekujBledu([&] {
        system.zmienStatusZamowienia(admin, id, StatusZamowienia::WYSLANE);
    });
    system.zmienStatusZamowienia(admin, id, StatusZamowienia::W_REALIZACJI);
    system.zmienStatusZamowienia(admin, id, StatusZamowienia::WYSLANE);
    system.zmienStatusZamowienia(admin, id, StatusZamowienia::DOSTARCZONE);
    oczekujBledu([&] {
        system.zmienStatusZamowienia(admin, id, StatusZamowienia::WYSLANE);
    });
    assert(system.pobierzZamowienia(klient).size() == 1);
    assert(system.pobierzZamowienia(system.pobierzUzytkownika(2)).empty());
    assert(system.pobierzZamowienia(admin).size() == 1);
    cout << "OK: wyszukiwanie, koszyk, ceny, zamowienie, platnosc, statusy\n";
}

void sprawdzBrakCzesciowychZmian() {
    SystemSklepu system;
    system.dodajUzytkownika(make_unique<Klient>(1, "Anna"));
    system.dodajUzytkownika(make_unique<Klient>(2, "Jan"));
    system.dodajUzytkownika(make_unique<Administrator>(3, "Admin"));
    const auto& admin = system.pobierzUzytkownika(3);
    int pierwszy = system.dodajProdukt(admin, "A", "K", 100, 5);
    int drugi = system.dodajProdukt(admin, "B", "K", 200, 1);
    system.dodajDoKoszyka(1, pierwszy, 2);
    system.dodajDoKoszyka(1, drugi, 1);
    system.dodajDoKoszyka(2, drugi, 1);
    system.zlozZamowienie(2);
    oczekujBledu([&] { system.zlozZamowienie(1); });
    assert(system.pobierzProdukt(pierwszy).pobierzStanMagazynowy() == 5);
    assert(system.obliczWartoscKoszyka(1) == 400);
    assert(system.pobierzZamowienia(admin).size() == 1);
    system.uzupelnijStanProduktu(admin, drugi, 2);
    system.wycofajProdukt(admin, drugi);
    oczekujBledu([&] { system.zlozZamowienie(1); });
    oczekujBledu([&] { system.dodajDoKoszyka(2, drugi, 1); });
    assert(system.wyszukajProdukty("B", "").empty());
    assert(system.pobierzProdukty(admin).size() == 2);
    system.usunZKoszyka(1, drugi);
    assert(system.zlozZamowienie(1) == 2);
    assert(system.pobierzProdukt(pierwszy).pobierzStanMagazynowy() == 3);
    cout << "OK: brak czesciowych zmian, dwa koszyki, wycofanie produktu\n";
}

void sprawdzUprawnienia() {
    SystemSklepu system;
    system.dodajUzytkownika(make_unique<Klient>(1, "Anna"));
    system.dodajUzytkownika(make_unique<Administrator>(3, "Admin"));
    const auto& klient = system.pobierzUzytkownika(1);
    const auto& admin = system.pobierzUzytkownika(3);
    int id = system.dodajProdukt(admin, "A", "K", 100, 3);
    oczekujBledu([&] { system.dodajProdukt(klient, "B", "K", 1, 1); });
    oczekujBledu([&] { system.zmienProdukt(klient, id, "B", "K", 1); });
    oczekujBledu([&] { system.uzupelnijStanProduktu(klient, id, 2); });
    oczekujBledu([&] { system.wycofajProdukt(klient, id); });
    oczekujBledu([&] { system.pobierzProdukty(klient); });
    system.dodajDoKoszyka(1, id, 1);
    int zamowienie = system.zlozZamowienie(1);
    system.oplacZamowienie(1, zamowienie);
    oczekujBledu([&] {
        system.zmienStatusZamowienia(klient, zamowienie,
                                     StatusZamowienia::W_REALIZACJI);
    });
    Administrator obcy(3, "Obcy");
    oczekujBledu([&] { system.wycofajProdukt(obcy, id); });
    oczekujBledu([&] { system.pobierzZamowienia(obcy); });
    oczekujBledu([&] { system.pobierzKlienta(3); });
    oczekujBledu([&] { system.pobierzUzytkownika(999); });
    oczekujBledu([&] { system.pobierzProdukt(999); });
    oczekujBledu([&] { system.pobierzZamowienie(999); });
    oczekujBledu([&] { system.dodajUzytkownika(nullptr); });
    oczekujBledu(
        [&] { system.dodajUzytkownika(make_unique<Klient>(1, "Duplikat")); });
    const SystemSklepu& widok = system;
    assert(widok.pobierzKlienta(1).pobierzKoszyk().czyPusty());
    oczekujBledu([&] { widok.pobierzKlienta(3); });
    cout << "OK: uprawnienia, nieistniejace obiekty, duplikaty\n";
}

void sprawdzGranice() {
    oczekujBledu([] { Produkt(1, " ", "K", 1, 1); });
    oczekujBledu([] { Produkt(1, "A", "K", 0, 1); });
    oczekujBledu([] { Produkt(1, "A", "K", 1, -1); });
    Produkt produkt(1, "A", "K", 1, numeric_limits<int>::max());
    oczekujBledu([&] { produkt.uzupelnijStan(1); });
    oczekujBledu([&] { produkt.zmienDane("", "K", 2); });
    assert(produkt.pobierzNazwe() == "A");
    PozycjaKoszyka pozycja(1, numeric_limits<int>::max());
    oczekujBledu([&] { pozycja.zwiekszLiczbe(1); });
    oczekujBledu(
        [&] { pozycja.obliczWartosc(numeric_limits<int64_t>::max()); });
    SystemSklepu system;
    system.dodajUzytkownika(make_unique<Klient>(1, "Anna"));
    system.dodajUzytkownika(make_unique<Administrator>(3, "Admin"));
    const auto& admin = system.pobierzUzytkownika(3);
    const auto MAX_KWOTA = numeric_limits<int64_t>::max();
    int pierwszy = system.dodajProdukt(admin, "A", "K", MAX_KWOTA, 2);
    int drugi = system.dodajProdukt(admin, "B", "K", 1, 1);
    system.dodajDoKoszyka(1, pierwszy, 1);
    system.dodajDoKoszyka(1, drugi, 1);
    oczekujBledu([&] { system.obliczWartoscKoszyka(1); });
    oczekujBledu([&] { system.zlozZamowienie(1); });
    assert(system.pobierzProdukt(pierwszy).pobierzStanMagazynowy() == 2);
    assert(system.pobierzZamowienia(admin).empty());
    assert(!system.pobierzKlienta(1).pobierzKoszyk().czyPusty());
    system.usunZKoszyka(1, drugi);
    system.zmienLiczbeWKoszyku(1, pierwszy, 2);
    oczekujBledu([&] { system.zlozZamowienie(1); });
    assert(system.pobierzProdukt(pierwszy).pobierzStanMagazynowy() == 2);
    cout << "OK: walidacja i przepelnienie liczb\n";
}
}  // namespace
int main() {
    sprawdzKomunikatyStatusow();
    sprawdzZakupy();
    sprawdzBrakCzesciowychZmian();
    sprawdzUprawnienia();
    sprawdzGranice();
}
