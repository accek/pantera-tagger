#include "../corpuslib_headers.h"
#include "WordsDictionary.h"

namespace Corpus
{
	CWordDictionary* CWordDictionary::GetInstance()
	{
		static CWordDictionary instance;
		return &instance;
	}

	CWordDictionary *CWordDictionary::CreateExternalInstance()
	{
		return new CWordDictionary();
	}

	void CWordDictionary::ReleaseExternalInstance(CWordDictionary *extInstance)
	{
		delete extInstance;
	}

	CWordDictionary::CWordDictionary(void)
	{
		stringVect = new STRING_VECTOR();
		string2int = new STRING2INT_MAP();
	}

	CWordDictionary::~CWordDictionary(void)
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

	bool CWordDictionary::Contains(const std::string &s)
	{
		return (string2int->find(s) != string2int->end());
	}

	bool CWordDictionary::Contains(const _TCHAR* s)
	{
		return Contains(std::string((char*)s));
	}


	bool CWordDictionary::Contains(const unsigned int i)
	{
		return stringVect->size() > i;
	}

	const std::string& CWordDictionary::GetAt(unsigned int i)
	{
		if(stringVect->size() > i)
			return (*stringVect)[i];
		else
		{
			char c[100];
			sprintf(c,"%i",i);
			throw tagexception(std::string("W slowniku nie ma wyrazu pod indeksem ").append(c).c_str());
		}
	}
	int CWordDictionary::GetIdx(const std::string &s)
	{
		STRING2INT_MAP::iterator it = string2int->find(s);
		if(it == string2int->end())
			return -1;
		else
			return it->second;
	}

	int CWordDictionary::Add(const std::string &s)
	{
		STRING2INT_MAP::iterator it = string2int->find(s);
		if(it == string2int->end())
		{
			stringVect->push_back(s);
			string2int->insert(STRING_INT_PAIR(s,Count()-1));
			return Count()-1;
		}
		else
			return it->second;
	}

	void CWordDictionary::Serialize(std::ostream& file)
	{
		INTEGER size = (INTEGER)stringVect->size();
		INTEGER len;

		// Najpierw zapisujemy wielkoœæ s³ownika do strumienia.
		file.write((char*)&size, sizeof(INTEGER));

		// Zapisujemy ka¿dy wpis do s³ownika, wa¿na jest kolejnoœæ, by nie pomyliæ indeksów
		for(INTEGER i = 0; i < size; i++)
		{
			len = (INTEGER)((*stringVect)[i].length());
			// najpierw zapisujemy d³ugoœæ ³añcucha znaków.
			file.write((char*)&len, sizeof(INTEGER));
			// nastêpnie sam ³añcuch jako (char*).
			file.write(&(*stringVect)[i][0], len);
		}
	}

	CWordDictionary *CWordDictionary::Deserialize(std::istream& file, bool writeMsg, bool clearDict)
	{
		CWordDictionary *instance = GetInstance();
		if(clearDict)
			instance->Clear();

		INTEGER size,len, step;
		std::string word;

		file.read((char*)&size,sizeof(INTEGER));
		step = size/13;
		if(step == 0)
			step = 1;

		for(int i = 0; i < size; i++)
		{
			file.read((char*)&len, sizeof(INTEGER));
			word.resize(len);
			file.read(&word[0], len);
			word[len] = 0;

			instance->stringVect->push_back(word);
			instance->string2int->insert(STRING_INT_PAIR(word,i));

			if(writeMsg && i%step == 0)
				std::cout << ".";
		}
		if(writeMsg)
			std::cout << "OK (" << instance->Count() << "pozycji)\n";

		return instance;
	}
	
	void CWordDictionary::SerializeAsText(std::ostream& file)
	{
		INTEGER size = (INTEGER)stringVect->size();
		// Zapisujemy ka¿dy wpis do s³ownika, wa¿na jest kolejnoœæ, by nie pomyliæ indeksów
		for(INTEGER i = 0; i < size; i++)
		{
			if((*stringVect)[i].empty())
				std::cerr << "WARNING: an empty string in WordDict\n";
			
			file << (*stringVect)[i] << std::endl;
		}
		std::cerr << "Saved " << size << " forms to text file\n";
	}
	
	CWordDictionary *CWordDictionary::DeserializeFromText(std::istream& file, bool writeMsg, bool clearDict)
	{
		CWordDictionary *instance = GetInstance();
		if(clearDict)
			instance->Clear();

		std::string word;
		INTEGER pos = 0;
		
		while(std::getline(file, word))
		{
			if(word.empty())
				continue;
			
			instance->stringVect->push_back(word);
			instance->string2int->insert(STRING_INT_PAIR(word,pos));
			pos++;
		}
		
		if(writeMsg)
			std::cout << std::cerr << "Read " << pos << " forms from text file\n";

		return instance;
	}

	int CWordDictionary::Count()
	{
		return (int)stringVect->size();
	}

	void CWordDictionary::Clear()
	{
		stringVect->clear();
		string2int->clear();
	}


	void CWordDictionary::Add(CWordDictionary* dict)
	{
		for(STRING_VECTOR::iterator iter = dict->stringVect->begin(); iter != dict->stringVect->end(); iter++)
			Add(*iter);
	}

}
