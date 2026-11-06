#include <cstdint>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

class Produkt {
private:
    int id_;
    std::string nazwa_;
    std::int64_t cena_grosze_;
    int stan_magazynowy_;

public:
    Produkt(int id, std::string nazwa,
            std::int64_t cena_grosze, int stan_magazynowy)
        : id_(id),
          nazwa_(nazwa),
          cena_grosze_(cena_grosze),
          stan_magazynowy_(stan_magazynowy) {}

    int pobierzId() const {
        return id_;
    }

    const std::string& pobierzNazwe() const {
        return nazwa_;
    }

    std::int64_t pobierzCeneGrosze() const {
        return cena_grosze_;
    }

    bool czyDostepny(int liczba) const {
        return liczba > 0 && liczba <= stan_magazynowy_;
    }
};

class PozycjaKoszyka {
private:
    int id_produktu_;
    int liczba_;

public:
    PozycjaKoszyka(int id_produktu, int liczba)
        : id_produktu_(id_produktu), liczba_(liczba) {}

    int pobierzIdProduktu() const {
        return id_produktu_;
    }

    int pobierzLiczbe() const {
        return liczba_;
    }

    void zwiekszLiczbe(int liczba) {
        liczba_ += liczba;
    }
};

class Koszyk {
private:
    std::vector<PozycjaKoszyka> pozycje_;

public:
    void dodajPozycje(int id_produktu, int liczba) {
        if (liczba <= 0) {
            throw std::invalid_argument("Liczba sztuk musi byc dodatnia.");
        }

        for (auto& pozycja : pozycje_) {
            if (pozycja.pobierzIdProduktu() == id_produktu) {
                pozycja.zwiekszLiczbe(liczba);
                return;
            }
        }

        pozycje_.emplace_back(id_produktu, liczba);
    }

    const std::vector<PozycjaKoszyka>& pobierzPozycje() const {
        return pozycje_;
    }

    std::int64_t obliczWartosc(
            const std::vector<Produkt>& produkty) const {
        std::int64_t suma = 0;

        for (const auto& pozycja : pozycje_) {
            for (const auto& produkt : produkty) {
                if (produkt.pobierzId() == pozycja.pobierzIdProduktu()) {
                    suma += produkt.pobierzCeneGrosze()
                            * pozycja.pobierzLiczbe();
                    break;
                }
            }
        }

        return suma;
    }
};

// Pomocnicza funkcja do prezentowania nieujemnych kwot.
void wyswietlKwote(std::int64_t grosze) {
    std::cout << grosze / 100 << ','
              << std::setfill('0') << std::setw(2) << grosze % 100
              << std::setfill(' ') << " zl";
}

int main() {
    std::vector<Produkt> produkty = {
        Produkt(1, "Klawiatura", 10000, 10),
        Produkt(2, "Mysz", 4999, 5)
    };

    Koszyk koszyk;

    // Na razie ta funkcja pelni role operacji SystemSklepu.
    auto dodajDoKoszyka = [&](int id_produktu, int liczba) {
        for (const auto& produkt : produkty) {
            if (produkt.pobierzId() != id_produktu) {
                continue;
            }

            int juz_w_koszyku = 0;

            for (const auto& pozycja : koszyk.pobierzPozycje()) {
                if (pozycja.pobierzIdProduktu() == id_produktu) {
                    juz_w_koszyku = pozycja.pobierzLiczbe();
                    break;
                }
            }

            if (liczba <= 0 ||
                !produkt.czyDostepny(juz_w_koszyku + liczba)) {
                std::cout << "Odrzucono: niepoprawna liczba lub brak towaru.\n";
                return;
            }

            koszyk.dodajPozycje(id_produktu, liczba);
            std::cout << "Dodano: " << produkt.pobierzNazwe()
                      << " x " << liczba << '\n';
            return;
        }

        std::cout << "Nie znaleziono produktu.\n";
    };

    dodajDoKoszyka(1, 1);
    dodajDoKoszyka(1, 1);
    dodajDoKoszyka(2, 1);
    dodajDoKoszyka(2, 10);

    std::cout << "\nKoszyk:\n";

    for (const auto& pozycja : koszyk.pobierzPozycje()) {
        for (const auto& produkt : produkty) {
            if (produkt.pobierzId() == pozycja.pobierzIdProduktu()) {
                std::cout << produkt.pobierzNazwe()
                          << " x " << pozycja.pobierzLiczbe()
                          << " = ";

                wyswietlKwote(
                    produkt.pobierzCeneGrosze() * pozycja.pobierzLiczbe()
                );

                std::cout << '\n';
                break;
            }
        }
    }

    std::cout << "Razem: ";
    wyswietlKwote(koszyk.obliczWartosc(produkty));
    std::cout << '\n';
}