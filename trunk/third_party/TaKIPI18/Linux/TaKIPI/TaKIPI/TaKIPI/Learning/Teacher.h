/*  TaKIPI 1.5
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Adam Radziszewski, Maciej Piasecki,
 *  Bartosz Broda, Adam Wardyński
 *
 *  Kod na licencji GPL.
 */

#ifndef TAKIPI_TEACHER_H
#define TAKIPI_TEACHER_H

#include "../../takipi_headers.h"
#include "../Layers.h"
#include "../Tagger.h"

namespace TaKIPI
{
	namespace Learning
	{
		class TAKIPI_DEV_API CTeacher
		{
		public:
			CTeacher(CLayers layers, Corpus::Tokenisation::CTokenReader& corpora, int layer);

			~CTeacher(void);

			Corpus::CToken::Attribute GetAttribute();

			// Przechodzi do pierwszej pozycji w zdaniu zgodnej z CFilter::IsAmb
			bool FirstPosition();
			// Przechodzi do następnej pozycji w zdaniu zgodnej z CFilter::IsAmb
			bool NextPosition();

			// Wczytuje następne zdanie z korpusu.
			bool ReadNextSentence();

			// Główna metoda uczenia
			void Learn(bool normalTrees = true, bool disambOnly = false, bool useRules = false);

			// Ładowanie
			void InitFromFile(std::string fileName);
			// Tymczasowe ladowania (do pojawienia się antalera)
			void TempInit();
			void CreateFiles();

		private:
			// Poziom, dla którego mamy 
			int treeGenerationLevel;

			// Klasa zawierająca uporządkowany wektor atrybutów reprezentujących daną warstwę.
			CLayers layers;

			// Mechanizm do wczytywania tokenow z korpusu
			Corpus::Tokenisation::CTokenReader& reader;

			// Klasa skladajaca tokeny w zdania.
			Corpus::ExtSentencer::CSentencer* sentencer;

			// Wskaźnik na obecnie przetwarzane zdanie.
			Corpus::CSentence* sentence;

			// Tworzy pliki drzew C4.5, jeśli false = normalTrees to drzewa są dokładniejsze,
			// nie tak bardzo ucinane.
			void SaveFiles(bool normalTrees);

			//AW!!
			void SaveLMTFiles();
			void SaveJRipFiles();
			void SavePartFiles();

			JOSKIPI::Rules::CRuleSet* rules;

			Corpus::TextUtils::CorpusUtil::CDump *dump;

			std::map<Corpus::CValue,JOSKIPI::Patterns::CPattern*> *patterns;
			std::map<Corpus::CValue,std::ofstream*> *patternsOut;
			std::vector<JOSKIPI::Operators::COperator*>* vUniv;

			CRTTTagger* tagger;

			int layer;
		};

	}//namespace Learning 
}//	namespace TaKIPI

#endif // TAKIPI_TEACHER_H


