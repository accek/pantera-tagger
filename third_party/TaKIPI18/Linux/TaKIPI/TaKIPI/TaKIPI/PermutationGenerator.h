/*  TaKIPI 1.5
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki WrocÅ‚awskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Adam Radziszewski, Maciej Piasecki,
 *  Bartosz Broda, Adam WardyÅ„ski
 *
 *  Kod na licencji GPL.
 */

#ifndef TAKIPI_PERMUTATIONGENERATOR_H
#define TAKIPI_PERMUTATIONGENERATOR_H

#include "../takipi_headers.h"
#include "LexemAscendingSort.h"


namespace TaKIPI
{
	class TAKIPI_DEV_API CPermutationGenerator
	{
	public:
		CPermutationGenerator(TaKIPI::CLayers& layers);
		~CPermutationGenerator(void);

		// Ustawia pobiera referencje do zdañ: przetwarzanego (sta³ego na tym poziomek) i wynikowego.
		void SetSentence(Corpus::CSentence* sourceSentence, Corpus::CSentence* workingSentence);

		// Przechodzi do pierwszej permutacji.
		bool FirstPermutation();
		// Przechodzi do nastêpnej permutacji. 
		bool NextPermutation();

		// Zmienila sie warstwa
		bool NewLayer(bool withPermutations);
		bool FirstLayer(bool withPermutations);

		// Ustawia wskaŸnik zdania na nastêpny nieujednoznaczniony token.
		bool NextAmbToken();

		// Metoda pobiera numer tokenu i zwracaj¹ca informacjê, czy wskazany token jest
		// niejednoznaczny w odniesieniu do bie¿¹cej warstwy.
		bool IsAmb(int tokenNum);

		// Wype³nia parê, klas¹ niejednoznacznoœci danego tokena.
		void GetAmbClass(int tokenNum, Corpus::CValue& out);

		// Moves the sentence's pointer to the next ambiguate token.
		bool MoveToNextAmbToken();
		// Sortuje znaczniki token'a wzglêdem danej warstwy.
		void Sort(Corpus::CToken& token);

	private:
		// Informacje o warstwach tagowania.
		TaKIPI::CLayers& layers;

		// Aktualnie permutowane zdanie (source)
		Corpus::CSentence* srcSentence;
		// Zdanie wynikowe w którym zawarta jest obecna permutacja.
		Corpus::CSentence* wrkSentence;

		// Wektor stanów.
		std::vector<int> states;
		// Permutation position.
		int permPos;
		// Lexems comparator (according to current layer).
		CLexemAscendingSort* las;

		// Moves to the next lexem group.
		bool NextLexemGroup();
	};

}//namespace TaKIPI

#endif //TAKIPI_PERMUTATIONGENERATOR_H

