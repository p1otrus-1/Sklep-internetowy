#include "zamowienie.h"
#include <limits>
#include <stdexcept>

using namespace std;

Zamowienie::Zamowienie(int id, int id_klienta,
                       const vector<PozycjaZamowienia>& pozycje)
    : id_(id),
      id_klienta_(id_klienta),
      status_(StatusZamowienia::NOWE),
      pozycje_(pozycje),
      platnosc_(obliczWartosc()) {
    if (id <= 0 || id_klienta <= 0 || pozycje.empty()) {
        throw invalid_argument("Niepoprawne zamowienie.");
    }
}
int Zamowienie::pobierzId() const {
    return id_;
}
int Zamowienie::pobierzIdKlienta() const {
    return id_klienta_;
}
StatusZamowienia Zamowienie::pobierzStatus() const {
    return status_;
}
const vector<PozycjaZamowienia>& Zamowienie::pobierzPozycje() const {
    return pozycje_;
}
const Platnosc& Zamowienie::pobierzPlatnosc() const {
    return platnosc_;
}
int64_t Zamowienie::obliczWartosc() const {
    int64_t suma = 0;
    for (const auto& pozycja : pozycje_) {
        if (pozycja.obliczWartosc() > numeric_limits<int64_t>::max() - suma) {
            throw overflow_error("Zbyt duza wartosc zamowienia.");
        }
        suma += pozycja.obliczWartosc();
    }
    return suma;
}
void Zamowienie::oplac() {
    platnosc_.potwierdz();
}
void Zamowienie::zmienStatus(StatusZamowienia nowy_status) {
    if (nowy_status == status_) {
        throw logic_error("Zamowienie ma juz wybrany status.");
    }

    if (!platnosc_.czyOplacona()) {
        throw logic_error("Najpierw klient musi oplacic zamowienie.");
    }

    bool poprawny = (status_ == StatusZamowienia::NOWE &&
                     nowy_status == StatusZamowienia::W_REALIZACJI) ||
                    (status_ == StatusZamowienia::W_REALIZACJI &&
                     nowy_status == StatusZamowienia::WYSLANE) ||
                    (status_ == StatusZamowienia::WYSLANE &&
                     nowy_status == StatusZamowienia::DOSTARCZONE);

    if (!poprawny) {
        throw logic_error(
            "Nie mozna cofac ani pomijac etapow. Kolejnosc: "
            "Nowe -> W realizacji -> Wyslane -> Dostarczone.");
    }

    status_ = nowy_status;
}
