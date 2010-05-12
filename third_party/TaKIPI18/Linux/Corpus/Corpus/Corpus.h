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

#ifndef CORPUSLIB_CORPUS_H
#define CORPUSLIB_CORPUS_H
#include "../corpuslib_headers.h"

#include "Documents/Document.h"

namespace Corpus
{
	class CORPUS_API CCorpus
	{
	public:
		CCorpus(void);
		~CCorpus(void);

		void AddDocument(Corpus::Documents::Document* d);
		void RemoveDocument(Corpus::Documents::Document* d);
		size_t Size();
		Documents::Document* GetDocument(int i);
		bool GotoFirst();
		bool Next();
		Documents::Document* GetDocument();
		void ClearDocuments();

		void Serialize(std::string filename) throw(std::string);
		void Deserialize(std::string filename) throw(std::string);
		void LoadTaggedDocuments(std::string filename, bool DissambOnly = false, std::string prefix = "", std::string prefix_in="") throw(std::string);

		size_t GetDocumentIndex() const;

	private:
#ifdef WIN32
		template class CORPUS_API std::allocator<Corpus::Documents::Document*>;
		template class CORPUS_API std::vector<Corpus::Documents::Document*, std::allocator<Corpus::Documents::Document*> >;
#endif
		std::vector<Corpus::Documents::Document*> m_documents;
		size_t m_iCurrent;

	};

}
#endif
