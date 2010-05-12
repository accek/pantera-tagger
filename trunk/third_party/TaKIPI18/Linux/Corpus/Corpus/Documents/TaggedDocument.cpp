#include "../../corpuslib_headers.h"
#include "TaggedDocument.h"

namespace Corpus
{
	namespace Documents
	{

		TaggedDocument::TaggedDocument(std::string filename, bool dissambOnly )
			:Document(filename)
		{
			m_bDissambOnly = dissambOnly;
		}

		TaggedDocument::~TaggedDocument(void)
		{
		}

		void TaggedDocument::Open() throw (std::string)
		{
			if(file != NULL && file->is_open())
				Close();

			file = new std::ifstream();
			file->open(this->filename.c_str());

			if(!file->is_open())
				throw std::string("Nie znaleziono pliku: ").append(filename);

			this->reader = new Corpus::Readers::CCorporaReader(*file, m_bDissambOnly);
			this->sentencer_ = new CSentencer(*reader);
		}

		CToken* TaggedDocument::NextToken()
		{
			if(reader == 0)
				return 0;

			return reader->GetToken();
		}

		CSentence* TaggedDocument::NextSentence()
		{
			if(reader == 0)
				return 0;

			return sentencer_->GetSentence();
		}
	}
}
