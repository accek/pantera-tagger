#include "../corpuslib_headers.h"


#include "Corpus.h"
#include "Documents/TaggedDocument.h"

namespace Corpus
{

	CCorpus::CCorpus(void)
	{
		m_iCurrent = 0;
	}

	CCorpus::~CCorpus(void)
	{
		ClearDocuments();
	}

	void CCorpus::AddDocument(Corpus::Documents::Document* d)
	{
		m_documents.push_back(d);
	}
	void CCorpus::RemoveDocument(Corpus::Documents::Document* d)
	{
		std::vector<Corpus::Documents::Document*>::iterator iter = m_documents.begin();
		std::vector<Corpus::Documents::Document*>::iterator end = m_documents.end();

		while( iter != end )
		{
			if( (*iter)->GetFilename() == d->GetFilename() )
			{
				m_documents.erase(iter);
				if(m_iCurrent >= m_documents.size() )
					m_iCurrent--;
				return;
			}
		}
	}
	size_t CCorpus::Size()
	{
		return m_documents.size();
	}
	Corpus::Documents::Document* CCorpus::GetDocument(int i)
	{
		return m_documents[i];
	}
	bool CCorpus::GotoFirst()
	{
		m_iCurrent = 0;
		return m_documents.size() > m_iCurrent;
	}
	bool CCorpus::Next()
	{
		m_iCurrent++;
		return m_documents.size() > m_iCurrent;
	}
	Corpus::Documents::Document* CCorpus::GetDocument()
	{
		return m_documents[m_iCurrent];
	}
	void CCorpus::ClearDocuments()
	{
		while(m_documents.size() > 0 )
		{
			Corpus::Documents::Document * doc = m_documents[m_documents.size()-1];
			m_documents.pop_back();
			delete doc;
		}
	}

	void CCorpus::Serialize(std::string filename) throw(std::string)
	{
		throw std::string("CCorpus::Serialize not implemented");
	}

	void CCorpus::Deserialize(std::string filename) throw(std::string)
	{
		throw std::string("CCorpus::Deserialize not implemented");
	}

	void CCorpus::LoadTaggedDocuments(std::string filename,bool DissambOnly , std::string prefix, std::string prefix_in) throw(std::string)
	{
		std::ifstream file(filename.c_str());
		if(!file)
			throw std::string( "Corpus file not found: " + filename);

		ClearDocuments();

		std::string line;
		static const int BUF_SIZE = 1024;
		char buf[BUF_SIZE];
		while(file)
		{
			file.getline(buf, BUF_SIZE-1);

			line = buf;
			boost::algorithm::trim(line);

			if(line.empty())
				continue;

			if(prefix_in.empty())
				m_documents.push_back(new Documents::TaggedDocument( prefix+line, DissambOnly) );
			else
			{
				if(line.find(prefix_in) == 0)
					m_documents.push_back( new Documents::TaggedDocument( prefix + line.substr( prefix_in.length() ),
					DissambOnly) );
				else
					m_documents.push_back(new Documents::TaggedDocument(line,DissambOnly));
			}

		};
	}

	size_t CCorpus::GetDocumentIndex() const
	{
		return m_iCurrent;
	}
}


