#include "../../corpuslib_headers.h"
#include "SentenceContext.h"

namespace Corpus
{
	namespace Context
	{

		SentenceContext::SentenceContext(Corpus::Documents::Document* doc)
			:Context(doc)
		{
			m_sentence = NULL;
			m_iSentence = 0;
		}

		SentenceContext::~SentenceContext()
		{
			if(m_sentence != NULL)
			{
				delete m_sentence;
				m_sentence = NULL;
			}
		}

		bool SentenceContext::FirstPosition()
		{
			document->Close();
			document->Open();

			return NextPosition();
		}

		bool SentenceContext::NextPosition()
		{
			//std::cout << "Kasuje poprzednie...\n";
			if(m_sentence != NULL)
			{
				delete m_sentence;
				m_sentence= NULL;
			}
			//std::cout << "Nastepne zdanie...\n";
			m_sentence = document->NextSentence();
			//std::cout << "Pobralem zdanie...\n";

			if(m_sentence == NULL)
				return false;

			m_iSentence++;

			return true;
		}

	// 	bool SentenceContext::PierwszaPozycjaFiltrowana()
	// 	{
	// 		if(!PierwszaPozycja())
	// 			return false;
	//
	// 		do
	// 		{
	// 			for(int i = 0; i < m_zdanie->Rozmiar(); i++)
	// 			{
	// 				if(m_zdanie->operator [](i)->CzyFiltrowany())
	// 				{
	// 					if(m_obecneZdanie)
	// 						delete m_obecneZdanie;
	// 					m_obecneZdanie = new CSentence(*m_zdanie);
	// 					return true;
	// 				}
	// 			}
	// 		}while(NastepnaPozycja());
	//
	// 		return false;
	// 	}
	//
	// 	bool SentenceContext::NastepnaPozycjaFiltrowana()
	// 	{
	// 		while(NastepnaPozycja())
	// 		{
	// 			for(int i = 0; i < m_zdanie->Rozmiar(); i++)
	// 			{
	// 				if(m_zdanie->operator [](i)->CzyFiltrowany())
	// 				{
	// 					if(m_obecneZdanie)
	// 						delete m_obecneZdanie;
	// 					m_obecneZdanie = new CSentence(*m_zdanie);
	// 					return true;
	// 				}
	// 			}
	// 		}
	//
	// 		return false;
	// 	}

		CToken* SentenceContext::operator[](int n)
		{
			if(n > Size())
				return NULL;

			if( !m_sentence)
				return NULL;

			m_sentence->Goto(n);
			return m_sentence->operator [](n);
		}
	}
}
