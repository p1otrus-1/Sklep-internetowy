#pragma once

#include "uzytkownik.h"

class Administrator : public Użytkownik {
public:
    Administrator(int id, const std::string& nazwa);
    bool mozeZarzadzacSklepem() const override;
};
