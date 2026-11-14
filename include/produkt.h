#pragma once

#include <cstdint>
#include <string>

using std::int64_t;
using std::string;

class Produkt {
public:
    Produkt(int id, const string& nazwa, const string& kategoria,
            int64_t cena_grosze, int stan_magazynowy);
    int pobierzId() const;
    const string& pobierzNazwe() const;
    const string& pobierzKategorie() const;
    int64_t pobierzCeneGrosze() const;
    int pobierzStanMagazynowy() const;
    bool czyAktywny() const;
    bool czyDostepny(int liczba) const;
    void zmienDane(const string& nazwa, const string& kategoria,
                   int64_t cena_grosze);
    void uzupelnijStan(int liczba);
    void zmniejszStan(int liczba);
    void wycofaj();

private:
    int id_;
    string nazwa_;
    string kategoria_;
    int64_t cena_grosze_;
    int stan_magazynowy_;
    bool aktywny_;
};
