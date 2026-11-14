#pragma once

#include "uzytkownik.h"
#include "koszyk.h"

using std::string;

class Klient : public Uzytkownik {
public:
    Klient(int id, const string& nazwa);
    bool mozeZarzadzacSklepem() const override;
    Koszyk& pobierzKoszyk();
    const Koszyk& pobierzKoszyk() const;

private:
    Koszyk koszyk_;
};
