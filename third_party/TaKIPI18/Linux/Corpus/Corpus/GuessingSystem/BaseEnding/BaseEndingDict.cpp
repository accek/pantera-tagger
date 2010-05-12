#include "../../../corpuslib_headers.h"
#include "BaseEndingDict.h"

namespace Corpus
{
	namespace GuessingSystem
	{
		namespace BaseEnding
		{
			BaseEndingDict *BaseEndingDict::GetInstance()
			{
				static BaseEndingDict instance;
				return &instance;
			}

			BaseEndingDict::BaseEndingDict()
			{
				stringVect = new STRING_VECTOR();
				string2int = new STRING2INT_MAP();
			}

			BaseEndingDict::~BaseEndingDict()
			{
				if(stringVect)
				{
					delete stringVect;
					stringVect = NULL;
				}

				if(string2int)
				{
					delete string2int;
					string2int = NULL;
				}
			}

			const std::string& BaseEndingDict::GetAt(unsigned int i)
			{
				if(stringVect->size() > i)
					return (*stringVect)[i];
				else
				{
					char c[100];
					sprintf(c,"%i",i);
					throw tagexception(std::string("W slowniku BaseEndingDict nie ma wyrazu pod indeksem ").append(c).c_str());
				}
			}

			int BaseEndingDict::Add(const char *s)
			{
				std::string form(s);

				STRING2INT_MAP::iterator it = string2int->find(form);
				if(it == string2int->end())
				{
					stringVect->push_back(form);
					string2int->insert(STRING_INT_PAIR(form,Count()-1));
					return Count()-1;
				}
				else
					return it->second;
			}

			BaseEndingDict* BaseEndingDict::Deserialize(std::string filename)
			{
				BaseEndingDict *instance = GetInstance();
				std::ifstream in(filename.c_str(), std::ios::binary);
				if(!in)
					return instance;

				instance->Clear();

				INTEGER size, len;
				std::string word;

				in.read((char*)&size, sizeof(INTEGER));

				for(INTEGER i = 0; i < size; i++)
				{
					in.read((char*)&len, sizeof(INTEGER));
					word.resize(len);
					in.read(&word[0],len);
					word[len] = 0;

					instance->stringVect->push_back(word);
					instance->string2int->insert(STRING_INT_PAIR(word,i));
				}
				in.close();

				return instance;
			}

			
			bool BaseEndingDict::Serialize(std::string filename)
			{
				BaseEndingDict *instance = GetInstance();
				std::ofstream out(filename.c_str(), std::ios::binary);
				if(!out)
					return false;

				INTEGER size = (INTEGER)instance->stringVect->size();
				INTEGER len;

				// Najpierw zapisujemy wielkoœæ s³ownika do strumienia.
				out.write((char*)&size, sizeof(INTEGER));

				// Zapisujemy ka¿dy wpis do s³ownika, wa¿na jest kolejnoœæ, by nie pomyliæ indeksów
				for(INTEGER i = 0; i < size; i++)
				{
					len = (INTEGER)((*(instance->stringVect))[i].length());
					// najpierw zapisujemy d³ugoœæ ³añcucha znaków.
					out.write((char*)&len, sizeof(INTEGER));
					// nastêpnie sam ³añcuch jako (char*).
					out.write(&(*(instance->stringVect))[i][0],len);
				}
				out.close();

				return true;
			}
		}
	}
}
