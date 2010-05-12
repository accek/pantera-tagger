#include "../../corpuslib_headers.h"
#include "TextDocument.h"

namespace Corpus
{
	namespace Documents
	{
		/**
		\todo TODO: tutaj jest wyciek pamieci - transduktor nigdy nie jest zwalniany
		*/
		CTransducer* TextDocument::transducer = NULL;

		TextDocument::TextDocument(std::string filename,  bool xml)
			:Document(filename)
		{
			if(TextDocument::transducer == NULL)
				TextDocument::transducer = CTransducer::ParseFromFile("Grammar.txt");
			this->isXml = xml;
			file = NULL;
		}

		TextDocument::~TextDocument(void)
		{

		}

		void TextDocument::Open() throw (std::string)
		{
			if(file != NULL && file->is_open())
				Close();

			file = new std::ifstream();
			file->open(filename.c_str());

			if(!file->is_open())
				throw std::string("Brak dokumentu: ").append(filename);

			reader = new Corpus::Readers::CPlainTextReader(*file,TextDocument::transducer);
			this->sentencer_ = new CSentencer(*reader);
		}

		CToken* TextDocument::NextToken()
		{
			if(reader == NULL)
				return NULL;

			CToken* t = reader->GetToken();

			return t;
		}

		CSentence* TextDocument::NextSentence()
		{
			if(reader == NULL)
				return NULL;

			return sentencer_->GetSentence();
		}
	}
}

