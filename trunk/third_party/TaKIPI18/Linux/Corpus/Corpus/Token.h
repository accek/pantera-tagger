/*  Biblioteka Corpus
 *  Zestaw narzędzi do przetwarzania tekstów w języku polskim.
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Adam Radziszewski, Maciej Piasecki,
 *  Bartosz Broda, Adam Wardyński
 *
 *  Kod na licencji GPL.
 */

#ifndef CORPUSLIB_TOKEN_H
#define CORPUSLIB_TOKEN_H

#include "../corpuslib_headers.h"
#include "Lexem.h"
#include "Parser/Parser.h"
//#include "Config.h"
#include "functions.h"

namespace Corpus
{
	CORPUS_API typedef std::vector<CLexem*>	LEXEM_VECTOR;

	class CORPUS_API CToken
	{
	public:

		int RefCount;

		CValue ambClass;
		
		///true, jesli dany token nie jest po przerwie, a false, jesli jest
		bool NS;
		
		/** variantNum == 0 <= token nie nale�y do wariant�w segmentacji (najcz��ciej).
		  * W przeciwnym wypadku variantNum okre�lu numer wariantu segmentacji,
		  * tj. alternatywnego ci�gu token�w, np. mia�:1 em:1 mia�em:2. */
		UBYTE variantNum;
		
		//tworzy nowy "pusty" token
		static CToken* CreateBlank();
		static CToken* CreateBlank(const std::string& orth);
		
		/**
		Tworzy nowy obiekt, parsujac dane wczytane ze strumienia
		\param readOpeningTag - czy czytac tag otwieraj�cy.
		domyslnie ustawia NS na false, nie odczytujac tej informacji ze strumienia!
		*/
		CToken(std::istream& input, bool readOpeningTag = true);
		
		//s�u�y do tworzenia pustego tokena
		CToken(const std::string& orth);

		/** Pobiera s�owo z tekstu, ktore pos�u�y�o do utworzenia tego tokenu
		  *(tj. ORTH). */
		const std::string& GetOrth() const
		{
			return CWordDictionary::GetInstance()->GetAt(orthIdx);
		}

		/// Zmienia orth'a tokenu na zadany.
		const void SetOrth(const std::string& orth)
		{
			orthIdx = CWordDictionary::GetInstance()->Add(orth);
		}
		const void SetOrth(const int orthIndex)
		{
			orthIdx = orthIndex;
		}
		/// Pobiera i-ty leksem, indeks od zera.
		CLexem* GetLexem(int i) const
		{
			return lexems[i];
		}
		/** Pobiera pierwszy leksem stanowi�cy dezambiguacj� tokena.
		  * UWAGA: je�li takiego leksemu nie ma, zachowanie NIEOKRE�LONE. */
		const CLexem& DisambLexem() const
		{
			return *lexems[DisambLexemIndex()];
		}

		/** Pobiera numer pierwszego rozstrzygaj�cego leksemu.
		  * Zwraca -1, gdy takiego nie ma. */
		const int DisambLexemIndex() const
		{
			for(int i = 0; i < (int)lexems.size(); i++)
			{
				if( lexems[i]->GetDisamb())
					return i;
			}
			return -1;
		}

		// Zwraca liczno�� rozstrzygni�tych lexem�w w danym tokenie.
		const int DisambCount() const
		{
			int count = 0;

			for(int i = 0; i < (int)lexems.size(); i++)
			{
				if( lexems[i]->GetDisamb())
				{
					count++;
					if (count == 2)
						return multi;
				}
			}

			if(count == 0)
				return none;
			else
				return one;
		}

		//pobiera ilosc leksemow
		const size_t GetLexCount() const
		{
			return lexems.size();
		}

		CToken(const CToken& t);
		const CToken& operator =(const CToken& t);

		void ClearLexems()
		{
			for(int k = 0; k < (int)lexems.size(); k++)
			{
				CLexem* l =	lexems[k];
				if(l->RefCount <= 1)
				{
					delete l;
				}
				else
					l->RefCount--;
			}
			lexems.clear();
		}

