#pragma once

#include "uzytkownik.h"

using std::string;

class Administrator : public Uzytkownik {
public:
    Administrator(int id, const string& nazwa);
    bool mozeZarzadzacSklepem() const override;
};
