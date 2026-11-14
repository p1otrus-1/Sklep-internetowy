# UML 2 - Sklep Internetowy

## Lista przypadków użycia

### PU01 - Wyszukaj produkty
Aktorzy: Klient, Administrator. Powiązanie: US01.

Użytkownik podaje nazwę lub kategorię. System zwraca pasujące aktywne produkty z ceną i stanem magazynowym; brak wyników daje pustą listę.

Metody: wyszukajProdukty.

### PU02 - Dodaj do koszyka
Aktor: Klient. Powiązanie: US02.

Klient wskazuje produkt i liczbę sztuk. System dodaje pozycję lub zwiększa liczbę sztuk, po sprawdzeniu poprawności danych i dostępności.

Metody: dodajDoKoszyka.

### PU03 - Przeglądaj i edytuj koszyk
Aktor: Klient. Powiązanie: US03.

Klient przegląda pozycje i łączną wartość koszyka, zmienia liczbę sztuk lub usuwa pozycję. System aktualizuje zawartość i wartość koszyka.

Metody: pobierzKoszyk; obliczWartoscKoszyka; zmienLiczbeWKoszyku; usunZKoszyka.

### PU04 - Złóż zamówienie
Aktor: Klient. Powiązanie: US04.

System sprawdza koszyk i dostępność wszystkich produktów. Tworzy zamówienie ze statusem Nowe i cenami z chwili zakupu, zmniejsza stan i czyści koszyk. Błąd walidacji pozostawia dane bez zmian.

Metody: zlozZamowienie.

### PU05 - Opłać zamówienie
Aktor: Klient. Powiązanie: US05.

Klient symuluje płatność za własne zamówienie. System oznacza płatność jako opłaconą. Ponowna płatność i próba opłacenia cudzego zamówienia są odrzucane.

Metody: oplacZamowienie.

### PU06 - Zarządzanie katalogiem
Aktor: Administrator. Powiązanie: US06.

Administrator zarządza ofertą sklepu, w szczególności wprowadza nowe produkty, podając nazwę, kategorię, cenę i stan magazynowy. Po sprawdzeniu danych system dodaje produkt do katalogu. Edycja danych, uzupełnianie stanu i wycofywanie produktu są dodatkowo przedstawione na diagramie jako osobne przypadki użycia PU07-PU09.

Metody: dodajProdukt.

### PU07 - Edytuj dane produktu
Aktor: Administrator. Powiązanie: US06.

Administrator zmienia nazwę, kategorię lub cenę istniejącego produktu. System sprawdza dane i zapisuje zmiany w katalogu.

Metody: zmienProdukt.

### PU08 - Uzupełnij stan produktu
Aktor: Administrator. Powiązanie: US06.

Administrator wskazuje produkt i dodatnią liczbę sztuk. System zwiększa stan magazynowy produktu.

Metody: uzupelnijStanProduktu.

### PU09 - Wycofaj produkt
Aktor: Administrator. Powiązanie: US06.

Administrator wycofuje produkt z oferty. System blokuje jego dalszy zakup, zachowując zapis wcześniejszych zamówień.

Metody: wycofajProdukt.

### PU10 - Zmień status zamówienia
Aktor: Administrator. Powiązanie: US07.

Administrator zmienia status w kolejności: Nowe, W realizacji, Wysłane, Dostarczone. Rozpoczęcie realizacji wymaga płatności; pomijanie i cofanie etapów jest odrzucane.

Metody: zmienStatusZamowienia.

## Relacje include/extend

Na zatwierdzonym diagramie nie występują relacje «include» ani «extend». Linie ciągłe łączą aktorów z dostępnymi dla nich przypadkami użycia. Administrator, podobnie jak Klient, ma dostęp do wyszukiwania produktów. Zarządzanie katalogiem i trzy szczegółowe operacje administracyjne wynikają z US06; na diagramie nie połączono ich relacjami «include» ani «extend».

Zgodnie z adnotacją na diagramie wybrane przypadki użycia wymagają wcześniejszego zalogowania. Logowanie jest traktowane jako warunek wstępny tych operacji; nie przedstawiono go jako osobnego przypadku użycia ani obowiązkowego wywołania «include». Diagram nie określa szczegółowego mechanizmu uwierzytelniania.

## Przebieg komunikatów na diagramie sekwencji - Złóż zamówienie (PU04 / US04)

