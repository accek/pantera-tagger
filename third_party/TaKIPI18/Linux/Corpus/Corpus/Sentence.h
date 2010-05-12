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

#ifndef CORPUSLIB_SENTENCE_H
#define CORPUSLIB_SENTENCE_H

#include "../corpuslib_headers.h"
#include "Token.h"
#include "Position.h"
#include <deque>

namespace Corpus
{
	/**
	Reprezentacja logiczna zdania, będącego wektorem tokenów. Posiada aktualną pozycję wraz z metodami poruszania się.	
	 */
	class CORPUS_API CSentence
	{
	public:
		/// Tworzenie zdania z wektora tokenów.
		CSentence(std::vector<CToken*> &sentence, bool removeTokensInDtor = true);
		CSentence(std::deque<CToken*> &sentence, bool removeTokensInDtor = true);
		
		/// Konstruktor pustego zdania.
		CSentence();
		
		/// Zwalnia pamiec zajmowana przez zdanie oraz jego tokeny.
		~CSentence(void);

		/// Zwraca token na danej pozycji w zdaniu.
		CToken* operator[](int position)
		{
			if (position < 0 || position >= Size())
				return blankToken;
			return sentence[position];
		}

		/// Zwraca token na danej pozycji w zdaniu.
		CToken* GetToken(int position);

		/// Zwraca rozmar zdania wyrażony w ilości tokenów.
		int Size()
		{
			return (int)sentence.size();
		}

		/// Przechodzenie po zdaniu (wybranym Tokenem zakładką).
		void Goto(int position);

		/// Zwraca aktualną pozycję w zdaniu.
		int GetPosition();

		/// Ustawia wskaznik obecnego tokena na pierwszy element.
		bool FirstPosition();
		
		/// Ustawia wskaznik obecnego tokena na następny element.
		bool NextPosition();

		/**
		Zwraca obecnie wskazywany token, lub wartość NULL jeśli żaden token
		nie jest wskazywany oraz gdy ilość tokenów w zdaniu == 0. */
		CToken* GetCurrentToken()
		{
			if(chosenToken == CPosition::nowhere || (int)sentence.size() == 0)
				return NULL;
			else
				return sentence[chosenToken];
		}

		/**
		Tworzy "płytką" kopię zdania, gdzie każdy token jest zastępowany tokenem pustym.
		(chyba nieużywane). */
		CSentence* CreateShallowCopy();
		/**
		Tworzy "mięką" kopię zdania, gdzie tokeny są te same,
		ale lexemy są kopiowane. */
		CSentence* CreateCopy();
		
		/// Tworzy "twardą" kopię zdania.
		CSentence* CreateHardCopy();
		
		/// Tworzy kopie "twardą" zdania, ale wyrzucane są lexemy "nie disamb".
		CSentence* CreateDisambCopy();

		/// Dodaje token na koniec zdania.
		void AddToken(CToken *token);

		/**
		Zamienia token na pozycji podanej w parametrze na token także podany
		w parametrze. */
		void Replace(CToken* token, int position);

		/// Zwraca reprezentację zdania w postaci XML'a KIPI.
		std::string ToXmlString(bool writeProbs = false);

		/** Konwertuje wewnętrzne kodowanie znaków użyte w polach orth i base
		leksemów, zakładając, że było fromEnc, a ma być toEnc.
		TODO: tego typu konwersja powoduje pęcznienie słownika
		-- poprzednie wersje pól nie są z niego wyrzucane. */
		void ConvertEncoding(Parser::Encoding fromEnc, Parser::Encoding toEnc);
		
		/** Przetwarza wstępnie niejednoznaczności. Jeśli występuje
		niejednoznaczność subst-ger, wszystkie leksemy subst lub ger otrzymują
		tag subst+ger. Jeśli występuje ign, pozostałe leksemy są usuwane. */
		void EliminateSubstGerIgn();


		/** Zwraca ciag orthow tego zdania oddzielonych spacjami. */
		std::string ToOrthString();

		void EraseTokens(const std::set<size_t> &indices);
		
		/// Ustawie wszystkie disamby na false, a prawdopodobieństwa na 0.
		void ClearDisambsAndProbs();
		
		void FilterDisamb()
		{
			//code taken from Corpus::Tokenisation::CCorpusTokenReader
			for(size_t tok_no = 0; tok_no < sentence.size(); tok_no++)
			{
				CToken * token = sentence[tok_no];
				for(int i = 0; i < (int)token->GetLexCount(); i++)
				{
					if( token->GetLexem(i)->GetDisamb() != true)
					{
						token->RemoveLexem(i);
						i--;
					}
				}
				// leave only one disamb (actually delete the last one)
				// this is copied from the old code without change
				while(token->GetLexCount() > 1)
				{
					token->RemoveLexem((int)token->GetLexCount()-1);
				}
			}
		}


	protected:
#ifdef WIN32
		template class CORPUS_API std::allocator<CToken*>;
		template class CORPUS_API std::vector<CToken*, std::allocator<CToken*> >;
#endif
		/// Zdanie przechowywane jako wektor Tokenów.
		std::vector<CToken*> sentence;

		/// Indeks aktualnie wybranego tokenu.
		int chosenToken;

		private:
		/**
		Łata na dziurę w JOSKIPI? Np. podczas tagowania CSimpleOperator
		gdy pobiera token (pierwsza linia), nie sprawdza zakresu... */
		CToken* blankToken;

		bool m_bIOwnTokens;
	};
}

#endif

