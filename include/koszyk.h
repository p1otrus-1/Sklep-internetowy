#pragma once

#include <cstdint>
#include <vector>
#include "pozycja_koszyka.h"
#include "produkt.h"

class Koszyk {
public:
    Koszyk();
    void dodajPozycje(int id_produktu, int liczba);
    void zmienLiczbe(int id_produktu, int liczba);
    void usunPozycje(int id_produktu);
    std::int64_t obliczWartosc(const std::vector<Produkt>& produkty) const;
    const std::vector<PozycjaKoszyka>& pobierzPozycje() const;
    bool czyPusty() const;
    void wyczysc();

private:
    std::vector<PozycjaKoszyka> pozycje_;
};
