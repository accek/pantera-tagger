/*  Biblioteka JOSKIPI
 *  J�zyk Opisu Stanu Korpusu IPI
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wroc�awskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Maciej Piasecki, Bartosz Broda, Adam Wardy�ski
 *
 *  Kod na licencji GPL.
 */

#ifndef JOSKIPI_PATTERN_H
#define JOSKIPI_PATTERN_H

#include "../joskipi_headers.h"
#include "../all_operators.h"

namespace JOSKIPI
{
	namespace Patterns
	{
		class JOSKIPI_API CPattern
		{
		public:
			/*
			Konstruujemy okno u�ywaj�c schematu b�d�cego ci�giem operator�w.
			*/
			CPattern(std::vector<JOSKIPI::Operators::COperator*>* schema, Corpus::TFlexClass flexClass,
				Corpus::DLONG gramCat);

			~CPattern();

			/*
			Wrzuca wz�r ucz�cy dla danego zdania, potrzebny do tworzenia drzew decyzyjnych c4.5, do strumienia.
			*/
			void GeneratePattern(Corpus::CValue &layerMask, Corpus::CSentence& srcSentence, 
				std::ostream& out, bool disambOnly);

			/*
			Dla danego okna generuje dane potrzebne do okre�lenia nazw w drzewie QuinLana.
			*/
			void GenerateValuesPattern(std::stringstream& out);

			/*
			Dla danego okna generuje dane jw., ale na potrzeby pliku .arff.
			*/
			void GenerateArffValuesPattern(std::stringstream& out);

			//Zwraca warto�� attrybutu reprezentowanego przez string'a
			void GetValueAt(int opIndex, Corpus::CSentence& sentence, int pos,Corpus::CValue& out);

			//Zwraca string reprezentuj�cy dany operator.
			std::string GetName(int index);

			//Zwraca d�ugo�� danego okna (schematu okna).
			int GetLength();

			//Zwraca ilo�� wzorc�w ucz�cych zamieszczonych w patternie
			int GetPatternSize();

			//Dodaje operator do wzorca
			void AddOperator(JOSKIPI::Operators::COperator* oper);

			//Dodaje zbi�r operator�w do wzorca
			void AddOperators(std::vector<JOSKIPI::Operators::COperator*>* opers);

			//Zwraca wektor string'�w reprezentuj�cy dany schemat.
			void GetNames(std::vector<std::string>& out);

			const Corpus::DLONG GetGramCat() const {return gramaticalCat;}
			const Corpus::TFlexClass GetFlexClass() const {return flexClass;}

			//Zwraca reprezentacje string'ow� wzorca.
			std::string ToString();

		private:
			//Schemat danego okna wyra�ony jako wektor operator�w.
			std::vector<JOSKIPI::Operators::COperator*>* schema;
#ifdef JOSKIPI_DEVELOPER
#ifdef WIN32
			template class JOSKIPI_API std::allocator<Corpus::DLONG>;
			template class JOSKIPI_API std::vector<Corpus::DLONG, std::allocator<Corpus::DLONG> >;

#endif //WIN32
#endif //JOSKIPI_DEVELOPER

			std::vector<Corpus::DLONG> names;

			//Ilo�� wzorc�w ucz�cych wygenerowanych przez pattern.
			int patternSize;

			// Klasa fleksyjna.
			Corpus::TFlexClass  flexClass;
			// Kategoria gramatyczna.
			Corpus::DLONG gramaticalCat;
		};
	}// namespace Patterns
}// namespace JOSKIPI
#endif //JOSKIPI_PATTERN_H
