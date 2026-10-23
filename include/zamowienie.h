#pragma once

#include <cstdint>
#include <vector>
#include "typy_domenowe.h"
#include "pozycja_zamowienia.h"
#include "platnosc.h"

class Zamówienie {
public:
    Zamówienie(int id, int id_klienta,
        const std::vector<PozycjaZamówienia>& pozycje);
    int pobierzId() const;
    int pobierzIdKlienta() const;
    StatusZamowienia pobierzStatus() const;
    const std::vector<PozycjaZamówienia>& pobierzPozycje() const;
    const Płatność& pobierzPlatnosc() const;
    std::int64_t obliczWartosc() const;
    void oplac();
    void zmienStatus(StatusZamowienia nowy_status);

private:
    int id_;
    int id_klienta_;
    StatusZamowienia status_;
    std::vector<PozycjaZamówienia> pozycje_;
    Płatność platnosc_;
};
