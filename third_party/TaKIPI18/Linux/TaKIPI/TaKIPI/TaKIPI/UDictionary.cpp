#include "../takipi_headers.h"
#include "UDictionary.h"
#include "Config.h"

namespace TaKIPI
{

	CUDictionary::CUDictionary(void)
	{
		dictionary = new std::map< int, std::vector< std::pair<int, Corpus::CToken *> > >();
	}

	CUDictionary::~CUDictionary(void)
	{
		if(dictionary)
		{
			for(std::map< int, std::vector< std::pair<int, Corpus::CToken *> > >::iterator dictIter = dictionary->begin();
				dictIter != dictionary->end(); dictIter++)
			{
				// len to liczba wpis�w w ramach jednego s�owa (second to wektor wpis�w postaci <liczba leksem�w, CToken *>)
				int len = (int)dictIter->second.size();
				for(int pos = 0; pos < len; pos++)
				{
					Corpus::CToken* token = (dictIter->second)[pos].second;
					if( (--token->RefCount) == 0)
						delete token;
				}
			}
			delete dictionary;
		}
	}

	bool CUDictionary::Add(Corpus::CToken* token)
	{
		std::string utf8_lower = token->GetOrth();
		//transform(s.begin(),s.end(),s.begin(),(int(*)(int))tolower);
		Corpus::Parser::ToLowerUTF8(utf8_lower);

		int wordId = Corpus::CWordDictionary::GetInstance()->Add(utf8_lower);
		int lexCount = (int)token->GetLexCount();

		std::map< int, std::vector< std::pair<int, Corpus::CToken *> > >::iterator found = dictionary->find(wordId);
		if(found == dictionary->end())
		{
			// je�li w og�le nie ma tego s�owa, dodajemy jednoelementowy wektor
			SetLexemCount(token);
			std::vector< std::pair<int, Corpus::CToken *> > oneItemVect;
			oneItemVect.push_back(std::pair<int, Corpus::CToken *>(lexCount, token));
			(*dictionary)[wordId] = oneItemVect;
			return false; // nie by�o takiego s�owa
		}
		else
		{
			// sprawdzamy found->second, tj. wektor par <liczba leksem�w, CToken *> w poszukiwaniu naszego lexCount
			int vectIdx;
			int len = (int)found->second.size();
			for(vectIdx = 0; vectIdx < len; vectIdx++)
			{
				if((found->second)[vectIdx].first == lexCount)
					break;
			}
			if(vectIdx < len)
			{
				// ju� jest na pozycji vectIdx, tylko zwi�kszamy liczno�� leksemu rozstrzygaj�cego
				Corpus::CToken* dictToken = (found->second)[vectIdx].second;
				for(int i = 0; i < (int)token->GetLexCount(); i++)
				{
					if(token->GetLexem(i)->GetDisamb())
					{
						dictToken->GetLexem(i)->probability++;
					}
				}
	
				return true; // s�owo ju� wyst�pi�o, tylko zwi�kszyli�my liczno��
			}
			else
			{
				// s�owo co prawda wyst�pi�o w s�owniku, ale z inn� liczb� leksem�w;
				// przygotowujemy token i dodajemy do wektora dw�jk� <lexCount, token>
				SetLexemCount(token);
				found->second.push_back(std::pair<int, Corpus::CToken *>(lexCount, token));
				return false; // takiego s�owa w takim kontek�cie nie by�o
			}
		}
	}
	
	void CUDictionary::SetLexemCount(Corpus::CToken *token)
	{
		for(int i = 0; i < (int)token->GetLexCount(); i++)
		{
			if(token->GetLexem(i)->GetDisamb())
			{
				token->GetLexem(i)->probability = 1;
				token->GetLexem(i)->SetDisamb(false);
			}
			else
				token->GetLexem(i)->probability = 0;
		}
	}

