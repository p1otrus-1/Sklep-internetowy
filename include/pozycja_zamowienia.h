#pragma once

#include <cstdint>
#include <string>

class PozycjaZamowienia {
public:
    PozycjaZamowienia(int id_produktu, const std::string& nazwa_produktu,
            std::int64_t cena_jednostkowa_grosze, int liczba);
    int pobierzIdProduktu() const;
    const std::string& pobierzNazweProduktu() const;
    std::int64_t pobierzCeneJednostkowaGrosze() const;
    int pobierzLiczbe() const;
    std::int64_t obliczWartosc() const;

private:
    int id_produktu_;
    std::string nazwa_produktu_;
    std::int64_t cena_jednostkowa_grosze_;
    int liczba_;
};
