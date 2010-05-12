TaKIPI --- tager dla jêzyka polskiego, wersja windowsowa 1.8
Autorzy: Maciej Piasecki, Grzegorz Godlewski, Adam Radziszewski, Bartosz Broda i Adam Wardyñski
Paczkê i instrukcjê przygotowa³ Adam Radziszewski, adam.radziszewski@pwr.wroc.pl

Do przetworzenia tekstu niepoddanego analizie morfologicznej konieczna jest instalacja analizatora morfologicznego zgodnego ze standardem Morfeusza. Morfeusz jest analizatorem morfologicznym autorstwa Marcina Woliñskiego, NIE JEST wolnym oprogramowaniem!
http://nlp.ipipan.waw.pl/~wolinski/morfeusz/morfeusz.html

Tager w wersji windowsowej dostarczany jest w postaci skompilowanej -- w paczce znajduje siê plik uruchamialny takipi.exe oraz niezbêdne biblioteki. ród³a za³¹czone s¹ w katalogu od wersji linuksowej -- za pomoc¹ systemu CMake mo¿na wygenerowaæ projekty Visual Studio do poszczególnych komponentów (choæ bêd¹ one wymaga³y pewnych zmian w konfiguracji, chodzi o lokalizacjê bibliotek na dysku).

UWAGA: jeœli po uruchomieniu takipi.exe pojawi siê lakoniczne okno mówi¹ce, ¿e nie uda³o siê uruchomiæ aplikacji, nale¿y œci¹gn¹æ ze strony Microsoftu i doinstalowaæ "Visual C++ 2005 SP1 Redistributable Package (x86)" (nie podajê tutaj linka, bo adres mo¿e siê zmieniæ -- ³atwiej znaleŸæ przez dowoln¹ wyszukiwarkê internetow¹).


Program mo¿e zawieraæ wiele b³êdów oraz niedoci¹gniêæ. Prosimy o ich zg³aszanie (oraz wszelkich innych uwag).


Plik uruchamialny tagera nosi nazwê takipi.exe. Niestety œcie¿ki do danych tagera zosta³y zakodowane na sztywno w owym programie (jako ¿e w systemie Windows nie ma standardowych katalogów przechowywania danych). W zwi¹zku z tym tager nale¿y uruchamiaæ bêd¹c w katalogu z plikiem takipi.exe (tam te¿ znajduj¹ siê wszystkie potrzebne dane).

takipi -it TXT -i in.txt -o out.xml

Powoduje przetagowanie tekstowego pliku in.txt i wyplucie wyjœcia jako out.xml.
Standardowo pliki wejœciowe traktowane s¹ jako zakodowane w UTF8. Tager mo¿e te¿ obs³u¿yæ kodowanie CP1250 oraz ISO-8859-2 (Latin-2); w tym celu nale¿y u¿yæ prze³¹cznika -CP1250 lub -ISO8859_2 (standardowe kodowanie plików tekstowych w Windowsie to CP1250).
Uwaga: wyjœciowy plik XML zawsze jest kodowany w UTF8

W razie tagowania wiêkszej liczby plików warto przygotowaæ plik zawieraj¹cy po kolei œcie¿ki do tagowanych plików (ka¿da œcie¿ka w nowej linii) i uruchomiæ tager z list¹ korzystaj¹c z prze³¹cznika -is. Skróci to istotnie czas dzia³ania, poniewa¿ dane tagera zostan¹ za³adowane raz przed przetworzeniem wszystkich plików i dopiero po wszystkim zwolnione.

Np.

takipi -it TXT -is filelist.txt -CP1250

Wygeneruje to pliki o nazwach NAZWA_WE.tag bêd¹cych w rzeczywistoœci plkami XML.

Prze³¹cznik -it okreœla typ wejœciowego pliku. Dostêpne opcje:
	TXT -- czysty tekst,
	XML (domyœlne) -- pliki typu pre_morph.xml lub inny XML, tager przetworzy tekst miêdzy tagami XML-owymi (PCDATA) pozostawiaj¹c tagi niezmienone na wyjœciu,
	CORPUS -- plik XML w standardzie XCES, zawieraj¹cy tekst poddany analizie morfologicznej; w tym wypadku tager dodaje znaczniki ujednoznaczniaj¹ce (disamb="1").

Wiêcej szczegó³ów po uruchomieniu tagera bez prze³¹czników (choæ wiêkszoœæ pozosta³ych prze³¹czników dotyczy uczenia b¹dŸ testowania tagera i nie ma znaczenia w przypadku normalnego u¿ytkowania).

Uwaga: standardowo tager generuje równie¿ nowe znaczniki: tnum (litera³ liczbowy), tdate (data), ttime (godzina) turi (adres URL) i tsym (niezidentyfikowany ci¹g liter, liczb, symboli). W celu osi¹gniêcia zgodnoœci ze standardem KIPI mo¿na uruchomiæ tager z prze³¹cznikiem -old. Spowoduje to wy³¹czenie nowych klas fleksyjnych (np. litera³y liczbowe bêd¹ oznaczone jako ign -- zgodnie z Morfeuszem).

Rozwi¹zywanie problemów. Jeœli tager wypluwa wszystkie s³owa oznaczone jako twf, oznacza to, ¿e nie uda³o siê zlokalizowaæ katalogu z danymi. Mo¿na wtedy uruchomiæ:
takipi -datainfo
Spowoduje to wyœwietlenie informacji o tym, gdzie tager szuka swoich danych. W katalogu wyœwietlonym jako "dane tagera" powinien byæ m.in. plik general.rul, natomiast w katalogu "dane projektu Corpus" powinien byæ m.in. plik ETS.bin.


Jeœli jesteœ zainteresowany u¿yciem tagera jako biblioteki ³adowanej dynamicznie, poczytaj plik README w katalogu Linux. Kody Ÿród³owe obu wersji s¹ te same, ró¿nice w funkcjach systemowych s¹ rozwi¹zane za pomoc¹ kompilacji warunkowej.
