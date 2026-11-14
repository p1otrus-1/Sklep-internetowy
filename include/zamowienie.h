#pragma once

#include <cstdint>
#include <vector>
#include "typy_domenowe.h"
#include "pozycja_zamowienia.h"
#include "platnosc.h"

using std::int64_t;
using std::vector;

class Zamowienie {
public:
    Zamowienie(int id, int id_klienta,
               const vector<PozycjaZamowienia>& pozycje);
    int pobierzId() const;
    int pobierzIdKlienta() const;
    StatusZamowienia pobierzStatus() const;
    const vector<PozycjaZamowienia>& pobierzPozycje() const;
    const Platnosc& pobierzPlatnosc() const;
    int64_t obliczWartosc() const;
    void oplac();
    void zmienStatus(StatusZamowienia nowy_status);

private:
    int id_;
    int id_klienta_;
    StatusZamowienia status_;
    vector<PozycjaZamowienia> pozycje_;
    Platnosc platnosc_;
};
