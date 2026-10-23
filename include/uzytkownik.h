#pragma once

#include <string>

class Użytkownik {
public:
    Użytkownik(int id, const std::string& nazwa);
    virtual ~Użytkownik();
    int pobierzId() const;
    const std::string& pobierzNazwe() const;
    virtual bool mozeZarzadzacSklepem() const = 0;

private:
    int id_;
    std::string nazwa_;
};
