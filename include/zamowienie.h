#pragma once

#include <cstdint>
#include <vector>
#include "typy_domenowe.h"
#include "pozycja_zamowienia.h"
#include "platnosc.h"

class Zamowienie {
public:
    Zamowienie(int id, int id_klienta,
        const std::vector<PozycjaZamowienia>& pozycje);
    int pobierzId() const;
    int pobierzIdKlienta() const;
    StatusZamowienia pobierzStatus() const;
    const std::vector<PozycjaZamowienia>& pobierzPozycje() const;
    const Platnosc& pobierzPlatnosc() const;
    std::int64_t obliczWartosc() const;
    void oplac();
    void zmienStatus(StatusZamowienia nowy_status);

private:
    int id_;
    int id_klienta_;
    StatusZamowienia status_;
    std::vector<PozycjaZamowienia> pozycje_;
    Platnosc platnosc_;
};
