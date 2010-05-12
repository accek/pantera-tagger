#include "../corpuslib_headers.h"
#include "Token.h"
#include "Config.h"
#include "entities.h"


namespace Corpus
{

	//tworzy nowy obiekt, parsujac dane wczytane ze strumienia
	//readOpeningTag - czy czytac tag otwieraj¹cy
	CToken::CToken(std::istream& input,bool readOpeningTag)
	{
		std::string readStr;
		NS = false;
		variantNum = 0;
		
		if(readOpeningTag)
		{
			bool wasTokBeg = false;
			std::string firstTag;
			do
			{
				Parser::GetXmlTag(input, firstTag, readStr);
				if(!input)
					throw tagexception("XML input stream failed while trying to create a token");
				if(firstTag.compare("ns/") == 0)
				{
					NS = true;
				}
				else if(firstTag.compare("tok") == 0)
				{
					wasTokBeg = true;
				}
				/* teraz szukamy az znajdziemy, bylo tak:
				if(firstTag.compare("tok") != 0)
				{
					throw tagexception(std::string("Wykryto nieoczkiwane dane przed <tok>! Odczytano:\n").append(readStr).c_str());
				} */
			}
			while(!wasTokBeg);
		}
		
		//wczytanie orth
		if(!(Parser::EatXmlTag(input,"orth",readStr).empty()))
			throw tagexception(std::string("Wykryto nieoczekiwane dane przed <orth>! Odczytano:\n").append(readStr).c_str());
		Parser::EatXmlTag(input,"/orth",readStr);
		DecodeEntities(readStr); // readStr will contain orth with XML entity references decoded
		orthIdx = CWordDictionary::GetInstance()->Add(readStr);

		//czytanie leksemów
		bool readingLexems = true;
		while(readingLexems)
		{
			bool disamb = CLexem::ReadOpeningTag(input,&readStr);

			if(CConfig::inputType == CConfig::corpora)
				disamb = false;

			if(!readStr.compare("lex"))
			{
				CLexem* lex = new CLexem(input,false,disamb);
				lexems.push_back(lex);
			}/* TODO co te znaczniki mia³y oznaczaæ?
			To nie jest i tak obs³ugiwane, wiêc szkoda porównañ.
			Nie spotka³em ich w ¿adnym korpusie, wiêc pomijam obs³ugê tego -- wysypie siê na dep, corth (A.R.)
			else if(!readStr.compare("dep"))
			{
				std::string temp;
				while(readStr.compare("/dep"))
					Parser::GetXmlTag(input,readStr,temp);
			}
			else if(!readStr.compare("corth"))
			{
				std::string temp;
				while(readStr.compare("/corth"))
					Parser::GetXmlTag(input,readStr,temp);
			} */
			else if(!strnicmp(readStr.data(),"/tok",4) && readStr.size()==4)

					readingLexems = false;
				else
					throw tagexception(std::string("Oczekiwano tagu <").append("/tok").append(">! Odczytano: <").append(readStr).c_str());
		}

		RefCount = 1;
		variantNum = 0;
	}



	STATIC CToken* CToken::CreateBlank()
	{
		return new CToken(true);
	}

	CToken* CToken::CreateCopy()
	{
		CToken* blank = new CToken(false);
		blank->orthIdx = this->orthIdx;
		blank->NS = NS;
		blank->variantNum = variantNum;

		for(int j = 0; j < (int)lexems.size(); j++)
		{
			lexems[j]->RefCount++;
			blank->lexems.push_back(lexems[j]);
		}

		return blank;
	}

	CToken * CToken::CreateHardCopy()
	{
		CToken* blank = new CToken(false);
		blank->orthIdx = this->orthIdx;
		blank->NS = NS;
		blank->variantNum = variantNum;

		for(int j = 0; j < (int)lexems.size(); j++)
		{
			blank->lexems.push_back(lexems[j]->CreateHardCopy());
		}

		return blank;
	}

	STATIC CToken* CToken::CreateBlank(const std::string& orth)
	{
		return new CToken(orth);
	}

