#pragma once

#include <cstdint>
#include <vector>
#include "pozycja_koszyka.h"
#include "produkt.h"

using std::int64_t;
using std::vector;

class Koszyk {
public:
    Koszyk();
    void dodajPozycje(int id_produktu, int liczba);
    void zmienLiczbe(int id_produktu, int liczba);
    void usunPozycje(int id_produktu);
    int64_t obliczWartosc(const vector<Produkt>& produkty) const;
    const vector<PozycjaKoszyka>& pobierzPozycje() const;
    bool czyPusty() const;
    void wyczysc();

private:
    vector<PozycjaKoszyka> pozycje_;
};
