#include "pozycja_zamowienia.h"
#include <limits>
#include <stdexcept>

using namespace std;

PozycjaZamowienia::PozycjaZamowienia(int id_produktu,
                                     const string& nazwa_produktu,
                                     int64_t cena_jednostkowa_grosze,
                                     int liczba)
    : id_produktu_(id_produktu),
      nazwa_produktu_(nazwa_produktu),
      cena_jednostkowa_grosze_(cena_jednostkowa_grosze),
      liczba_(liczba) {
    if (id_produktu <= 0 || nazwa_produktu.empty() || liczba <= 0 ||
        cena_jednostkowa_grosze <= 0 ||
        cena_jednostkowa_grosze > numeric_limits<int64_t>::max() / liczba) {
        throw invalid_argument("Niepoprawna pozycja zamowienia.");
    }
}
int PozycjaZamowienia::pobierzIdProduktu() const {
    return id_produktu_;
}
const string& PozycjaZamowienia::pobierzNazweProduktu() const {
    return nazwa_produktu_;
}
int64_t PozycjaZamowienia::pobierzCeneJednostkowaGrosze() const {
    return cena_jednostkowa_grosze_;
}
int PozycjaZamowienia::pobierzLiczbe() const {
    return liczba_;
}
int64_t PozycjaZamowienia::obliczWartosc() const {
    return cena_jednostkowa_grosze_ * liczba_;
}
