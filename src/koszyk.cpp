#include "koszyk.h"
#include <limits>
#include <stdexcept>

using namespace std;

Koszyk::Koszyk() = default;

void Koszyk::dodajPozycje(int id_produktu, int liczba) {
    if (id_produktu <= 0 || liczba <= 0) {
        throw invalid_argument("Niepoprawne dane pozycji.");
    }
    for (auto& pozycja : pozycje_) {
        if (pozycja.pobierzIdProduktu() == id_produktu) {
            pozycja.zwiekszLiczbe(liczba);
            return;
        }
    }
    pozycje_.emplace_back(id_produktu, liczba);
}

void Koszyk::zmienLiczbe(int id_produktu, int liczba) {
    for (auto& pozycja : pozycje_) {
        if (pozycja.pobierzIdProduktu() == id_produktu) {
            pozycja.zmienLiczbe(liczba);
            return;
        }
    }
    throw invalid_argument("Brak produktu w koszyku.");
}

void Koszyk::usunPozycje(int id_produktu) {
    for (auto it = pozycje_.begin(); it != pozycje_.end(); ++it) {
        if (it->pobierzIdProduktu() == id_produktu) {
            pozycje_.erase(it);
            return;
        }
    }
    throw invalid_argument("Brak produktu w koszyku.");
}

int64_t Koszyk::obliczWartosc(const vector<Produkt>& produkty) const {
    int64_t suma = 0;
    for (const auto& pozycja : pozycje_) {
        bool znaleziono = false;
        for (const auto& produkt : produkty) {
            if (produkt.pobierzId() == pozycja.pobierzIdProduktu()) {
                const auto WARTOSC =
                    pozycja.obliczWartosc(produkt.pobierzCeneGrosze());
                if (WARTOSC > numeric_limits<int64_t>::max() - suma) {
                    throw overflow_error("Zbyt duza wartosc koszyka.");
                }
                suma += WARTOSC;
                znaleziono = true;
                break;
            }
        }
        if (!znaleziono) {
            throw invalid_argument(
                "Produkt z koszyka nie istnieje w katalogu.");
        }
    }
    return suma;
}

const vector<PozycjaKoszyka>& Koszyk::pobierzPozycje() const {
    return pozycje_;
}
bool Koszyk::czyPusty() const {
    return pozycje_.empty();
}
void Koszyk::wyczysc() {
    pozycje_.clear();
}
