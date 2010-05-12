/*  TaKIPI 1.5
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Adam Radziszewski, Maciej Piasecki,
 *  Bartosz Broda, Adam Wardyński
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

		// Ustawia pobiera referencje do zda�: przetwarzanego (sta�ego na tym poziomek) i wynikowego.
		void SetSentence(Corpus::CSentence* sourceSentence, Corpus::CSentence* workingSentence);

		// Przechodzi do pierwszej permutacji.
		bool FirstPermutation();
		// Przechodzi do nast�pnej permutacji. 
		bool NextPermutation();

		// Zmienila sie warstwa
		bool NewLayer(bool withPermutations);
		bool FirstLayer(bool withPermutations);

		// Ustawia wska�nik zdania na nast�pny nieujednoznaczniony token.
		bool NextAmbToken();

		// Metoda pobiera numer tokenu i zwracaj�ca informacj�, czy wskazany token jest
		// niejednoznaczny w odniesieniu do bie��cej warstwy.
		bool IsAmb(int tokenNum);

		// Wype�nia par�, klas� niejednoznaczno�ci danego tokena.
		void GetAmbClass(int tokenNum, Corpus::CValue& out);

		// Moves the sentence's pointer to the next ambiguate token.
		bool MoveToNextAmbToken();
		// Sortuje znaczniki token'a wzgl�dem danej warstwy.
		void Sort(Corpus::CToken& token);

	private:
		// Informacje o warstwach tagowania.
		TaKIPI::CLayers& layers;

		// Aktualnie permutowane zdanie (source)
		Corpus::CSentence* srcSentence;
		// Zdanie wynikowe w kt�rym zawarta jest obecna permutacja.
		Corpus::CSentence* wrkSentence;

		// Wektor stan�w.
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

