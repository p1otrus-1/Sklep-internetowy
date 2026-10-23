#pragma once

#include <cstdint>

class PozycjaKoszyka {
public:
    PozycjaKoszyka(int id_produktu, int liczba);
    int pobierzIdProduktu() const;
    int pobierzLiczbe() const;
    void zwiekszLiczbe(int liczba);
    void zmienLiczbe(int liczba);
    std::int64_t obliczWartosc(std::int64_t cena_jednostkowa_grosze) const;

private:
    int id_produktu_;
    int liczba_;
};
