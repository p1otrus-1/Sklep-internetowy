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
    void dodajUzytkownika(std::unique_ptr<Użytkownik> uzytkownik);
    const Użytkownik& pobierzUzytkownika(int id_uzytkownika) const;
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
    int dodajProdukt(const Użytkownik& uzytkownik, const std::string& nazwa,
            const std::string& kategoria, std::int64_t cena_grosze,
            int stan_magazynowy);
    void zmienProdukt(const Użytkownik& uzytkownik, int id_produktu,
            const std::string& nazwa, const std::string& kategoria,
            std::int64_t cena_grosze);
    void uzupelnijStanProduktu(const Użytkownik& uzytkownik, int id_produktu,
        int liczba);
    void wycofajProdukt(const Użytkownik& uzytkownik, int id_produktu);
    void zmienStatusZamowienia(const Użytkownik& uzytkownik, int id_zamowienia,
            StatusZamowienia nowy_status);
    const Zamówienie& pobierzZamowienie(int id_zamowienia) const;

private:
    std::vector<std::unique_ptr<Użytkownik>> uzytkownicy_;
    std::vector<Produkt> produkty_;
    std::vector<Zamówienie> zamowienia_;
    int nastepne_id_produktu_;
    int nastepne_id_zamowienia_;
};