	//operator s³u¿y do pobierania z tokenu wartoœci po indexie
	//zwraca zakodowane binarnie wartoœci danego atrybuty dla wszystkich lexemów
	DLONG CToken::operator [](int attribute)
	{
		if (attribute == CToken::orth)
			return orthIdx;

		DLONG values = 0;

		for(std::vector<CLexem*>::iterator i = lexems.begin(); i != lexems.end(); i++)
			values |= (*(*i))[attribute];

		return values;
	}

	//operator s³u¿y do pobierania z tokenu wartoœci po indexie
	//zwraca zakodowane binarnie wartoœci danego atrybuty dla wszystkich leksemów
	CValue CToken::operator [](const CValue& mask)
	{
		CValue returnVal;
		CValue currentVal;

		for(std::vector<CLexem*>::iterator i = lexems.begin(); i != lexems.end(); i++)
		{
			(*(*i)).GetMaskedValue(mask,currentVal);
			returnVal.first |= currentVal.first;
			returnVal.second |= currentVal.second;
		}

		return returnVal;
	}

	// Operator przypisania.
	const CToken& CToken::operator =(const CToken& t)
	{
		this->ClearLexems();
		for(int k = 0; k < (int)t.lexems.size(); k++)
		{
			this->lexems.push_back(t.lexems[k]);
			this->lexems[k]->RefCount++;
		}
		this->orthIdx = t.orthIdx;
		//this->NS;
		this->RefCount = 1;
		this->variantNum = 0;

		return *this;
	}

	CToken::CToken(const CToken& t)
	{
		this->ClearLexems();
		for(int k = 0; k < (int)t.lexems.size(); k++)
		{
			this->lexems.push_back(t.lexems[k]);
			this->lexems[k]->RefCount++;
		}
		this->orthIdx = t.orthIdx;
		//this->NS;
		this->RefCount = 1;
		this->variantNum = 0;
	}

	// Sprawdzamy, czy dla danego atrybutu Token jest jednoznaczny.
	bool CToken::IsDisambAttr(int attribute)
	{
		// TODO: TO jest tylko dla binarnych wartoœci.

		DLONG bin = (*(*lexems.begin()))[attribute];

		for(std::vector<CLexem*>::iterator i = lexems.begin()+1; i < lexems.end(); i++)
			if (bin != (*(*i))[attribute])
				return false;

		return true;
	}
	
	void CToken::PreprocessSubstGerIgn()
	{
		// substGer == if subst-ger ambiguity present
		bool substGer = ((*this)[CValue((cf_subst|cf_ger),0)].first & (cf_subst|cf_ger))
				== (cf_subst|cf_ger);

		for(int lex_num = 0; lex_num < (int)lexems.size(); lex_num++)
		{
			// take care of igns
			CLexem* lex = lexems[lex_num];
			if( (*lex)[CToken::flex] == cf_ign )
			{
				lex->RefCount++;
				this->ClearLexems();
				this->AddLexem(lex);
				lex->SetDisamb(true);
				break;
			}
			// transform subst/ign into subst+ign tags
			if(substGer)
			{
				if((lex->GetTag().FlexClass & (cf_subst|cf_ger)) != 0)
				{
					CTag tag(lex->GetTag());
					tag.FlexClass |= (cf_subst|cf_ger);
					lex->SetTag(tag);
				}
			}
		}
	}
	
	void CToken::PostprocessSubstGer()
	{
		for(int lex_num = 0; lex_num < (int)lexems.size(); lex_num++)
		{
			lexems[lex_num]->PostprocessSubstGer();
		}
	}
	
	void CToken::AddLexem(const std::string &cx_base, const std::string &cx_tag)
	{
		CLexem *lex = new CLexem();
		CTag tag(cx_tag);
		lex->SetTag(tag);
		lex->SeBase(cx_base);
		lexems.push_back(lex);
	}
	
	void CToken::ClearDisambsAndProbs()
	{
		for(int lex_num = 0; lex_num < (int)lexems.size(); lex_num++)
		{
			lexems[lex_num]->SetDisamb(false);
			lexems[lex_num]->probability = 0.0;
		}
	}

	// Sprawdza prawdopodobieñstwo Lexemu
	double CToken::GetLexProbability(int lexem)
	{
		return lexems[lexem]->probability;
	}

