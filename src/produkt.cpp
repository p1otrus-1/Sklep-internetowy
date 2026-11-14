#include "produkt.h"
#include <limits>
#include <stdexcept>

using namespace std;

Produkt::Produkt(int id, const string& nazwa, const string& kategoria,
                 int64_t cena_grosze, int stan_magazynowy)
    : id_(id),
      nazwa_(nazwa),
      kategoria_(kategoria),
      cena_grosze_(cena_grosze),
      stan_magazynowy_(stan_magazynowy),
      aktywny_(true) {
    if (id <= 0 || nazwa.find_first_not_of(" \t\r\n") == string::npos ||
        kategoria.find_first_not_of(" \t\r\n") == string::npos ||
        cena_grosze <= 0 || stan_magazynowy < 0) {
        throw invalid_argument("Niepoprawne dane produktu.");
    }
}

int Produkt::pobierzId() const {
    return id_;
}
const string& Produkt::pobierzNazwe() const {
    return nazwa_;
}
const string& Produkt::pobierzKategorie() const {
    return kategoria_;
}
int64_t Produkt::pobierzCeneGrosze() const {
    return cena_grosze_;
}
int Produkt::pobierzStanMagazynowy() const {
    return stan_magazynowy_;
}
bool Produkt::czyAktywny() const {
    return aktywny_;
}

bool Produkt::czyDostepny(int liczba) const {
    return aktywny_ && liczba > 0 && liczba <= stan_magazynowy_;
}

void Produkt::zmienDane(const string& nazwa, const string& kategoria,
                        int64_t cena_grosze) {
    if (nazwa.find_first_not_of(" \t\r\n") == string::npos ||
        kategoria.find_first_not_of(" \t\r\n") == string::npos ||
        cena_grosze <= 0) {
        throw invalid_argument("Niepoprawne dane produktu.");
    }
    nazwa_ = nazwa;
    kategoria_ = kategoria;
    cena_grosze_ = cena_grosze;
}

void Produkt::uzupelnijStan(int liczba) {
    if (liczba <= 0 || liczba > numeric_limits<int>::max() - stan_magazynowy_) {
        throw invalid_argument("Niepoprawne uzupelnienie stanu.");
    }
    stan_magazynowy_ += liczba;
}

void Produkt::zmniejszStan(int liczba) {
    if (!czyDostepny(liczba)) {
        throw invalid_argument("Produkt niedostepny w podanej liczbie.");
    }
    stan_magazynowy_ -= liczba;
}

void Produkt::wycofaj() {
    aktywny_ = false;
}
