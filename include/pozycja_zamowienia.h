#pragma once

#include <cstdint>
#include <string>

using std::int64_t;
using std::string;

class PozycjaZamowienia {
public:
    PozycjaZamowienia(int id_produktu, const string& nazwa_produktu,
                      int64_t cena_jednostkowa_grosze, int liczba);
    int pobierzIdProduktu() const;
    const string& pobierzNazweProduktu() const;
    int64_t pobierzCeneJednostkowaGrosze() const;
    int pobierzLiczbe() const;
    int64_t obliczWartosc() const;

private:
    int id_produktu_;
    string nazwa_produktu_;
    int64_t cena_jednostkowa_grosze_;
    int liczba_;
};
