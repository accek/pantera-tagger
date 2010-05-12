TaKIPI --- tager dla j�zyka polskiego, wersja windowsowa 1.8
Autorzy: Maciej Piasecki, Grzegorz Godlewski, Adam Radziszewski, Bartosz Broda i Adam Wardy�ski
Paczk� i instrukcj� przygotowa� Adam Radziszewski, adam.radziszewski@pwr.wroc.pl

Do przetworzenia tekstu niepoddanego analizie morfologicznej konieczna jest instalacja analizatora morfologicznego zgodnego ze standardem Morfeusza. Morfeusz jest analizatorem morfologicznym autorstwa Marcina Woli�skiego, NIE JEST wolnym oprogramowaniem!
http://nlp.ipipan.waw.pl/~wolinski/morfeusz/morfeusz.html

Tager w wersji windowsowej dostarczany jest w postaci skompilowanej -- w paczce znajduje si� plik uruchamialny takipi.exe oraz niezb�dne biblioteki. �r�d�a za��czone s� w katalogu od wersji linuksowej -- za pomoc� systemu CMake mo�na wygenerowa� projekty Visual Studio do poszczeg�lnych komponent�w (cho� b�d� one wymaga�y pewnych zmian w konfiguracji, chodzi o lokalizacj� bibliotek na dysku).

UWAGA: je�li po uruchomieniu takipi.exe pojawi si� lakoniczne okno m�wi�ce, �e nie uda�o si� uruchomi� aplikacji, nale�y �ci�gn�� ze strony Microsoftu i doinstalowa� "Visual C++ 2005 SP1 Redistributable Package (x86)" (nie podaj� tutaj linka, bo adres mo�e si� zmieni� -- �atwiej znale�� przez dowoln� wyszukiwark� internetow�).


Program mo�e zawiera� wiele b��d�w oraz niedoci�gni��. Prosimy o ich zg�aszanie (oraz wszelkich innych uwag).


Plik uruchamialny tagera nosi nazw� takipi.exe. Niestety �cie�ki do danych tagera zosta�y zakodowane na sztywno w owym programie (jako �e w systemie Windows nie ma standardowych katalog�w przechowywania danych). W zwi�zku z tym tager nale�y uruchamia� b�d�c w katalogu z plikiem takipi.exe (tam te� znajduj� si� wszystkie potrzebne dane).

takipi -it TXT -i in.txt -o out.xml

Powoduje przetagowanie tekstowego pliku in.txt i wyplucie wyj�cia jako out.xml.
Standardowo pliki wej�ciowe traktowane s� jako zakodowane w UTF8. Tager mo�e te� obs�u�y� kodowanie CP1250 oraz ISO-8859-2 (Latin-2); w tym celu nale�y u�y� prze��cznika -CP1250 lub -ISO8859_2 (standardowe kodowanie plik�w tekstowych w Windowsie to CP1250).
Uwaga: wyj�ciowy plik XML zawsze jest kodowany w UTF8

W razie tagowania wi�kszej liczby plik�w warto przygotowa� plik zawieraj�cy po kolei �cie�ki do tagowanych plik�w (ka�da �cie�ka w nowej linii) i uruchomi� tager z list� korzystaj�c z prze��cznika -is. Skr�ci to istotnie czas dzia�ania, poniewa� dane tagera zostan� za�adowane raz przed przetworzeniem wszystkich plik�w i dopiero po wszystkim zwolnione.

Np.

takipi -it TXT -is filelist.txt -CP1250

Wygeneruje to pliki o nazwach NAZWA_WE.tag b�d�cych w rzeczywisto�ci plkami XML.

Prze��cznik -it okre�la typ wej�ciowego pliku. Dost�pne opcje:
	TXT -- czysty tekst,
	XML (domy�lne) -- pliki typu pre_morph.xml lub inny XML, tager przetworzy tekst mi�dzy tagami XML-owymi (PCDATA) pozostawiaj�c tagi niezmienone na wyj�ciu,
	CORPUS -- plik XML w standardzie XCES, zawieraj�cy tekst poddany analizie morfologicznej; w tym wypadku tager dodaje znaczniki ujednoznaczniaj�ce (disamb="1").

Wi�cej szczeg��w po uruchomieniu tagera bez prze��cznik�w (cho� wi�kszo�� pozosta�ych prze��cznik�w dotyczy uczenia b�d� testowania tagera i nie ma znaczenia w przypadku normalnego u�ytkowania).

Uwaga: standardowo tager generuje r�wnie� nowe znaczniki: tnum (litera� liczbowy), tdate (data), ttime (godzina) turi (adres URL) i tsym (niezidentyfikowany ci�g liter, liczb, symboli). W celu osi�gni�cia zgodno�ci ze standardem KIPI mo�na uruchomi� tager z prze��cznikiem -old. Spowoduje to wy��czenie nowych klas fleksyjnych (np. litera�y liczbowe b�d� oznaczone jako ign -- zgodnie z Morfeuszem).

Rozwi�zywanie problem�w. Je�li tager wypluwa wszystkie s�owa oznaczone jako twf, oznacza to, �e nie uda�o si� zlokalizowa� katalogu z danymi. Mo�na wtedy uruchomi�:
takipi -datainfo
Spowoduje to wy�wietlenie informacji o tym, gdzie tager szuka swoich danych. W katalogu wy�wietlonym jako "dane tagera" powinien by� m.in. plik general.rul, natomiast w katalogu "dane projektu Corpus" powinien by� m.in. plik ETS.bin.


Je�li jeste� zainteresowany u�yciem tagera jako biblioteki �adowanej dynamicznie, poczytaj plik README w katalogu Linux. Kody �r�d�owe obu wersji s� te same, r�nice w funkcjach systemowych s� rozwi�zane za pomoc� kompilacji warunkowej.
