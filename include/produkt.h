#pragma once

#include <cstdint>
#include <string>

class Produkt {
public:
    Produkt(int id, const std::string& nazwa,
            const std::string& kategoria, std::int64_t cena_grosze,
            int stan_magazynowy);
    int pobierzId() const;
    const std::string& pobierzNazwe() const;
    const std::string& pobierzKategorie() const;
    std::int64_t pobierzCeneGrosze() const;
    int pobierzStanMagazynowy() const;
    bool czyAktywny() const;
    bool czyDostepny(int liczba) const;
    void zmienDane(const std::string& nazwa,
            const std::string& kategoria, std::int64_t cena_grosze);
    void uzupelnijStan(int liczba);
    void zmniejszStan(int liczba);
    void wycofaj();

private:
    int id_;
    std::string nazwa_;
    std::string kategoria_;
    std::int64_t cena_grosze_;
    int stan_magazynowy_;
    bool aktywny_;
};
