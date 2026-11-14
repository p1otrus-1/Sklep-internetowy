#pragma once

#include <cstdint>
#include "typy_domenowe.h"

using std::int64_t;

class Platnosc {
public:
    explicit Platnosc(int64_t kwota_grosze);
    int64_t pobierzKwoteGrosze() const;
    StatusPlatnosci pobierzStatus() const;
    bool czyOplacona() const;
    void potwierdz();

private:
    int64_t kwota_grosze_;
    StatusPlatnosci status_;
};
