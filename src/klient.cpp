#include "klient.h"

using namespace std;

Klient::Klient(int id, const string& nazwa) : Uzytkownik(id, nazwa) {}
bool Klient::mozeZarzadzacSklepem() const {
    return false;
}
Koszyk& Klient::pobierzKoszyk() {
    return koszyk_;
}
const Koszyk& Klient::pobierzKoszyk() const {
    return koszyk_;
}
