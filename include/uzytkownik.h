#pragma once

#include <string>

class Uzytkownik {
    public:
        Uzytkownik(int id, const std::string& nazwa);
        virtual ~Uzytkownik();
        int pobierzId() const;
        const std::string& pobierzNazwe() const;
        virtual bool mozeZarzadzacSklepem() const = 0;
    
    private:
        int id_;
        std::string nazwa_;
};