	Corpus::CToken* CUDictionary::GetExactToken(const std::string &word, int lexCount)
	{
		int wordId = Corpus::CWordDictionary::GetInstance()->Add(word);
		std::map< int, std::vector< std::pair<int, Corpus::CToken *> > >::iterator found = dictionary->find(wordId);
		if(found == dictionary->end())
			return NULL;
		
		int pos, len = (int)found->second.size();
		for(pos = 0; pos < len; pos++)
		{
			if(found->second[pos].first == lexCount)
				break;
		}
		if(pos < len)
			return found->second[pos].second;
		
		return NULL;
	}
	
	Corpus::CToken* CUDictionary::GetToken(const Corpus::CToken *refToken)
	{
		std::string utf8_lower = refToken->GetOrth();
		//transform(lower.begin(),lower.end(),lower.begin(),(int(*)(int))tolower);
		Corpus::Parser::ToLowerUTF8(utf8_lower);
		int wordId = Corpus::CWordDictionary::GetInstance()->Add(utf8_lower);
		std::map< int, std::vector< std::pair<int, Corpus::CToken *> > >::iterator found = dictionary->find(wordId);
		if(found == dictionary->end())
		{
			// w s�owniku nie ma w og�le takiego s�owa
			return NULL;
		}
		
		int len = (int)found->second.size();
		if(len < 1) throw Corpus::tagexception("UDictionary corrupted: an entry with no tokens");
		if(len == 1)
		{
			// typowa sytuacja: w s�owniku pod naszym has�em jest tylko jeden wpis;
			// zwracamy token nie przejmuj�c si� liczno�ci� leksem�w (i tak nie mamy innego)
			return found->second[0].second;
		}
		else
		{
			// mamy wi�cej pozycji do wyboru, dobieramy token o najlepszym pokryciu
			int best = 0;
			double best_measure = 0.0;
			for(int pos = 0; pos < len; pos++)
			{
				Corpus::CToken *atPos = found->second[pos].second;
				double meas = CompareTokens(atPos, refToken);
				if(meas > best_measure)
				{
					best_measure = meas;
					best = pos;
				}
			}
			return found->second[best].second;
		}
	}

	void CUDictionary::Serialize(std::ofstream& file)
	{
		int dictionarySize = (int)dictionary->size(); // liczba s��w w s�owniku
		file.write((char*)&dictionarySize,sizeof(dictionarySize));

		for(std::map< int, std::vector< std::pair<int, Corpus::CToken *> > >::iterator iter = dictionary->begin(); iter != dictionary->end(); iter++)
		{
			file.write((char*)&iter->first, sizeof(int)); // id s�owa
			
			int len = (int)iter->second.size();
			file.write((char*)&len, sizeof(int)); // d�ugo�� wektora
			for(int pos = 0; pos < len; pos++)
			{
				// liczba leksem�w
				int lexCount = (iter->second)[pos].first;
				file.write((char*)&lexCount, sizeof(int)); // id s�owa
				// token
				Corpus::CToken* token = (iter->second)[pos].second;
				token->Serialize(file);
			}
		}
	}

	CUDictionary* CUDictionary::Deserialize(std::ifstream& file)
	{
		CUDictionary* d = new CUDictionary();

		int dictionarySize = 0;
		file.read((char*)&dictionarySize,sizeof(dictionarySize));

		int step = dictionarySize/5;

		for(int i = 0; i < dictionarySize; i++)
		{
			std::vector< std::pair<int, Corpus::CToken *> > entries;
			int wordIdx = 0, len = 0;
			file.read((char*)&wordIdx,sizeof(int)); // id s�owa
			file.read((char*)&len,sizeof(int)); // d�ugo�� wektora
			for(int pos = 0; pos < len; pos++)
			{
				int lexCount = 0;
				file.read((char*)&lexCount,sizeof(int)); // d�ugo�� wektora
				Corpus::CToken* token = Corpus::CToken::Deserialize(file);
				entries.push_back(std::pair<int, Corpus::CToken *>(lexCount, token));
			}
			d->dictionary->insert(std::pair< int, std::vector< std::pair<int, Corpus::CToken *> > >(wordIdx, entries)); 
			if(i%step == 0)
				std::cout << ".";
		}
		std::cout << "OK (" <<d->dictionary->size() << " pozycji)\n";
		return d;
	}
	
