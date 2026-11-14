#pragma once

#include <cstdint>

using std::int64_t;

class PozycjaKoszyka {
public:
    PozycjaKoszyka(int id_produktu, int liczba);
    int pobierzIdProduktu() const;
    int pobierzLiczbe() const;
    void zwiekszLiczbe(int liczba);
    void zmienLiczbe(int liczba);
    int64_t obliczWartosc(int64_t cena_jednostkowa_grosze) const;

private:
    int id_produktu_;
    int liczba_;
};
