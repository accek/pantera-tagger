#include "../../corpuslib_headers.h"
#include "XmlReader.h"

namespace Corpus
{
	namespace Readers
	{

		CXmlReader::CXmlReader(std::istream& stream, std::string grammarFileName)
			:CReader(stream)
		{
			this->transducer_ = CTransducer::ParseFromFile(grammarFileName);
			this->reader_ = 0;
			this->isChunkS_ = false;
		}

		CXmlReader::~CXmlReader(void)
		{
			delete transducer_;
			if(reader_ != 0)
				delete reader_;
		}

		CToken* CXmlReader::GetToken()
		{
			CToken* token = 0;

			if(reader_ == 0 || (token = reader_->GetToken()) == 0)
			{
				newTag = true;
				while(!xmlTextBuffor_.eof())
				{
					xmlTextBuffor_.ignore(256);
				}
				xmlTextBuffor_.clear();

				std::string gotTag;
				std::string befTag;

				befTextTags.clear();
				if(!lastXmlTag.empty())
					befTextTags.append(lastXmlTag);

				lastXmlTag.clear();

				do
				{
					try
					{
						Parser::GetXmlTag(this->stream,gotTag,befTag);
					}
					catch(tagexception ex)
					{
						return 0;
					}
					if(gotTag.compare("chunk type=\"s\""))
					{
						if(!gotTag.compare("/chunk") && isChunkS_)
						{
							isChunkS_ = false;
						}
						else
						{
							if(befTag.empty())
							{
								befTextTags.append("<");
								befTextTags.append(gotTag);
								befTextTags.append(">\n");
							}
							else
							{
								lastXmlTag.append("<");
								lastXmlTag.append(gotTag);
								lastXmlTag.append(">\n");
							}
						}
					}
					else
						isChunkS_ = true;
				}
				while(befTag.empty());

				xmlTextBuffor_ << befTag;

				if(reader_ != 0)
					delete reader_;
				reader_ = new CPlainTextReader(xmlTextBuffor_,transducer_);
			}
			else // if(reader_ == 0 || (token = reader_->GetToken()) == 0)
			{
				newTag = false;
				befTextTags.clear();
			}

			if(reader_ == 0)
				return 0;
			if(token == 0)
				return reader_->GetToken();
			else
				return token;
		}
	}
}