	void CUDictionary::SerializeAsText(std::ostream& file)
	{
		int dictionarySize = (int)dictionary->size(); // liczba s��w w s�owniku
		file << dictionarySize << std::endl;

		for(std::map< int, std::vector< std::pair<int, Corpus::CToken *> > >::iterator iter = dictionary->begin(); iter != dictionary->end(); iter++)
		{
			file << iter->first << std::endl;
			
			int len = (int)iter->second.size(); // d�ugo�� wektora
			file << len << std::endl;
			for(int pos = 0; pos < len; pos++)
			{
				// liczba leksem�w
				int lexCount = (iter->second)[pos].first;
				file << lexCount << std::endl;
				// token
				Corpus::CToken* token = (iter->second)[pos].second;
				token->SerializeAsText(file);
			}
		}
	}
	
	void CUDictionary::Dump(std::ostream& file)
	{
		int dictionarySize = (int)dictionary->size();
		file << dictionarySize << std::endl;

		for(std::map< int, std::vector< std::pair<int, Corpus::CToken *> > >::iterator iter = dictionary->begin(); iter != dictionary->end(); iter++)
		{
			int len = (int)iter->second.size(); // d�ugo�� wektora
			for(int pos = 0; pos < len; pos++)
			{
				file << Corpus::CWordDictionary::GetInstance()->GetAt(iter->first)
						<< "\t" << iter->second[pos].first // liczba leksem�w
						<< "\n";
			}
		}
	}
	
	
	CUDictionary *CUDictionary::DeserializeFromText(std::istream &file)
	{
		CUDictionary* d = new CUDictionary();
		int dictionarySize = 0;
		file >> dictionarySize;

		int step = dictionarySize/5;

		for(int i = 0; i < dictionarySize; i++)
		{
			std::vector< std::pair<int, Corpus::CToken *> > entries;
			int wordIdx = 0, len = 0;
			file >> wordIdx;
			file >> len;
			for(int pos = 0; pos < len; pos++)
			{
				int lexCount = 0;
				file >> lexCount;
				Corpus::CToken* token = Corpus::CToken::DeserializeFromText(file);
				entries.push_back(std::pair<int, Corpus::CToken *>(lexCount, token));
			}
			d->dictionary->insert(std::pair< int, std::vector< std::pair<int, Corpus::CToken *> > >(wordIdx, entries)); 
			if(i%step == 0)
				std::cout << ".";
		}
		std::cout << "OK (" <<d->dictionary->size() << " pozycji)\n";
		return d;
	}
	
	double CUDictionary::CompareTokens(const Corpus::CToken *dictToken, const Corpus::CToken *sentToken)
	{
		int l_dict = (int)dictToken->GetLexCount();
		int l_sent = (int)sentToken->GetLexCount();
		int l_inter = 0;
		
		for(int sent_i = 0; sent_i < l_sent; sent_i++)
		{
			Corpus::CLexem *lex_sent = sentToken->GetLexem(sent_i);
			for(int dict_i = 0; dict_i < l_dict; dict_i++)
			{
				Corpus::CLexem *lex_dict = dictToken->GetLexem(dict_i);
				
				if(lex_dict->GetTag().Categories == lex_sent->GetTag().Categories
					&& lex_dict->GetTag().FlexClass == lex_sent->GetTag().FlexClass)
				{
					l_inter++;
					break;
				}
			}
		}
		
		//double recall = (double)l_inters / (double)l_dict;
		//double precision = (double)l_inters / (double)l_sent;
		//return (2.0 * recall * precision) / (recall + precision);
		return 2.0 * l_inter / (l_dict + l_sent);
	}
}

