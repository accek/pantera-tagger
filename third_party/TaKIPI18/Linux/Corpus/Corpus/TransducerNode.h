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

#ifndef CORPUSLIB_TRANSDUCERNODE_H_
#define CORPUSLIB_TRANSDUCERNODE_H_

#include "../corpuslib_headers.h"

namespace Corpus
{

	class CORPUS_API CTransducerNode
	{
	public:
		CTransducerNode(void);
		~CTransducerNode(void);

		void AddNewForm(const char *form, const std::vector<int>& finalFormCode);
		bool CheckForm(const char* form);
		std::vector<int>& Transduce(const char *form);

		inline bool IsValid()
		{
			return finalForm.empty();
		}

		CTransducerNode* PerformStep(char character);

	protected:
#ifdef WIN32
		template class CORPUS_API std::allocator<int>;
		template class CORPUS_API std::vector<int, std::allocator<int> >;
#endif
		std::vector<int> finalForm;
		std::map<char,CTransducerNode*> *nodes;

		void AddUnionForm(const char* form, const std::vector<int>& finalFormCode);
		void AddOptionalityForm(const char* form, const std::vector<int>& finalFormCode);
		void AddIterationForm(const char* form, const std::vector<int>& finalFormCode);
		void AddStarForm(const char* form, const std::vector<int>& finalFormCode);

	private:
		bool isRecursive_;
	};
}
#endif

