#include "../../corpuslib_headers.h"
#include "PlainTextReader.h"
#include "../ResourceManager.h"
#include "../Config.h"

namespace Corpus
{
	namespace Readers
	{

		CPlainTextReader::CPlainTextReader(std::istream& stream, std::string grammarFileName)
			:CReader(stream)
		{
			tokens = new std::queue<CToken*>();
			this->transducer = CTransducer::ParseFromFile(grammarFileName);
//			if(transducer == 0)
//				transducer = new CTransducer();
			externalTransducer = false;
		}

		CPlainTextReader::CPlainTextReader(std::istream& stream, CTransducer* transducer)
			:CReader(stream)
		{
			tokens = new std::queue<CToken*>();
			this->transducer = transducer;
			externalTransducer = true;
			this->forceUtfEncoding = true;
		}

		CPlainTextReader::~CPlainTextReader(void)
		{
			if(!externalTransducer)
				delete transducer;
			if(tokens)
			{
				delete tokens;
				tokens = NULL;
			}
		}

		CCharUTF* CPlainTextReader::ParseCharUTF(unsigned char prefix)
		{
			int bytesToGet = -1;
			if(prefix <= 0xDF)
				bytesToGet = 1;
			else if(prefix <= 0xEF)
				bytesToGet = 2;
			else if(prefix <= 0xF7)
				bytesToGet = 3;
			else if(prefix <= 0xFB)
				bytesToGet = 4;
			else if(prefix <= 0xFD)
				bytesToGet = 5;

			std::vector<unsigned char> utfChars;
			utfChars.push_back(prefix);
			for(int i = 0; i < bytesToGet; i++)
			{
				utfChars.push_back(stream.get());
			}

			return new CCharUTF(utfChars);
		}

		CChar* CPlainTextReader::ReadNextChar()
		{
			if(CConfig::inputEncoding != Parser::utf8)
			{
				return new CCharASCII(stream.get());
			}
			else
			{
				unsigned char tempChar = stream.get();
				if(tempChar > (unsigned char)0xC0 && tempChar != 255)
				{
					return ParseCharUTF(tempChar);
				}
				else
				{
					return new CCharASCII(tempChar);
				}
			}
		}


		CToken* CPlainTextReader::GetToken()
		{
			if(tokens->empty())
			{
				bool NS = false;
				CChar* c = ReadNextChar();

				if(c->IsEOF())
				{
					delete c;
					return 0;
				}

				std::string form = "";

				bool transducerFailed = (transducer == NULL);
				if(!transducerFailed)
					transducer->GoToStart();

				while(c->IsSpace())
				{
					delete c;
					c = ReadNextChar();
				}

				do
				{
					c->AppendToString(form);
					if(!transducerFailed)
					{
						// to jest zle!
						if(!transducer->PerformStep((char) (*c)[0] ))
						{
							transducerFailed = true;
							transducer->GoToStart();
						}
						/*if(!transducerFailed && transducer->IsValidState())
						{
							transducer->GoToStart();
							break;
						}*/
					}
					delete c;
					c = ReadNextChar();
				}
				while(!c->IsEOF() && !c->IsSpace());

				if(!transducerFailed && transducer->IsValidState())
				{
					transducer->GoToStart();
					transducerFailed = true;
				}

				if(c->IsEOF())
					NS = true;

				if(transducerFailed)
				{
					boost::algorithm::trim(form);
					if(c->IsEOF() && form.empty())
					{
						delete c;
						return 0;
					}
					delete c;

					std::vector<std::string> words;
					Parser::Tokenize(form,words);
					Corpus::GuessingSystem::FormsIdentifier *formsIdentifier = Corpus::CResourceManager::GetInstance()->GetFormsIdentifier();

					for(int i = 0; i < (int)words.size(); i++)
					{
						std::vector<CToken*> tempTokens;
						formsIdentifier->Identify(words[i],tempTokens);

						for(int j = 0; j < (int)tempTokens.size(); j++)
						{
							CToken* tempToken = tempTokens[j];
							tokens->push(tempToken->CreateHardCopy());
							formsIdentifier->Free(tempToken);
						}
					}
				}
				else
				{
					delete c;
					std::vector<CToken*> *transTokens = transducer->Transduce(form);
					for(int i = 0; i < (int)transTokens->size(); i++)
						tokens->push((*transTokens)[i]->CreateHardCopy());
					delete transTokens;
				}
				if(!tokens->empty())
		#ifdef WIN32
					if(stream.eof())
						tokens->c.at(tokens->size()-1)->NS = false;
					else
						tokens->c.at(tokens->size()-1)->NS = NS;
		#else
					if(stream.eof())
						tokens->back()->NS = false;
					else
						tokens->back()->NS = NS;
		#endif
				else
					return 0;
			}

			CToken* token = tokens->front();
			tokens->pop();

			return token;
		}
	}
}
