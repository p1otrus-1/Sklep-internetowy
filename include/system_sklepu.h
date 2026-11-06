#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include "uzytkownik.h"
#include "klient.h"
#include "produkt.h"
#include "zamowienie.h"

class SystemSklepu {
public:
    SystemSklepu();
    ~SystemSklepu();
    void dodajUzytkownika(std::unique_ptr<Uzytkownik> uzytkownik);
    const Uzytkownik& pobierzUzytkownika(int id_uzytkownika) const;
    Klient& pobierzKlienta(int id_klienta);
    const Klient& pobierzKlienta(int id_klienta) const;
    std::vector<Produkt> wyszukajProdukty(const std::string& nazwa,
            const std::string& kategoria) const;
    void dodajDoKoszyka(int id_klienta, int id_produktu, int liczba);
    void zmienLiczbeWKoszyku(int id_klienta, int id_produktu, int liczba);
    void usunZKoszyka(int id_klienta, int id_produktu);
    std::int64_t obliczWartoscKoszyka(int id_klienta) const;
    int zlozZamowienie(int id_klienta);
    void oplacZamowienie(int id_klienta, int id_zamowienia);
    int dodajProdukt(const Uzytkownik& uzytkownik, const std::string& nazwa,
            const std::string& kategoria, std::int64_t cena_grosze,
            int stan_magazynowy);
    void zmienProdukt(const Uzytkownik& uzytkownik, int id_produktu,
            const std::string& nazwa, const std::string& kategoria,
            std::int64_t cena_grosze);
    void uzupelnijStanProduktu(const Uzytkownik& uzytkownik, int id_produktu,
        int liczba);
    void wycofajProdukt(const Uzytkownik& uzytkownik, int id_produktu);
    void zmienStatusZamowienia(const Uzytkownik& uzytkownik, int id_zamowienia,
            StatusZamowienia nowy_status);
    const Zamowienie& pobierzZamowienie(int id_zamowienia) const;

private:
    std::vector<std::unique_ptr<Uzytkownik>> uzytkownicy_;
    std::vector<Produkt> produkty_;
    std::vector<Zamowienie> zamowienia_;
    int nastepne_id_produktu_;
    int nastepne_id_zamowienia_;
};