Diagram przedstawia składanie zamówienia z uwzględnieniem poprawnego wykonania oraz odrzucenia operacji. Uczestniczą w nim obiekty `klient : Klient`, `system : SystemSklepu`, `koszyk : Koszyk`, `produkt : Produkt` oraz tworzony obiekt `zamowienie : Zamówienie`.

1. **Rozpoczęcie operacji.** Obiekt Klient wywołuje `zlozZamowienie(id_klienta)` na obiekcie SystemSklepu. Rozpoczyna się aktywacja systemu, obejmująca obsługę żądania.
2. **Pobranie koszyka.** System wywołuje na Kliencie `pobierzKoszyk()` i otrzymuje referencję do koszyka (`Koszyk&`). Następnie wywołuje na Koszyku `czyPusty()`, które zwraca wartość `pusty` typu `bool`.
3. **Sprawdzenie pustego koszyka - fragment `alt`.** Dla warunku `[koszyk pusty]` system zgłasza wyjątek i kończy operację bez utworzenia zamówienia. Dla warunku `[koszyk niepusty]` wywołuje `pobierzPozycje()` i otrzymuje referencję do kolekcji pozycji (`const vector<PozycjaKoszyka>&`).
4. **Weryfikacja produktów - fragment `loop`.** Dla każdej pozycji koszyka system wywołuje na odpowiednim Produkcie `czyDostepny(liczba)`, `pobierzNazwe()` i `pobierzCeneGrosze()`. Otrzymuje kolejno dostępność typu `bool`, nazwę typu `const string&` i cenę w groszach typu `int64`. Linia życia Produkt reprezentuje kolejne produkty przetwarzane w pętli. Na tym etapie stan magazynowy nie jest zmniejszany.
5. **Utworzenie zamówienia - fragment `alt`.** Gdy wszystkie pozycje są poprawne i dostępne, system tworzy obiekt przez komunikat `«create» Zamówienie(id, id_klienta, pozycje)`. W tym miejscu rozpoczyna się linia życia obiektu `zamowienie : Zamówienie`. Pozycje zamówienia zachowują nazwę i cenę produktu z chwili zakupu.
6. **Zmiana stanów i zakończenie.** W drugiej pętli `loop`, dla każdej zamówionej pozycji, system wywołuje `zmniejszStan(liczba)` na Produkcie. Następnie wywołuje `wyczysc()` na Koszyku i zwraca Klientowi `id_zamowienia` typu `int`.
7. **Odrzucenie zamówienia.** Alternatywna gałąź `[produkt niedostępny lub dane niepoprawne]` kończy się zgłoszeniem wyjątku. Przy błędzie walidacji koszyk i stany magazynowe pozostają bez zmian, ponieważ operacje zmieniające dane znajdują się wyłącznie w gałęzi poprawnego wykonania.

Przerywane strzałki odpowiedzi pokazują wartości zwracane przez metody. Komunikaty opisane jako wyjątki oznaczają zgłoszenie błędu, a nie zwrot wartości `bool`. Metody `zmniejszStan(liczba)` i `wyczysc()` mają typ `void`, dlatego nie pokazano dla nich wartości zwrotnych. Konstruktor również nie zwraca wartości. Prostokąty aktywacji oznaczają wykonywanie operacji przez obiekty.


## Uzupełnienie implementacyjne - wersja końcowa

Kod z przesłanego ZIP-a używa nazw ASCII: `Uzytkownik`, `Zamowienie`,
`PozycjaZamowienia`, `Platnosc`. Odpowiadają one klasom zapisanym z polskimi
znakami na zatwierdzonych diagramach. Istotne operacje sekwencji zachowano.

Dodano warstwę terminalową `Konsola`. Jej pole `system_ : SystemSklepu&`
stanowi asocjację: jedna konsola korzysta z jednego systemu, system może
istnieć bez konsoli lub być współdzielony przez kilka konsol. Ta klasa odpowiada wyłącznie za wejście i wyjście.
`main.cpp` przygotowuje dane i uruchamia konsolę, bez logiki biznesowej.

Doprecyzowania do diagramu klas (publiczne operacje SystemSklepu):

- `oplacZamowienie(id_klienta: int, id_zamowienia: int): void` - istniejąca
  deklaracja z nagłówka, teraz zaimplementowana; realizuje US05.
- `pobierzProdukt(id_produktu: int): const Produkt&` - odczyt danych do koszyka.
- `pobierzProdukty(uzytkownik: const Uzytkownik&): const vector<Produkt>&`
  - administrator odczytuje pełny katalog, także produkty wycofane.
