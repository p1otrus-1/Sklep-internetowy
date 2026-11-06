#pragma once

#include "uzytkownik.h"
#include "koszyk.h"

class Klient : public Uzytkownik {
public:
    Klient(int id, const std::string& nazwa);
    bool mozeZarzadzacSklepem() const override;
    Koszyk& pobierzKoszyk();
    const Koszyk& pobierzKoszyk() const;

private:
    Koszyk koszyk_;
};
