#pragma once

#include <cstdint>
#include "typy_domenowe.h"

class Platnosc {
public:
    explicit Platnosc(std::int64_t kwota_grosze);
    std::int64_t pobierzKwoteGrosze() const;
    StatusPlatnosci pobierzStatus() const;
    bool czyOplacona() const;
    void potwierdz();

private:
    std::int64_t kwota_grosze_;
    StatusPlatnosci status_;
};
