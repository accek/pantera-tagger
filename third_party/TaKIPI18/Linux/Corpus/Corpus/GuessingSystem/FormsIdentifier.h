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

		/// Klasa do identyfikacji form. Stanowi po³¹czenie Morfeusza oraz Guessera dla form nieznanych morfeuszowi.
		class CORPUS_API FormsIdentifier
		{
		public:

			/// Ró¿ne rodzaje u¿ywania taggera.
			enum GuesserUsage
			{
				/// Odgadniêcie wszystkich form nierozpoznanych przez Morfeusza.
				guessAll,
				/// Do przysz³ego u¿ytku, NIE DZIA£A
				guessLetters,
				/// Nie odgadywanie form nierozpoznanych przez Morfeusza (guesser nie jest wczytywany).
				guessNone
			};

			/** Konstruktor, gdzie podawane jest kodowanie wejœciowe, a tak¿e kodowanie wyjœciowe znaków
			 oraz sposób u¿ywania guessera. */
			FormsIdentifier(Parser::Encoding inputEncoding, Parser::Encoding outputEncoding, int usage);
			~FormsIdentifier(void);

			/// G³ówna metoda dzia³ania, po otrzymaniu formy identyfikuje j¹, zwracaj¹c Token (lub ci¹g tokenów).
			void Identify(std::string form, std::vector<CToken*>& out);

			/// Dzia³a jak Identify, lecz nie dokonuje konwersji. Za³o¿enie: wszystkie stringi kodowane w cp1250.
			void IdentifyNoConv(std::string cp_form, std::vector<CToken*>& out);

			
			/// Usuwa token.
			static void Free(CToken* token)
			{
				delete token;
			}
			/// Opró¿nia wektor z tokenów prawid³owo zwalniaj¹c pamiêæ. Wektor mo¿na u¿yæ ponownie.
			static void FreeTokenVector(std::vector<CToken*> &tokens);

		private:

			/// Metoda wczytywania guessera
			void LoadGuesser(std::string endingTreeFile, std::string endingTagSetsFile);
			/// Rodzaj u¿ycia guessera.
			int guesserUsage;
			/// Guesser do odgadywania form.
			GuessingSystem::Guesser* guesser;

			/// Kodowanie wejœciowe znaków.
			Parser::Encoding inputEncoding;
			/// Kodowanie wyjœciowe znaków.
			Parser::Encoding outputEncoding;
		};
	}
}
#endif
