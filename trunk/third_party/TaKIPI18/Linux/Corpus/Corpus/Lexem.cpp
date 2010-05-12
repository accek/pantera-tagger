#include "../corpuslib_headers.h"

#include "Parser/Parser.h"
#include "tagexception.h"
#include "functions.h"
#include "Value.h"
#include "Lexem.h"
#include "Token.h"
#include "entities.h"

namespace Corpus
{
	//buduje lexem na podstawie parsowania strumienia wejsciowego
	//readOpeningTag -> czy czytac tag otwierajacy
	//disamb -> jesli nie czytac tagu otwierajacego, to ustawic taka wartosc disamb tokenowi
	CLexem::CLexem(std::istream &input,bool readOpeningTag,bool disamb)
	{
		//wczytanie <lex>
		if(readOpeningTag)
			this->disamb = ReadOpeningTag(input);
		else
			this->disamb = disamb;

		std::string readStr;
		//wczytanie base
		if(!(Parser::EatXmlTag(input,"base",readStr).empty()))
			throw tagexception(std::string("Wykryto nieoczekiwane dane przed <base>:\n").append(readStr).append("\n").c_str());
		Parser::EatXmlTag(input,"/base",readStr);
		DecodeEntities(readStr); // readStr will contain base with XML entity references decoded
		baseIdx = CWordDictionary::GetInstance()->Add(readStr);

		try
		{
			//wczytanie ctag
			tag = CTag(input);
		}
		catch(tagexception e)
		{
			tag = CTag(cf_ign);
		}

		std::string gotTag;

		Parser::GetXmlTag(input,gotTag,readStr);

		//zakonczenie </lex>
		if(!gotTag.compare("count"))
		{
			Parser::GetXmlTag(input,gotTag,readStr);
			probability = atof(readStr.c_str());
			if (!Parser::EatXmlTag(input,"/lex",readStr).empty())
				throw tagexception(std::string("Wykryto nieoczekiwane dane przed </lex>:\n").append(readStr).append("\n").c_str());
		}
		else if (gotTag.compare("/lex"))
		{
			throw tagexception(std::string("Wykryto nieoczekiwane dane przed </lex>:\n").append(readStr).append("\n").c_str());
		}
		else
		{
			probability = disamb ? 1 : 0;
		}

		RefCount = 1;
	}
	CLexem::CLexem(std::string & base, std::string & tok,bool disamb)
	{
		this->disamb = disamb;
		baseIdx = CWordDictionary::GetInstance()->Add(base);
		try
		{
			//wczytanie ctag
			tag = CTag(tok);
		}
		catch(tagexception e)
		{
			tag = CTag(cf_ign);
		}
		RefCount = 1;
	
	
	};
	CLexem::CLexem(int & baseId, std::string & tok,bool disamb)
	{
		this->disamb = disamb;
		baseIdx = baseId;
		try
		{
			//wczytanie ctag
			tag = CTag(tok);
		}
		catch(tagexception e)
		{
			tag = CTag(cf_ign);
		}
		RefCount = 1;
	
	
	};
	CLexem* CLexem::CreateHardCopy()
	{
		return new CLexem(*this);
	}

	//wczytuje tag otwierajacy <lex> i zwraca wartosc atrybutu disamb
	//czyli true jestli disamb="1" albo false, jesli brak disasm albo disamb="0"
	//readTag - jesli == NULL, rzucaj wyjatkami jesli odczyta sie co innego niz <lex>
	//a jesli != NULL, to zapisz tam wartosc wczytanego tagu
	//chyba ze odczytano lex, a nie rozpoznano atrybutow, wtedy tak czy siak rzuc wyjatkiem
	STATIC bool CLexem::ReadOpeningTag(std::istream &input, std::string * readTag)
	{
		std::string readStr;
		std::string lexTag;
		bool dis;
		Parser::GetXmlTag(input,lexTag,readStr);
		if(!readStr.empty())
			throw tagexception(std::string("Wykryto nieoczekiwane dane przed <").append(lexTag).append(">:\n").append(readStr).append("\n").c_str());

		if(lexTag.size() > 3)
		{
			if(strnicmp(lexTag.data(),"lex ",4))
			{
				if(readTag)
				{
					(*readTag).assign(lexTag);
					return false;
				}
				throw tagexception(std::string("Oczekiwano tagu <lex>, znaleziono tag <").append(lexTag).append(">").c_str());
			}
			size_t attribEnd = lexTag.find("disamb=\"1\"");
			if(!(dis = (attribEnd != std::string::npos)))
				attribEnd = lexTag.find("disamb=\"0\"");
			if(attribEnd == std::string::npos)
				throw tagexception(std::string("Nierozpoznane dane o atrybutach dla tagu <lex>! Oczekiwano disamb=\"[0|1]\",\na odczytano:\n").append(lexTag).c_str());
			if(readTag)
				(*readTag).assign("lex");
			return dis;
		}
		else
		{
			if(readTag)
				(*readTag).assign(lexTag);
			else
			{
				if(strnicmp(lexTag.data(),"lex",3))
					throw tagexception(std::string("Oczekiwano tagu <lex>, znaleziono tag <").append(lexTag).append(">").c_str());
			}
			return false;
		}
	}

