#include "platnosc.h"
#include <stdexcept>

using namespace std;

Platnosc::Platnosc(int64_t kwota_grosze)
    : kwota_grosze_(kwota_grosze), status_(StatusPlatnosci::OCZEKUJACA) {
    if (kwota_grosze <= 0) {
        throw invalid_argument("Kwota musi byc dodatnia.");
    }
}
int64_t Platnosc::pobierzKwoteGrosze() const {
    return kwota_grosze_;
}
StatusPlatnosci Platnosc::pobierzStatus() const {
    return status_;
}
bool Platnosc::czyOplacona() const {
    return status_ == StatusPlatnosci::OPLACONA;
}
void Platnosc::potwierdz() {
    if (czyOplacona()) {
        throw logic_error("Zamowienie jest juz oplacone.");
    }
    status_ = StatusPlatnosci::OPLACONA;
}
