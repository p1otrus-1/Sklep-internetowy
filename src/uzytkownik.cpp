#include "uzytkownik.h"
#include <stdexcept>

using namespace std;

Uzytkownik::Uzytkownik(int id, const string& nazwa) : id_(id), nazwa_(nazwa) {
    if (id <= 0 || nazwa.find_first_not_of(" \t\r\n") == string::npos) {
        throw invalid_argument("Niepoprawne dane uzytkownika.");
    }
}
Uzytkownik::~Uzytkownik() = default;
int Uzytkownik::pobierzId() const {
    return id_;
}
const string& Uzytkownik::pobierzNazwe() const {
    return nazwa_;
}