- `pobierzZamowienia(uzytkownik: const Uzytkownik&): vector<Zamowienie>`
  - klient widzi swoje zamówienia, administrator wszystkie.

Prywatne metody `znajdzProdukt`, `znajdzZamowienie` i
`sprawdzAdministratora` służą wspólnemu wyszukiwaniu oraz sprawdzaniu roli.
Nie tworzą nowych przypadków użycia.

Wybór przykładowego użytkownika w konsoli symuluje spełniony warunek
wcześniejszego zalogowania; nie jest implementacją bezpiecznego logowania.
Uprawnienia operacji administracyjnych i właściciel płatności są sprawdzane
w SystemSklepu. Dane żyją wyłącznie w pamięci jednej instancji programu.

Aktualny pełny model implementacji znajduje się w
`diagram_klas_aktualny.png` oraz edytowalnym `diagram_klas_aktualny.puml`.
Zatwierdzony `diagram klas.pdf` pozostawiono jako wersję z wcześniejszych
zajęć. W aktualnym modelu zaznaczono również własność kolekcji SystemSklepu
jako kompozycję, zgodnie z `unique_ptr` i przechowywaniem obiektów przez wartość.
Przy wymaganym oddaniu w Visual Paradigm należy odwzorować te uzupełnienia
w projekcie tego narzędzia; plik PlantUML nie jest plikiem Visual Paradigm.


### Usprawnienia konsoli i wyszukiwania

Katalog początkowy zawiera 20 stałych produktów w 5 kategoriach.
Konsola wyświetla dynamiczną listę kategorii i opcjonalnie przyjmuje
fragment nazwy. Opcja „Wyszukiwanie” tylko wyświetla wyniki.
Opcja „Dodaj do koszyka” dodatkowo pozwala wskazać produkt i liczbę sztuk.
Nie dodano nowej klasy kategorii: jej nazwę nadal przechowuje Produkt.
SystemSklepu::wyszukajProdukty ignoruje wielkość liter ASCII w obu filtrach.
Konsola zawęża wybraną z listy kategorię do dokładnej nazwy oraz sprawdza,
czy wybrane ID należy do pokazanych wyników. Dostępność i ilość nadal
weryfikuje SystemSklepu. Administrator używa wyszukiwania do przeglądania,
a operacje katalogowe ma w osobnym menu. To usprawnienia PU01 i PU02;
przebieg składania zamówienia i reguły pozostałych przypadków nie zmieniły się.

Konsola otrzymała prywatne metody pokazNaglowek, wybierzKategorie,
dodajZListy oraz parametry uzytkownik i dodawanie w wyszukaj.
Parametr dodawanie ma wartość false dla wyszukiwania i true dla
dodawania do koszyka. Aktualna sygnatura to
`wyszukaj(const Uzytkownik& uzytkownik, bool dodawanie)`.
Aktualny diagram klas uwzględnia te sygnatury. Wejście jest liniowe (Enter), bez opóźnień i
czyszczenia terminala; dane i płatności pozostają demonstracyjne.


### Listy przed wyborem obiektu

Wyszukiwanie i dodawanie do koszyka współdzielą wybór kategorii,
filtrowanie nazwy i wyświetlanie wyników. Wyszukiwanie kończy się
na prezentacji wyników, natomiast dodawanie pozwala następnie wybrać
produkt i liczbę sztuk. Pozostają osobnymi przypadkami użycia.
Wspólny kod obsługi filtrów nie wymaga dopisywania relacji include/extend
do zatwierdzonego diagramu.

Konsola pokazuje koszyk przed zmianą ilości i usuwaniem, własne zamówienia
przed płatnością oraz wszystkie zamówienia administratorowi przed zmianą
statusu. Operacje katalogowe administratora także pokazują produkty przed
pytaniem o ID. Puste listy i wybór 0 kończą operację bez zmiany danych.
Metody pomocnicze wybierzPozycjeKoszyka, wybierzZamowienie, wybierzProdukt
oraz edytujKoszyk organizują interakcję; reguły biznesowe pozostają w klasach.

Kolejność statusów z US07 zachowano. Osobne komunikaty dotyczą braku płatności,
wyboru obecnego statusu oraz cofania lub pomijania etapów. Usunięto dopisek
„sesja demonstracyjna bez haseł” z nagłówka wyboru użytkownika, ale program
nadal nie uwierzytelnia użytkowników hasłem. Zmiana napisów tego nie zmienia.
