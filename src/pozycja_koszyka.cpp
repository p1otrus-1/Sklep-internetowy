#include "pozycja_koszyka.h"
#include <limits>
#include <stdexcept>

using namespace std;

PozycjaKoszyka::PozycjaKoszyka(int id_produktu, int liczba)
    : id_produktu_(id_produktu), liczba_(liczba) {
    if (id_produktu <= 0 || liczba <= 0) {
        throw invalid_argument("Niepoprawna pozycja koszyka.");
    }
}

int PozycjaKoszyka::pobierzIdProduktu() const {
    return id_produktu_;
}
int PozycjaKoszyka::pobierzLiczbe() const {
    return liczba_;
}

void PozycjaKoszyka::zwiekszLiczbe(int liczba) {
    if (liczba <= 0 || liczba > numeric_limits<int>::max() - liczba_) {
        throw invalid_argument("Niepoprawna liczba sztuk.");
    }
    liczba_ += liczba;
}

void PozycjaKoszyka::zmienLiczbe(int liczba) {
    if (liczba <= 0) {
        throw invalid_argument("Liczba sztuk musi byc dodatnia.");
    }
    liczba_ = liczba;
}

int64_t PozycjaKoszyka::obliczWartosc(int64_t cena_jednostkowa_grosze) const {
    if (cena_jednostkowa_grosze < 0 ||
        cena_jednostkowa_grosze > numeric_limits<int64_t>::max() / liczba_) {
        throw invalid_argument("Niepoprawna lub zbyt duza kwota.");
    }
    return cena_jednostkowa_grosze * liczba_;
}
