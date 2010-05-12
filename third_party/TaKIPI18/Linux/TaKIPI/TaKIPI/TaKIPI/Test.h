/*  TaKIPI 1.5
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Adam Radziszewski, Maciej Piasecki,
 *  Bartosz Broda, Adam Wardyński
 *
 *  Kod na licencji GPL.
 */


#ifndef TAKIPI_TEST_H_
#define TAKIPI_TEST_H_



#include "../takipi_headers.h"

#include "Layers.h"
// #define WITH_COLLOCATIONS -- dla kodu Piotra Kukly

namespace TaKIPI
{
	class TAKIPI_DEV_API CTest
	{
	public:
		CTest(TaKIPI::CLayers& layers);
		~CTest(void);

		/** Testuje dane zdanie wzgl�dem zgodno�ci rozstrzygalno�ci z prawdopodobie�stwami.
		  * Uwaga: to nie jest w stanie ca�kiem odrzuci� nieoznaczonych token�w wzorcowych.
		  */
		int Test(Corpus::CSentence& sentence, Corpus::CSentence& orginalSentence);
		
		/// Jaka� inna procedura testowa, nie wiem co robi (A.R.).
		int Test(Corpus::CSentence& sentence, std::vector<Corpus::CValue >& ambClasses,
			std::vector<Corpus::DLONG >& disAmbClasses, std::vector<int>& disAmbCount);

		/** Zwraca liczb� token�w w zdaniu, kt�re r�ni� si�. Uwaga: tokeny odrzucane (oznaczone
		  * w korpusie wzorcowym jako ign) s� zawsze traktowane jako dobrze rozpoznane.
		  * UWAGA: w zdaniu testedSentence rozpatrywane s� tylko tagi rozstrzygaj�ce!
		  */
		int TestInterTagged(Corpus::CSentence &testedSentence, Corpus::CSentence &referenceSentence);
		
		/** Nowa procedura testowa.
		  * Sprawdza, czy tokeny r�ni� si�, tj. maj� pust� cz�� wsp�ln� po rzutowaniu
		  * na mask� warstw.
		  * UWAGA: w obu tokenach rozpatrywane s� tylko tagi rozstrzygaj�ce! */
		bool TestInterTagged(Corpus::CToken *testedToken, Corpus::CToken *referenceToken);
		
		/** Sprawdza, czy token z korpusu wzorcowego jest do odrzucenia, tj.
		  * -- brakuje mu leksem�w oznaczonych jako rozstrzygaj�cych lub
		  * -- wyst�puje ign po�r�d leksem�w (niekoniecznie rozstrzygaj�cy). */
		bool ShallWeReject(Corpus::CToken *referenceToken);
		
		void PrintStatistics(std::ofstream& file);

		bool IsAmb(Corpus::CSentence& sentence, int tokenNum);

	public:
		//poniewaz nie chce za duzo mieszac przy okazji licze test tylko dla tych tokoenow
		//dla ktorych znalazlem kolokacje a wynik dla kazdego zdania zapisuje w 
		//ponizszej zmiennej statycznej
		
		
		static int testCollocations;

	private:
		TaKIPI::CLayers layers;
		Corpus::CValue mask;

		// Statystyki: dla danej klasy niejednoznaczno�ci (pair<int,DLONG>),
		// ile by�o prypadk�w i ile zosta�o odgadni�tych (pair<int,int>.
		std::map<Corpus::CValue,std::pair<int,int> > statistics;
	};
}//TaKIPI

#endif

