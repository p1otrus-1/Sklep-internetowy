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
