#include "administrator.h"
#include "klient.h"
#include "konsola.h"
#include "system_sklepu.h"
#include <exception>
#include <iostream>
#include <memory>

using namespace std;

namespace {
void dodajProduktyPrzykladowe(SystemSklepu& system) {
    const auto& administrator = system.pobierzUzytkownika(3);
    system.dodajProdukt(administrator, "Klawiatura", "Akcesoria", 10000, 10);
    system.dodajProdukt(administrator, "Mysz", "Akcesoria", 4999, 5);
    system.dodajProdukt(administrator, "Monitor", "Monitory", 79900, 3);
    system.dodajProdukt(administrator, "Podkladka", "Akcesoria", 2999, 20);
    system.dodajProdukt(administrator, "Hub USB", "Akcesoria", 8990, 8);
    system.dodajProdukt(administrator, "Monitor 24", "Monitory", 54900, 6);
    system.dodajProdukt(administrator, "Monitor 27", "Monitory", 99900, 4);
    system.dodajProdukt(administrator, "Monitor 32", "Monitory", 149900, 0);
    system.dodajProdukt(administrator, "Sluchawki", "Audio", 14900, 9);
    system.dodajProdukt(administrator, "Glosniki", "Audio", 19900, 7);
    system.dodajProdukt(administrator, "Mikrofon", "Audio", 12900, 6);
    system.dodajProdukt(administrator, "Karta dzwiekowa", "Audio", 7999, 2);
    system.dodajProdukt(administrator, "Dysk SSD 500GB", "Pamiec", 19900, 12);
    system.dodajProdukt(administrator, "Dysk SSD 1TB", "Pamiec", 32900, 8);
    system.dodajProdukt(administrator, "Pendrive 64GB", "Pamiec", 3999, 25);
    system.dodajProdukt(administrator, "Karta SD 128GB", "Pamiec", 6999, 15);
    system.dodajProdukt(administrator, "Router", "Siec", 24900, 5);
    system.dodajProdukt(administrator, "Switch", "Siec", 11900, 7);
    system.dodajProdukt(administrator, "Kabel Ethernet", "Siec", 1999, 30);
    system.dodajProdukt(administrator, "Adapter WiFi", "Siec", 5999, 0);
}
}  // namespace

int main() {
    try {
        SystemSklepu system;
        system.dodajUzytkownika(make_unique<Klient>(1, "Anna"));
        system.dodajUzytkownika(make_unique<Klient>(2, "Jan"));
        system.dodajUzytkownika(make_unique<Administrator>(3, "Administrator"));
        dodajProduktyPrzykladowe(system);
        Konsola konsola(system);
        konsola.uruchom();
    } catch (const exception& blad) {
        cerr << "Blad uruchomienia: " << blad.what() << '\n';
        return 1;
    }
    return 0;
}
