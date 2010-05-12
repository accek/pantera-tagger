#ifndef INC_ANTLRParser_hpp_
#define INC_ANTLRParser_hpp_


#include "../../joskipi_headers.h"
#include "../../all_operators.h"
#include "../../all_rules.h"
#include "../../all_patterns.h"

#include <antlr/config.hpp>
/* $ANTLR 2.7.6 (20071205): "joskipi.g" -> "ANTLRParser.hpp"$ */
#include <antlr/TokenStream.hpp>
#include <antlr/TokenBuffer.hpp>
#include "ANTLRParserTokenTypes.hpp"
#include <antlr/LLkParser.hpp>

ANTLR_BEGIN_NAMESPACE(JOSKIPI)
ANTLR_BEGIN_NAMESPACE(Parsers)
ANTLR_BEGIN_NAMESPACE(antlrgen)

ANTLR_USING_NAMESPACE(std)
ANTLR_USING_NAMESPACE(antlr)
using namespace Operators;
using namespace Rules;
using namespace Patterns;
using namespace Corpus;

class CUSTOM_API ANTLRParser : public ANTLR_USE_NAMESPACE(antlr)LLkParser, public ANTLRParserTokenTypes
{

private:
	JOSKIPI::Operators::COperator* m_Result;
	std::map<std::string,pos_ptr> m_mPositions;
	bool ruleDumpMode;

	CValue CreateOrValue(vector<CValue>& vals)
	{
		CValue logor(0,0);
		for(size_t i = 0; i < vals.size(); i++)
		{
			logor.first |= vals[i].first;
			logor.second |= vals[i].second;
		}
		return logor;
	}

	CConstValue* CreateConstValue(vector<CValue>& vals)
	{
		CConstValue* constval = new CConstValue();
		//TODO: brzydko!
		if(vals[0].second & str_msk)
		{
			for(size_t i = 0; i < vals.size(); i++)
				constval->AddValue(vals[i]);
		}
		else
			constval->AddValue(CreateOrValue(vals));
		return constval ;
	}
public:
	void Initalize()
	{
		m_mPositions.clear();
		ruleDumpMode = false;
	}

	void SwitchRuleDumpMode(bool value)
	{
		ruleDumpMode = value;
	}
public:
	void initializeASTFactory( ANTLR_USE_NAMESPACE(antlr)ASTFactory& factory );
protected:
	ANTLRParser(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf, int k);
public:
	ANTLRParser(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf);
protected:
	ANTLRParser(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer, int k);
public:
	ANTLRParser(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer);
	ANTLRParser(const ANTLR_USE_NAMESPACE(antlr)ParserSharedInputState& state);
	int getNumTokens() const
	{
		return ANTLRParser::NUM_TOKENS;
	}
	const char* getTokenName( int type ) const
	{
		if( type > getNumTokens() ) return 0;
		return ANTLRParser::tokenNames[type];
	}
	const char* const* getTokenNames() const
	{
		return ANTLRParser::tokenNames;
	}
	public: void wzorce(
		map<CValue,CPattern*>* patterns
	);
	public: void makro(
		map<string,vector<COperator*>*>* preProcessor
	);
	public: void wzorzec(
		map<CValue,CPattern*>* patterns, map<string,vector<COperator*>*>* macros
	);
	public: void zbior_operatorow(
		vector<COperator*>* schema
	);
	public: COperator*  operator_joskipi();
	public: void maska_klas(
		CValue& mask
	);
	public: void maska_kategorii(
		CValue& mask
	);
	public: CPattern*  operatory_makra(
		map<string,vector<COperator*>*>* macros, CValue& mask
	);
	public: void klasa(
		CValue& mask
	);
	public: void klasagram();
	public: void kategoria(
		CValue& mask
	);
	public: void kategoriagram();
	public: void operator_makro(
		CPattern* patt, map<string,vector<COperator*>*>* macros
	);
	public: void reguly(
		CRuleSet* rs
	);
	public: void regula(
		CRuleSet* rules
	);
	public: CCondition*  testop();
	public: COperator * operator_warunkowy();
	public: COperator * operator_bezwarunkowy();
	public: COperator*  elementwektora();
	public: COperator*  prosty_operator();
	public: COperator*  tekstowy_operator();
	public: string  prosty_tekstowy_operator();
	public: pos_ptr  pozycja();
	public: COperator*  prefix_tekstowy_operator();
	public: COperator*  suffix_tekstowy_operator();
	public: pos_ptr  num();
	public: void atrybut();
	public: void wartosc(
		vector<CValue>& vals
	);
	public: CCondition * prosty_test();
	public: CBinaryComplexC * binarny_test();
	public: CCondition*  zlozony_test();
	public: void sekwencja_test(
		vector<CCondition*>* conditions
	);
	public: pos_ptr  zmienna();
	public: void prosty_arg(
		std::vector<CCondElem*>* cond
	);
	public: void prosty_arg_krotki(
		std::vector<CCondElem*>* cond
	);
	public: CValue  prosta_wartosc();
	public: void wartosci(
		vector<CValue>& vals
	);
	public: void rzeczownik();
	public: void liczebnik();
	public: void przymiotnik();
	public: void zaimek();
	public: void czasownik();
	public: void liczba();
	public: void przypadek();
	public: void rodzaj();
	public: void osoba();
	public: void stopien();
	public: void aspekt();
	public: void zanegowanie();
	public: void akcentowosc();
	public: void poprzyimkowosc();
	public: void akomodacyjnosc();
	public: void aglutynacyjnosc();
	public: void wokalicznosc();
	public: void typliczby();
public:
	ANTLR_USE_NAMESPACE(antlr)RefAST getAST()
	{
		return returnAST;
	}
	
protected:
	ANTLR_USE_NAMESPACE(antlr)RefAST returnAST;
private:
	static const char* tokenNames[];
#ifndef NO_STATIC_CONSTS
	static const int NUM_TOKENS = 141;
#else
	enum {
		NUM_TOKENS = 141
	};
#endif
	
	static const unsigned long _tokenSet_0_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_0;
	static const unsigned long _tokenSet_1_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_1;
	static const unsigned long _tokenSet_2_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_2;
	static const unsigned long _tokenSet_3_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_3;
};

ANTLR_END_NAMESPACE
ANTLR_END_NAMESPACE
ANTLR_END_NAMESPACE
#endif /*INC_ANTLRParser_hpp_*/
