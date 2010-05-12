/*  Biblioteka Corpus
 *  Zestaw narzÄ™dzi do przetwarzania tekstÃ³w w jÄ™zyku polskim.
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki WrocÅ‚awskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Adam Radziszewski, Maciej Piasecki,
 *  Bartosz Broda, Adam WardyÅ„ski
 *
 *  Kod na licencji GPL.
 */

#ifndef CORPUSLIB_LEXEM_H
#define CORPUSLIB_LEXEM_H

#include "../corpuslib_headers.h"
#include "Tag.h"
#include "WordsDictionary.h"

#include "Value.h"

namespace Corpus
{

	class CTag;

	class CORPUS_API CLexem
	{
	public:
		//buduje lexem na podstawie parsowania strumienia wejsciowego
		//readClosing = czy odczytac ze strumienia zamykajacy tag </lex>
		//disamb -> jesli nie czytac tagu otwierajacego, to ustawic taka wartosc disamb tokenowi
		CLexem(std::istream &input,bool readOpeningTag,bool disamb);
		// Konstruktor kopiuj¹cy.
		CLexem(const CLexem& l);
		CLexem(std::string & base, std::string & tok,bool disamb);
		CLexem(int & baseIdx, std::string & tok,bool disamb);
		//tworzy pusty lexem
		CLexem();
		//destruktor
		virtual ~CLexem(void);

		// Operator przypisania.
		const CLexem& operator=(const CLexem& l);
		const bool operator==(const CLexem& l);

		//pobiera index slowa bazowego dla leksemu w slowniku
		int GetBaseIdx() const {
			return baseIdx;
		}
		//pobiera slowo bazowe
		const std::string& GetBase() const {
			return CWordDictionary::GetInstance()->GetAt(baseIdx);
		}
		const void SeBase(const std::string& base) {
			baseIdx = CWordDictionary::GetInstance()->Add(base);
		}

		//pobiera informacje o tagu
		const CTag& GetTag() const {
			return this->tag;
		}
		//podmienia taga
		void SetTag(CTag& tag) {
			this->tag = tag;
		}
		//pobiera informacje o tym, czy tag stanowi dezambiguacje leksemu, ktoremu jest przyporzadkowany
		bool GetDisamb() const {
			return this->disamb;
		}
		void SetDisamb(bool disamb) {
			this->disamb = disamb;
		}
		//parsuje strumien wejsciowy pod katem otwierajacego tagu <lex>
		//zwraca odpowiadajaca mu wartosc disasm
		//readTag - jesli == NULL, rzucaj wyjatkami jesli odczyta sie co innego niz <lex>
		//a jesli != NULL, to zapisz tam wartosc wczytanego tagu, jesli rozny od <lex>
		static bool ReadOpeningTag(std::istream &input, std::string * readTag = NULL);

		//prawdopodobieñstwo rozstrzygalnoœci danego Lexemu (u¿ywane w procesie taggowania)
		double probability;

		const DLONG operator[](int attribIndex) const;

		// Serializacja
		void Serialize(std::ofstream& file);
		static CLexem* Deserialize(std::ifstream& file);
		void SerializeAsText(std::ostream& file);
		static CLexem* DeserializeFromText(std::istream& file);

		void GetMaskedValue(CValue mask, CValue& out) const;

		int RefCount;
		void PostprocessSubstGer();

		//Do ponownej zamiany na Xml'a
		std::string ToXmlString(bool writeProbs = false);
		CLexem * CreateHardCopy();

	#ifdef OCR
		double CD;
		double bufferCD;
		double lastCD;
	#endif

	private: //fields
		//przechowuje idx bazy lexemu w slowniku wyrazow
		unsigned int baseIdx;
		//przechowuje informacje o tagu lexemu
		CTag tag;
		//czy tag stanowi dezambiguacje leksemu, ktoremu jest przyporzadkowany
		bool disamb;
	};

#ifdef WIN32
	template class CORPUS_API std::allocator<CLexem*>;
	template class CORPUS_API std::vector<CLexem*, std::allocator<CLexem*> >;
#endif
}

#endif