		void AddLexem(CLexem* l)
		{
			lexems.push_back(l);
		}
		
		/// Dodaje leksem o podanym lemacie i tagu; bez disamba i prob=0.
		void AddLexem(const std::string &cx_base, const std::string &cx_tag);

		std::vector<CLexem*>& GetLexems()
		{
			return lexems;
		}

		CToken* CreateCopy();

		CToken* CreateHardCopy();

		//operator s�u�y do pobierania z tokenu warto�ci po indexie
		//zwraca zakodowane binarnie warto�ci danego atrybuty dla wszystkich lexem�w
		DLONG operator[](int attribute);
		//operator s�u�y do pobierania z tokenu warto�ci po masce
		CValue operator[](const CValue& mask);

		~CToken(void);

		// Enumerator efiniuj�cy nam attrybuty
		enum Attribute
		{
			orth = 0,
			base = 1,

			flex = 2,//1
			nmb = 3,//2
			cas = 4,//4
			gnd = 5,//2
			per = 6,//3
			deg = 7,
			asp = 8,
			neg = 9,
			acm = 10,
			acn = 11,
			ppr = 12,
			agg = 13,
			vcl = 14,
			inp = 15,
			tnt = 16, // t/numtype (klasa tnum)
			END = 17 //  pozala ustali� liczb� atrybut�w
		};

		// Enumerator definiuj�cy ilo�ci ujednoznacznionych lexem�w w tokenie.
		enum DisambCount
		{
			multi = INT_MIN,
			none  = -1,
			one   = 0
		};

		//sprawdza, czy dany atrybut zwraca warto�� bitow� czy string'a
		static bool IsStrAttribute(int index)
		{
			if (index == orth || index == base)
				return true;
			return false;
		}

		// Usuwa lexem o podanym indeksie.
		void RemoveLexem(int i)
		{
			std::vector<CLexem*>::iterator iter = lexems.begin();
			iter += i;
			CLexem *l = *iter;
			lexems.erase(iter);
			if(l->RefCount > 1)
				l->RefCount--;
			else
				delete l;
		}
		
		/// Ustawia wszystkie disamby na false, a prawdopodobie�stwa na zero.
		void ClearDisambsAndProbs();
		
		/** Przetwarza wstępnie niejednoznaczności. Jeśli występuje
		niejednoznaczność subst-ger, wszystkie leksemy subst lub ger otrzymują
		tag subst+ger. Jeśli występuje ign, pozostałe leksemy są usuwane. */
		void PreprocessSubstGerIgn();
		
		/** Przywraca niejednoznaczność subst-ger do normalnej postaci, tj.
		  * wszystkie leksemy z tagiem subst+ger są zamieniane na subst
		  * lub ger na podstawie obecności odpowiednich atrybutów. */
		void PostprocessSubstGer();

		/// Sprawdzamy, czy dla danego atrybutu Token jest jednoznaczny.
		bool IsDisambAttr(int attribute);

		// Sprawdza prawdopodobie�stwo Lexemu
		double GetLexProbability(int lexem);
		// Zmienia prawdopodobie�stwo Lexemu dodaj�c logarytm
		void SetLexProbability(int lexem, double lProbability);
		
		// Serializacja
		void Serialize(std::ofstream& file);
		static CToken* Deserialize(std::ifstream& file);
		
		void SerializeAsText(std::ostream& file);
		static CToken* DeserializeFromText(std::istream& file);

		// Do ponownej zamiany na Xml'a
		std::string ToXmlString(bool writeProbs = false);
		
		void ConvertEncoding(Parser::Encoding fromEnc, Parser::Encoding toEnc);
		//s�u�y do tworzenia pustego tokena
		CToken(bool createBlankLex);
	private:

		
		//index ze slownika, pod ktorym znajduje sie orth danego tokenu
		int orthIdx;
		// wektor tag�w

		LEXEM_VECTOR lexems;
	};

#ifdef WIN32
	template class CORPUS_API std::allocator<CToken*>;
	template class CORPUS_API std::vector<CToken*, std::allocator<CToken*> >;
#endif

}


#endif

