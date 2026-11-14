#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include "uzytkownik.h"
#include "klient.h"
#include "produkt.h"
#include "zamowienie.h"

using std::int64_t;
using std::string;
using std::unique_ptr;
using std::vector;

class SystemSklepu {
public:
    SystemSklepu();
    ~SystemSklepu();
    void dodajUzytkownika(unique_ptr<Uzytkownik> uzytkownik);
    const Uzytkownik& pobierzUzytkownika(int id_uzytkownika) const;
    Klient& pobierzKlienta(int id_klienta);
    const Klient& pobierzKlienta(int id_klienta) const;
    vector<Produkt> wyszukajProdukty(const string& nazwa,
                                     const string& kategoria) const;
    void dodajDoKoszyka(int id_klienta, int id_produktu, int liczba);
    void zmienLiczbeWKoszyku(int id_klienta, int id_produktu, int liczba);
    void usunZKoszyka(int id_klienta, int id_produktu);
    int64_t obliczWartoscKoszyka(int id_klienta) const;
    int zlozZamowienie(int id_klienta);
    void oplacZamowienie(int id_klienta, int id_zamowienia);
    int dodajProdukt(const Uzytkownik& uzytkownik, const string& nazwa,
                     const string& kategoria, int64_t cena_grosze,
                     int stan_magazynowy);
    void zmienProdukt(const Uzytkownik& uzytkownik, int id_produktu,
                      const string& nazwa, const string& kategoria,
                      int64_t cena_grosze);
    void uzupelnijStanProduktu(const Uzytkownik& uzytkownik, int id_produktu,
                               int liczba);
    void wycofajProdukt(const Uzytkownik& uzytkownik, int id_produktu);
    void zmienStatusZamowienia(const Uzytkownik& uzytkownik, int id_zamowienia,
                               StatusZamowienia nowy_status);
    const Zamowienie& pobierzZamowienie(int id_zamowienia) const;

    const Produkt& pobierzProdukt(int id_produktu) const;
    const vector<Produkt>& pobierzProdukty(const Uzytkownik& uzytkownik) const;
    vector<Zamowienie> pobierzZamowienia(const Uzytkownik& uzytkownik) const;

private:
    Produkt& znajdzProdukt(int id_produktu);
    Zamowienie& znajdzZamowienie(int id_zamowienia);
    void sprawdzAdministratora(const Uzytkownik& uzytkownik) const;

    vector<unique_ptr<Uzytkownik>> uzytkownicy_;
    vector<Produkt> produkty_;
    vector<Zamowienie> zamowienia_;
    int nastepne_id_produktu_;
    int nastepne_id_zamowienia_;
};
