/*  TaKIPI 1.5
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki WrocÅ‚awskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Adam Radziszewski, Maciej Piasecki,
 *  Bartosz Broda, Adam WardyÅ„ski
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

		/** Testuje dane zdanie wzglêdem zgodnoœci rozstrzygalnoœci z prawdopodobieñstwami.
		  * Uwaga: to nie jest w stanie ca³kiem odrzuciæ nieoznaczonych tokenów wzorcowych.
		  */
		int Test(Corpus::CSentence& sentence, Corpus::CSentence& orginalSentence);
		
		/// Jakaœ inna procedura testowa, nie wiem co robi (A.R.).
		int Test(Corpus::CSentence& sentence, std::vector<Corpus::CValue >& ambClasses,
			std::vector<Corpus::DLONG >& disAmbClasses, std::vector<int>& disAmbCount);

		/** Zwraca liczbê tokenów w zdaniu, które ró¿ni¹ siê. Uwaga: tokeny odrzucane (oznaczone
		  * w korpusie wzorcowym jako ign) s¹ zawsze traktowane jako dobrze rozpoznane.
		  * UWAGA: w zdaniu testedSentence rozpatrywane s¹ tylko tagi rozstrzygaj¹ce!
		  */
		int TestInterTagged(Corpus::CSentence &testedSentence, Corpus::CSentence &referenceSentence);
		
		/** Nowa procedura testowa.
		  * Sprawdza, czy tokeny ró¿ni¹ siê, tj. maj¹ pust¹ czêœæ wspóln¹ po rzutowaniu
		  * na maskê warstw.
		  * UWAGA: w obu tokenach rozpatrywane s¹ tylko tagi rozstrzygaj¹ce! */
		bool TestInterTagged(Corpus::CToken *testedToken, Corpus::CToken *referenceToken);
		
		/** Sprawdza, czy token z korpusu wzorcowego jest do odrzucenia, tj.
		  * -- brakuje mu leksemów oznaczonych jako rozstrzygaj¹cych lub
		  * -- wystêpuje ign poœród leksemów (niekoniecznie rozstrzygaj¹cy). */
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

		// Statystyki: dla danej klasy niejednoznacznoï¿½ci (pair<int,DLONG>),
		// ile byï¿½o prypadkï¿½w i ile zostaï¿½o odgadniï¿½tych (pair<int,int>.
		std::map<Corpus::CValue,std::pair<int,int> > statistics;
	};
}//TaKIPI

#endif