	CLexem::CLexem(const CLexem& l)
	{
		//std::cout  <<"doing copy ";
		this->baseIdx = l.baseIdx;
		this->disamb = l.disamb;
		this->probability = l.probability;
		this->tag = l.tag;
		this->RefCount = 1;
	}

	const CLexem& CLexem::operator=(const CLexem& l)
	{
		//std::cout<<"initialize";
		this->baseIdx = l.baseIdx;
		this->disamb = l.disamb;
		this->probability = l.probability;
		this->tag = l.tag;
		this->RefCount = 1;

		return *this;
	}

	const bool CLexem::operator ==(const CLexem& l)
	{
		if (this->baseIdx == l.baseIdx
			&& this->tag.Categories == l.tag.Categories
			&& this->tag.FlexClass == l.tag.FlexClass)
			return true;
		return false;
	}

	//s³u¿y do tworzenia pustego lexemu
	CLexem::CLexem()
	{
		disamb = false;
		probability = 0.0;
		tag = CTag();
		RefCount = 1;
		this->baseIdx = -1;
	}

	CLexem::~CLexem(void)
	{

	}

	const DLONG CLexem::operator [](int attribIndex) const
	{
		switch(attribIndex)
		{
			case CToken::base:
				// Part of speech
				return baseIdx;
			case CToken::flex:
				// Part of speech
				return tag.GetFlex();
			case CToken::nmb:
				// Number
				return tag.GetNmb();
			case CToken::cas:
				// Case
				return tag.GetCas();
			case CToken::gnd:
				// Gender
				return tag.GetGnd();
			case CToken::per:
				// Person
				return tag.GetPer();
			case CToken::deg:
				// Degree
				return tag.GetDeg();
			case CToken::asp:
				// Aspect
				return tag.GetAsp();
			case CToken::neg:
				// Negation
				return tag.GetNeg();
			case CToken::acm:
				// Accommodability
				return tag.GetAcm();
			case CToken::acn:
				// Accentability
				return tag.GetAcn();
			case CToken::ppr:
				// Post - prepositionality
				return tag.GetPpr();
			case CToken::agg:
				// Agglutination
				return tag.GetAgg();
			case CToken::vcl:
				// Vocalicity
				return tag.GetVcl();
			case CToken::inp:
				// Vocalicity
				return tag.GetInp();
			case CToken::tnt:
				// Number literal type
				return tag.GetTnt();
			default:
				throw tagexception("Invalid attribute index.");
		}
	}

	void CLexem::Serialize(std::ofstream& file)
	{
		file.write((char*)&baseIdx,sizeof(baseIdx));
		file.write((char*)&tag.FlexClass,sizeof(tag.FlexClass));
		file.write((char*)&tag.Categories,sizeof(tag.Categories));
		file.write((char*)&disamb,sizeof(disamb));
		file.write((char*)&probability,sizeof(probability));
	}
	
	void CLexem::SerializeAsText(std::ostream& file)
	{
		file << baseIdx << std::endl;
		file << tag.ToString() << std::endl;
		file << (disamb ? "1" : "0") << std::endl;
		file << probability << std::endl;
	}

	CLexem* CLexem::Deserialize(std::ifstream& file)
	{
		CLexem* l = new CLexem();

		file.read((char*)&(l->baseIdx),sizeof(l->baseIdx));
		file.read((char*)&(l->tag.FlexClass),sizeof(l->tag.FlexClass));
		file.read((char*)&(l->tag.Categories),sizeof(l->tag.Categories));
		file.read((char*)&(l->disamb),sizeof(l->disamb));
		file.read((char*)&(l->probability),sizeof(l->probability));

		return l;
	}
	
	CLexem* CLexem::DeserializeFromText(std::istream& file)
	{
		CLexem *l = new CLexem();
		file >> l->baseIdx;
		std::string contentLine;
		std::getline(file, contentLine);
		if(contentLine.empty()) std::getline(file, contentLine);
		l->tag.ParseContent(contentLine);
		int disambInt = 0;
		file >> disambInt;
		l->disamb = (disambInt == 1);
		file >> l->probability;
		return l;
	}

	void CLexem::GetMaskedValue(CValue mask, CValue& out) const
	{
		out.first = this->tag.FlexClass & mask.first;
		out.second = this->tag.Categories & mask.second;
	}

	std::string CLexem::ToXmlString(bool writeProbs)
	{
		std::stringstream ss;

		if(disamb)
			ss << "<lex disamb=\"1\"><base>";
		else
			ss << "<lex><base>";

		std::string cx_base(CWordDictionary::GetInstance()->GetAt(baseIdx));
		EncodeEntities(cx_base);
		ss << cx_base << "</base><ctag>"
			<< tag.ToString() << "</ctag></lex>";

		if(writeProbs)
			ss << " " << this->probability;

		return ss.str();
	}
	
	void CLexem::PostprocessSubstGer()
	{
		if( (tag.FlexClass & (cf_ger | cf_subst)) != 0)
		{
			if( (tag.Categories & gcm_negacja) != 0)
				tag.FlexClass = cf_ger;
			else
				tag.FlexClass = cf_subst;
		}
	}
}

