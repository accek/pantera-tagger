/*  TaKIPI 1.5
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki WrocÅ‚awskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Adam Radziszewski, Maciej Piasecki,
 *  Bartosz Broda, Adam WardyÅ„ski
 *
 *  Kod na licencji GPL.
 */

#ifndef TAKIPI_TAGGER_H
#define TAKIPI_TAGGER_H

#include "../takipi_headers.h"
#include "PermutationGenerator.h"
#include "UDictionary.h"
#include "CutOff.h"

#include "Classifiers/Classifier.h"
#include "Classifiers/NegRuleDecisionTree.h"

namespace TaKIPI
{
	class TAKIPI_DEV_API CRTTTagger
	{
	public:
		// Konstrutkor
		CRTTTagger(TaKIPI::CLayers& layers, int iterations, double cutOffLevel, bool dumpRules = false);
		~CRTTTagger(void);

		// Metoda do inicjalizowania taggera (tymczasowa).
		void TempInit();

		// Metoda wczytuje dane do taggera.
		void Load(bool loadWordDictionary = true, bool loadUDictionary = true);

		/// Metoda rozstrzygaj¹ca o zdaniu. Wyrzuca leksemy, które nie spe³niaj¹ warunków; disamby nie maj¹ znaczenia.
		void Disambiguate(Corpus::CSentence& sentence, bool filtered, bool withPermutations, bool useRules);

		/// Wywo³uje pojedyncz¹ iteracjê.
		bool Iteration(Corpus::CSentence& sentence,
			std::map<int,std::vector<std::pair<JOSKIPI::Patterns::CPattern*,Classifiers::CClassifier*> > >& chosenTrees);

		/// Wywo³uje pojedyncz¹ permutacje.
		bool ProceedSinglePermutation(Corpus::CSentence& sentence,
			std::map<int,std::vector<std::pair<JOSKIPI::Patterns::CPattern*,Classifiers::CClassifier*> > >& chosenTrees);

		/// Normalizuje wartoœci w tagerze (wzi¹zszy pod uwagê logarytmiczne wartoœci).
		void NormalizeProbabilities(Corpus::CToken& t);
		void NormalizeProbabilities(Corpus::CSentence& sentence);

		void InitFromFile(std::string fileName);

		JOSKIPI::Rules::CRuleSet* GetRules();

		/** Oznacza disambem w zdaniu tagdSent wszystkie leksemy, które nale¿¹ do sourceDisdSent (nie wa¿ne czy maj¹ tam disamba).
		  * Ustawienie forceOne powoduje wymuszenie dok³adnie jednego (ew. braku) disamba na wyjœciu. */
		void TagSentence(Corpus::CSentence *tagdSent, Corpus::CSentence *sourceDisdSent, bool forceOne = false);
		void SeleceDecisionTrees(std::map<int,std::vector<std::pair<JOSKIPI::Patterns::CPattern*,Classifiers::CClassifier*> > >
			& chosenTrees, 	Corpus::CSentence& sentence);

		/** Symulacja narzedzia koder.exe z poprzedniej wersji tagera.
		  * Laduje drzewa w formie tekstowej (w formacie generatora drzew c4.5)
		  * i serializuje do tagerowej postaci binarnej. */
		static void RunKoder();

		/** Generuje slownik WordDict i slownik unigramowy, po czym zapisuje je
		  * do standardowych sciezek. */
		static void GenerateDicts(std::string trainCorpusFile);
		/** Konwertuje slowniki z bin do txt (toTxt == true) lub na odwrot. */
		static void ConvertDicts(bool toTxt);

	private:
		/// For remembering the state of pseudo-random number generator.
		unsigned int randstate;
		
		bool dumpRuleInfo;
		/// Kolekcja przechowuj¹ca dane do taggera.
		std::map<Corpus::CValue,Classifiers::CClassifier*> *trees;
		/// Drzewa negatywne.
		std::map<Corpus::CValue,Classifiers::CNegRuleDecisionTree*> *negTrees;
		/// Kolekcja przechowuj¹ca obiekty typu pattern dla klasyfikatora.
		std::map<Corpus::CValue,JOSKIPI::Patterns::CPattern*> *patterns;
		/// Kolekcja przechowuj¹ca obiekty typu pattern dla drzew negatywnych.
		std::map<Corpus::CValue,JOSKIPI::Patterns::CPattern*> *negPatterns;

		/* Kolekcja przechowuj¹ca dane do taggera.
		//std::vector<Classifiers::CClassifier*> *sentenceTrees;
		 Kolekcja przechowuj¹ca obiekty typu pattern.
		//std::vector<JOSKIPI::Patterns::CPattern*> *sentencePatterns;//*/
		JOSKIPI::Rules::CRuleSet* rules;

		/// tryb ³adowania
		enum LoadMode
		{
			Override,
			Bag,
			VotingWTA,
			VotingAverage
		};

		/** Ogranicza liczbêe disambów do 1 -- jeœli jest ich wiêcej, jeden jest losowany. */
		void ForceOneDisamb(Corpus::CToken *tagdTok);
		
		/// Taken from POSIX.1-2001 example of rand_r definition (re-entrant pseudo-random generator).
		unsigned int RandomR();
		
		void AverageClassifier(std::vector<Corpus::CToken*> probTokens, Corpus::CSentence* sentence);

		/// £aduje dane drzew do Taggera. Musi byc zawsze pierwsze wywolane
		void LoadTrees(LoadMode loadMode = Override);
		/// £aduje dane drzew negatywnych do Taggera.
		void LoadNegTrees();
		/// £aduje dane drzew LMT do Taggera.
		void LoadLMTrees(LoadMode loadMode = Override);
		/// £aduje zestawy regu³ Rippera do Taggera
		void LoadRipperRules(LoadMode loadMode = Override);
		/// £aduje zestawy regu³ PART do Taggera
		void LoadPartRules(LoadMode loadMode = Override);
		/// £aduje wszystkie klasyfikatory (c4.5, LMT, Ripper, PART) na raz
		void LoadAllBagged(LoadMode loadMode);
		/// £aduje s³ownik unigramowy do Taggera.
		void LoadDictionary(bool loadWordDictionary, bool loadUDictionary);
		/// Ustawia pocz¹tkowe prawdopodobieñstwa.
		void SetStartProbabilities(Corpus::CSentence& sentence);
		/// Informacje o warstwach tagowania.
		TaKIPI::CLayers& layers;
		/// Iloœæ iteracji na warstwê.
		int iterations;
		/// Generator permutacji.
		CPermutationGenerator pGen;
		/// S³ownik unigramowy.
		CUDictionary* dictionary;
		/// Bufor do wyrzucania wyników poœrednich.
		Corpus::TextUtils::CorpusUtil::CDump* dump;
		/// Klasa do odcinania, po iteracjach.
		CCutOff* cutOff;
		std::vector<JOSKIPI::Operators::COperator*>* vUniv;


		CRTTTagger(const CRTTTagger&);//no implementation
		CRTTTagger& operator= (CRTTTagger&);//no implementation
	};
}//namespace TaKIPI

#endif //TAKIPI_TAGGER_H

