# Sklep internetowy - C++17

Konsolowy projekt OOP realizujący US01-US07 z `logic/user stories.txt`.
Dane są w pamięci i znikają po zamknięciu programu. Na starcie jest 20 stałych produktów w 5 kategoriach
i użytkownicy: Anna (1), Jan (2), Administrator (3).
Wybór użytkownika symuluje sesję, nie uwierzytelnia hasłem.
Płatność to symulacja, bez przesyłania pieniędzy.

## Uruchomienie

Otwórz w VS Code cały katalog projektu, wybierz Ctrl+Shift+B.
Zadanie kompiluje wszystkie pliki `src/*.cpp`, niezależnie od otwartego pliku.
Linux korzysta z `g++` w PATH. Windows używa `C:\MinGW\bin\g++.exe`;
jeżeli kompilator jest gdzie indziej, popraw `windows.command` w tasks.json.
Uruchom w terminalu `./sklep` (Linux) lub `.\sklep.exe` (PowerShell).

Alternatywnie, w terminalu Linux, z katalogu projektu:

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -I include src/*.cpp -o sklep
./sklep
```

## Co można zrobić

Klient: katalog, wyszukiwanie, dodawanie do koszyka, zmiana liczby sztuk,
usuwanie pozycji, składanie zamówień, przegląd własnych zamówień i płatność.
Administrator: katalog także z wycofanymi produktami, dodawanie i edycja
produktów, uzupełnianie magazynu, wycofanie produktu, przegląd wszystkich
zamówień i zmiana statusu.

W wyszukiwaniu wybierz numer kategorii (0 = wszystkie), a potem opcjonalny
fragment nazwy (Enter = wszystkie nazwy). Kategorie są pobierane z aktywnego
katalogu i sortowane alfabetycznie. Nowa kategoria dodana przez administratora
automatycznie pojawia się na liście. Wybór kategorii jest dokładny, a nazwa
jest wyszukiwana jako fragment bez rozróżniania wielkości liter ASCII (A-Z).
Polskie litery nie są normalizowane.

Opcja 2 „Wyszukiwanie” kończy się wyświetleniem wyników.
Opcja 3 „Dodaj do koszyka” korzysta z tych samych filtrów,
a następnie pozwala wybrać ID produktu i liczbę sztuk.
Przy wyborze produktu 0 oznacza powrót do menu.

Nie trzeba pamiętać ID z innych ekranów: przed zmianą ilości lub usunięciem
pozycji pojawia się koszyk, przed płatnością własne zamówienia, a przed zmianą
statusu administrator widzi wszystkie zamówienia. Edycja, uzupełnienie stanu
i wycofanie produktu również zaczynają się od listy produktów.
Pusta lista powoduje powrót bez pytania o ID. Przy wyborze ID 0 oznacza powrót;
nie wyświetla wtedy komunikatu sukcesu. Niepoprawne ID jest odrzucane przed
pytaniem o kolejne dane.
Numery kategorii są pozycjami na liście, a ID produktów pozostają stałe.
Kategorie o różnych zapisach, np. Audio i audio, są osobnymi kategoriami.
Metoda SystemSklepu::wyszukajProdukty nadal obsługuje dwa filtry fragmentowe
połączone przez AND; Konsola dodatkowo zawęża wybraną kategorię dokładnie.

Wszystkie wpisy zatwierdzamy Enterem. Nie ma opóźnień ani czyszczenia ekranu.
Separatory oddzielają wyniki od kolejnego menu. Dwa produkty startują ze
stanem 0, co pozwala sprawdzić odrzucenie zakupu niedostępnego towaru. Ceny wpisuje się w groszach,
np. `4999` oznacza 49,99 zł. Koszyk nie rezerwuje magazynu.
Wycofanie produktu jest trwałe w danej sesji; uzupełnienie stanu go nie przywraca.

## Sprawdzenie nowej obsługi katalogu

1. Anna -> Wyszukiwanie -> 2. Audio -> wpisz `SLUCH`.
   Powinny pojawić się słuchawki, bez pytania o dodanie do koszyka.
2. Anna -> Dodaj do koszyka -> 2. Audio -> wpisz `SLUCH`.
   Wybierz ID 9 i 2 sztuki. W koszyku powinno być 298,00 zł.
3. Wyszukaj nieistniejącą nazwę - pojawi się pusta lista.
4. W opcji „Dodaj do koszyka” wybierz ID spoza wyników.
   Operacja powinna zostać odrzucona.
5. W opcji „Dodaj do koszyka” wybierz kategorię Monitory,
   pozostaw nazwę pustą i spróbuj dodać produkt 8.
   Zakup powinien zostać odrzucony z powodu zerowego stanu.
6. Administrator -> Dodaj produkt -> podaj nową kategorię, np. Video.
   Ponowne wyszukiwanie powinno pokazać ją na liście kategorii.

## Szybka prezentacja

1. Anna: katalog -> dodaj produkt 1 dwa razy -> pokaż koszyk.
2. Spróbuj dodać 100 sztuk. Operacja zostanie odrzucona bez zmiany koszyka.
3. Złóż zamówienie. Koszyk jest pusty, magazyn mniejszy.
4. Administrator: zmień cenę produktu. Kwota starego zamówienia się nie zmieni.
5. Spróbuj rozpocząć realizację przed płatnością - system odmówi.
6. Jan: otwórz płatność - zamówienie Anny nie pojawi się na jego liście.
   Odrzucenie próby opłacenia cudzego zamówienia bezpośrednio przez
   SystemSklepu sprawdzają testy.
7. Anna: opłać zamówienie. Druga płatność zostanie odrzucona.
8. Administrator: W realizacji -> Wysłane -> Dostarczone.
   Pominięcie lub cofnięcie etapu jest odrzucane.

## Jak czytać kod

- `main.cpp`: tworzy system, przykładowych użytkowników i produkty.
- `Konsola`: wejście, menu, wyświetlanie i wywołania systemu.
- `SystemSklepu`: kolekcje obiektów, uprawnienia, koordynacja operacji.
- `Uzytkownik`: abstrakcyjna baza; `Klient` i `Administrator` określają rolę.
- `Klient` posiada `Koszyk`, a koszyk posiada swoje `PozycjaKoszyka`.
- `Produkt`: cena, dostępność, stan i wycofanie.
- `Zamowienie`: własne pozycje ze stałymi cenami, płatność i kolejność statusów.
- `PozycjaZamowienia`: kopia nazwy i ceny z chwili złożenia zamówienia.
- `Platnosc`: kwota i jednorazowe potwierdzenie.

Przy składaniu zamówienia najpierw sprawdzane są wszystkie produkty.
Dopiero po utworzeniu zamówienia zmniejsza się magazyn i czyści koszyk.
Ceny są liczbami całkowitymi, aby uniknąć błędów obliczeń zmiennoprzecinkowych.
`unique_ptr` jest właścicielem użytkownika; wskaźnik z `dynamic_cast` tylko
sprawdza typ i niczego nie usuwa. Wirtualne `mozeZarzadzacSklepem()`
wywoływane przez referencję do `Uzytkownik` demonstruje polimorfizm.

Aktualny diagram klas: `logic/diagram_klas_aktualny.png` (źródło `.puml`).
Uzasadnienia zmian: końcowa sekcja istniejącego `logic/sprawozdanie.md`.

## Elementy OOP i styl

Pola klas są prywatne, a publiczne metody stanowią ich interfejs.
Uzytkownik jest abstrakcyjny, ponieważ mozeZarzadzacSklepem jest czysto
wirtualna (= 0). Klient i Administrator nadpisują ją przez override.
Wirtualny destruktor umożliwia poprawne usuwanie przez wskaźnik do bazy.
Przeciążenia pobierzKoszyk() i pobierzKlienta() mają wersję const i wersję
pozwalającą modyfikować obiekt. To inny mechanizm niż override.
Nie przeciążamy własnych operatorów: nie wymagają tego dostarczone PDF-y.
Nie stosujemy shared_ptr, bo użytkownik ma jednego właściciela - SystemSklepu.
Wskaźnik uzyskany przez dynamic_cast nie posiada obiektu; nie wywołujemy delete.
Referencje SystemSklepu& w konsoli i inicjalizacji celowo umożliwiają zmiany
systemu; nie są parametrami wyłącznie do odczytu.

W plikach .cpp używamy using namespace std; dla krótszego zapisu.
Nagłówki importują tylko używane nazwy, np. using std::string; oraz
using std::vector;. Nie wprowadzają całej przestrzeni std do plików,
które je dołączają. Sam zapis std:: jest poprawny - skrócenie jest wyborem stylu.

## Testy

W VS Code: Terminal -> Run Build Task -> Kompiluj testy.
Uruchom `./testy` lub `.\testy.exe`. Testy używają standardowego `assert`;
nie kompiluj ich z `-DNDEBUG`.

Sprawdzają wszystkie US, wyszukiwanie bez rozróżniania wielkości liter ASCII, brak częściowych zmian przy nieudanym zamówieniu,
uprawnienia, cudzą i podwójną płatność, nieprawidłowe statusy i ich komunikaty, stałość cen,
wycofanie produktu oraz błędne i zbyt duże liczby. Bez dodatkowych bibliotek.

## Włączenie do repozytorium

ZIP nie zawiera `.git` ani programów wynikowych. Skopiuj jego zawartość do
swojego repozytorium po sprawdzeniu lokalnych zmian; usuń stary `demo.cpp`.
Zostaw istniejący katalog `.git`. Sprawdź `git diff` przed commitem.
Wersja bazuje na przesłanym ZIP-ie, nie pobiera późniejszych zmian z GitHuba.