	// Zmienia prawdopodobieñstwo Lexemu dodaj¹c logarytm
	void CToken::SetLexProbability(int lexem, double lProbability)
	{
		lexems[lexem]->probability += lProbability;
	}


	CToken::CToken(bool createBlankLex)
	{
		orthIdx = -1;
		if(createBlankLex)
			lexems.push_back(new CLexem());
		RefCount = 1;
		NS = false;
		variantNum = 0;
	}

	CToken::CToken(const std::string& orth)
	{
		this->orthIdx = CWordDictionary::GetInstance()->Add(orth);
		lexems.push_back(new CLexem());
		RefCount = 1;
		NS = false;
		variantNum = 0;
	}

	CToken::~CToken(void)
	{
		for(std::vector<CLexem*>::iterator i = lexems.begin(); i < lexems.end(); i++)
		{
			CLexem* lex = *i;
			if(lex->RefCount == 1)
			{
				//std::cout << "deleting lexem ";
				delete lex;
			}
			else
				lex->RefCount--;
		}
	}

	void CToken::Serialize(std::ofstream& file)
	{
		file.write((char*)&orthIdx,sizeof(orthIdx));

		int lexemsCount = (int)lexems.size();
		file.write((char*)&lexemsCount,sizeof(lexemsCount));

		for(std::vector<CLexem*>::iterator i = lexems.begin(); i != lexems.end(); i++)
		{
			(*i)->Serialize(file);
		}
	}

	CToken* CToken::Deserialize(std::ifstream& file)
	{
		CToken* t = new CToken(false);

		file.read((char*)&t->orthIdx,sizeof(t->orthIdx));

		int lexemsCount;
		file.read((char*)&lexemsCount,sizeof(lexemsCount));

		for(int i = 0; i < lexemsCount; i++)
		{
			CLexem* l = CLexem::Deserialize(file);
			t->lexems.push_back(l);
		//	int a = 0;
		}

		return t;
	}
	
	void CToken::SerializeAsText(std::ostream& file)
	{
		file << orthIdx << std::endl;
		file << (int)lexems.size() << std::endl;
		
		for(std::vector<CLexem*>::iterator i = lexems.begin(); i != lexems.end(); i++)
		{
			(*i)->SerializeAsText(file);
		}
	}
	
	CToken* CToken::DeserializeFromText(std::istream& file)
	{
		CToken* t = new CToken(false);

		file >> t->orthIdx;
		int lexCount;
		file >> lexCount;

		for(int i = 0; i < lexCount; i++)
		{
			CLexem* l = CLexem::DeserializeFromText(file);
			t->lexems.push_back(l);
		}

		return t;
	}

	std::string CToken::ToXmlString(bool writeProbs)
	{
		std::stringstream ss;
		std::string cx_orth(CWordDictionary::GetInstance()->GetAt(orthIdx).substr());
		EncodeEntities(cx_orth); // use XML entity references
		//if(CConfig::morfeuszEncoding == MORFEUSZ_UTF8)
		//{
		//	Parser::Convert(CWordDictionary::GetInstance()->GetAt(orthIdx).substr(),utf);
		//}

		if(NS)
			ss << "<ns/>" << std::endl;
		
		if(writeProbs && variantNum > 0)
		{
			ss << "<tok variant=\"" << (int)variantNum << "\">" << std::endl;
		}
		else
		{
			ss << "<tok>" << std::endl;
		}
		ss << "<orth>" << cx_orth << "</orth>" << std::endl;

		for(int i = 0; i < (int)lexems.size(); i++)
		{
			ss << lexems[i]->ToXmlString(writeProbs) << std::endl;
		}

		ss << "</tok>" << std::endl;

		return ss.str();
	}
	
	void CToken::ConvertEncoding(Parser::Encoding fromEnc, Parser::Encoding toEnc)
	{
		SetOrth(Parser::Convert(GetOrth(), fromEnc, toEnc));
		for(size_t lex_num = 0; lex_num < lexems.size(); lex_num++)
		{
			lexems[lex_num]->SeBase(Parser::Convert(lexems[lex_num]->GetBase(), fromEnc, toEnc));
		}
	}

}


