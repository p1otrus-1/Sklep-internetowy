#include "administrator.h"

using namespace std;

Administrator::Administrator(int id, const string& nazwa)
    : Uzytkownik(id, nazwa) {}
bool Administrator::mozeZarzadzacSklepem() const {
    return true;
}
