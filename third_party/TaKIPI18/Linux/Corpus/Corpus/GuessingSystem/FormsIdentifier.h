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

#ifndef CORPUSLIB_FORMSIDENTIFIER_H
#define CORPUSLIB_FORMSIDENTIFIER_H

#include "../../corpuslib_headers.h"
#include "../Token.h"
#include "../Parser/Parser.h"
#include "Guesser.h"

namespace Corpus
{

	class CToken;

	namespace GuessingSystem
	{

		/// Klasa do identyfikacji form. Stanowi po��czenie Morfeusza oraz Guessera dla form nieznanych morfeuszowi.
		class CORPUS_API FormsIdentifier
		{
		public:

			/// R�ne rodzaje u�ywania taggera.
			enum GuesserUsage
			{
				/// Odgadni�cie wszystkich form nierozpoznanych przez Morfeusza.
				guessAll,
				/// Do przysz�ego u�ytku, NIE DZIA�A
				guessLetters,
				/// Nie odgadywanie form nierozpoznanych przez Morfeusza (guesser nie jest wczytywany).
				guessNone
			};

			/** Konstruktor, gdzie podawane jest kodowanie wej�ciowe, a tak�e kodowanie wyj�ciowe znak�w
			 oraz spos�b u�ywania guessera. */
			FormsIdentifier(Parser::Encoding inputEncoding, Parser::Encoding outputEncoding, int usage);
			~FormsIdentifier(void);

			/// G��wna metoda dzia�ania, po otrzymaniu formy identyfikuje j�, zwracaj�c Token (lub ci�g token�w).
			void Identify(std::string form, std::vector<CToken*>& out);

			/// Dzia�a jak Identify, lecz nie dokonuje konwersji. Za�o�enie: wszystkie stringi kodowane w cp1250.
			void IdentifyNoConv(std::string cp_form, std::vector<CToken*>& out);

			
			/// Usuwa token.
			static void Free(CToken* token)
			{
				delete token;
			}
			/// Opr�nia wektor z token�w prawid�owo zwalniaj�c pami��. Wektor mo�na u�y� ponownie.
			static void FreeTokenVector(std::vector<CToken*> &tokens);

		private:

			/// Metoda wczytywania guessera
			void LoadGuesser(std::string endingTreeFile, std::string endingTagSetsFile);
			/// Rodzaj u�ycia guessera.
			int guesserUsage;
			/// Guesser do odgadywania form.
			GuessingSystem::Guesser* guesser;

			/// Kodowanie wej�ciowe znak�w.
			Parser::Encoding inputEncoding;
			/// Kodowanie wyj�ciowe znak�w.
			Parser::Encoding outputEncoding;
		};
	}
}
#endif
