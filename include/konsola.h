#pragma once

#include "system_sklepu.h"
#include <string>

using std::int64_t;
using std::string;
using std::vector;

class Konsola {
public:
    explicit Konsola(SystemSklepu& system);
    void uruchom();

private:
    string wczytajTekst(const string& pytanie);
    int64_t wczytajLiczbe(const string& pytanie);
    int wczytajInt(const string& pytanie);
    void pokazKwote(int64_t grosze) const;
    void pokazProdukty(const vector<Produkt>& produkty) const;
    void pokazNaglowek(const string& tytul) const;
    string wybierzKategorie();
    void wyszukaj(const Uzytkownik& uzytkownik, bool dodawanie);
    void dodajZListy(int id_klienta, const vector<Produkt>& produkty);
    void pokazKoszyk(int id_klienta) const;
    void pokazZamowienia(const Uzytkownik& uzytkownik) const;
    int wybierzPozycjeKoszyka(int id_klienta);
    int wybierzZamowienie(const Uzytkownik& uzytkownik);
    int wybierzProdukt(const Uzytkownik& administrator);
    void edytujKoszyk(int id_klienta, bool usun);
    void pokazMenu(bool administrator) const;
    void obsluzKlienta(int wybor, int id_klienta);
    void obsluzAdministratora(int wybor, const Uzytkownik& administrator);
    void zapiszProdukt(bool nowy, const Uzytkownik& administrator);
    void zmienStatus(const Uzytkownik& administrator);
    void sesja(int id_uzytkownika);

    SystemSklepu& system_;
};
