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

#ifndef CORPUSLIB_TRANSDUCER_H
#define CORPUSLIB_TRANSDUCER_H

#include "../corpuslib_headers.h"
#include "TransducerNode.h"
#include "Token.h"
#include "Readers/CorporaReader.h"

namespace Corpus
{

	class CORPUS_API CTransducer
	{
	public:
		CTransducer(void);
		~CTransducer(void);

		static CTransducer* ParseFromFile(std::string fileName);

		void AddNewForm(const std::string& form, const std::vector<int>& outForm);
		bool CheckForm(const std::string& form);
		std::vector<CToken*>* Transduce(const std::string& form);

		bool PerformStep(char character);
		bool IsValidState();
		void GoToStart();

		void Serialize(std::string fileName);
		static CTransducer* Deserialize(std::string fileName);

	private:
		CTransducerNode* root;
		static void ParseRule(std::vector<std::string>& rule, CTransducer* t);
		CTransducerNode* actualState_;

		static size_t transducerCount;
		static std::map<int,CToken*> *tokens;
		static std::map<std::string,int> *tokenLabels;
	};
}
#endif

