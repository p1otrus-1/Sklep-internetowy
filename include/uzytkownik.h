#pragma once

#include <string>

using std::string;

class Uzytkownik {
public:
    Uzytkownik(int id, const string& nazwa);
    virtual ~Uzytkownik();
    int pobierzId() const;
    const string& pobierzNazwe() const;
    virtual bool mozeZarzadzacSklepem() const = 0;

private:
    int id_;
    string nazwa_;
};
