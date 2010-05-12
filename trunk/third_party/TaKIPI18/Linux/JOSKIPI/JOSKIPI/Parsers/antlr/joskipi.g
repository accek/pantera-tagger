header {
ANTLR_USING_NAMESPACE(std)
ANTLR_USING_NAMESPACE(antlr)
using namespace Operators;
using namespace Rules;
using namespace Patterns;
using namespace Corpus;
}

header "pre_include_hpp" {
#include "../../joskipi_headers.h"
#include "../../all_operators.h"
#include "../../all_rules.h"
#include "../../all_patterns.h"
}
header "pre_include_cpp" {
#include "../../joskipi_headers.h"

//#define JOSKIPI_SHOW_ANTLR_DEBUG_MSG

}

options {
language="Cpp";
namespace="JOSKIPI::Parsers::antlrgen";
//namespaceStd="std";
//namespaceAntlr="antlr";     
genHashLines = false;
}
//------------------------------------------------------------------------
//--------------  PARSER   -----------------------------------------------
//------------------------------------------------------------------------
class ANTLRParser extends Parser;
options {
        k = 5;
		defaultErrorHandler=false;
		//codeGenMakeSwitchThreshold = 2;
        //codeGenBitsetTestThreshold = 3;
        buildAST = false;
}
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
}

wzorce [map<CValue,CPattern*>* patterns]
{
	map<string,vector<COperator*>*>* preProcessor = new map<string,vector<COperator*>*>();
}
		: ( makro[preProcessor] )* ( wzorzec[patterns, preProcessor] )+ ;

makro[map<string,vector<COperator*>*>* preProcessor]
{
	vector<COperator*>* schemat = new vector<COperator*>();
}
		: ident : MACROID LPAREN zbior_operatorow[schemat] RPAREN
		{
			string mid = ident->getText();
			(*preProcessor)[mid] = schemat;
		}
		;

zbior_operatorow[vector<COperator*>* schema]
{
	COperator* o = NULL;
}
		: o = operator_joskipi
		{
			schema->push_back(o);
		}
		(
			COMMA o = operator_joskipi
			{
				schema->push_back(o);
			}
		)*
		;

wzorzec[map<CValue,CPattern*>* patterns, map<string,vector<COperator*>*>* macros]
{
	CValue mask(0,0);
	CPattern* pattern = NULL;
}
		: maska_klas[mask] COLON maska_kategorii[mask] HASH LCHEVRON pattern = operatory_makra[macros, mask] RCHEVRON
		{
			(*patterns)[mask] = pattern;
		}
		;

maska_klas[CValue& mask]
		: ( klasa[mask] )* ;

klasa[CValue& mask]
{
	RefToken curr = LT(1);
}
		: klasagram
		{
			TFlexClass flexclass = CTag::GetFlexClass( curr->getText());
			if(flexclass != cf_none)
				mask.first |= flexclass;
			else throw string( string("Unknown flex class: " + curr->getText()).c_str());
		}
		;

maska_kategorii[CValue& mask]
		: ( kategoria[mask] )* ;

kategoria[CValue& mask]
{
	RefToken curr = LT(1);
}
		: kategoriagram 
		{
			DLONG gcat = CTag::GetGrammCatValue(curr->getText());
			if(gcat != gc_none)
				mask.second |= gcat;
			else throw string( string("Unknown gramm category: " + curr->getText()).c_str());
		}
		;

operatory_makra[map<string,vector<COperator*>*>* macros, CValue& mask] returns [CPattern* patt]
{
	vector<COperator*>* schema = new vector<COperator*>();
	patt = new CPattern(schema, mask.first, mask.second);
	COperator* o = NULL;
}
		: operator_makro[patt, macros] ( COMMA operator_makro[patt, macros] )*
		;

operator_makro[CPattern* patt, map<string,vector<COperator*>*>* macros]
{
	COperator* o = NULL;
}
		: o = operator_joskipi
		{
			patt->AddOperator(o);
		}
		| macro : MACROID
		{
			string mid = macro->getText();
			vector<COperator*>* opers = (*macros)[mid];
			patt->AddOperators(opers);
		}
		;

reguly [CRuleSet* rs]
{
}
		: ( regula[rs] )* ;

regula[CRuleSet* rules]
{
	m_mPositions.clear(); // nazwy zmiennych liczymy od nowa
	CCondition* akcja = NULL;
	CCondition* warunek = NULL;
}
		: DELETETOK LPAREN akcja = testop RPAREN HASH i : INT DSEPARATOR warunek = testop
		{
			CRule *new_rule = NULL;
			CAction* new_action = new CAction(akcja);
			int priorytet = atoi(i->getText().c_str());
			if(ruleDumpMode)
			{
				char fileName[50];
				sprintf(fileName, "./Rules/Rule%d.txt", priorytet);
				new_rule = new CRule(warunek, new_action, priorytet, fileName);
			}
			else
			{
				new_rule = new CRule(warunek, new_action, priorytet);
			}
			rules->AddRule(new_rule);
		}
		;

operator_joskipi returns [COperator* o]
{
	m_mPositions.clear(); // nazwy zmiennych liczymy od nowa
	o = NULL;
}
		: o = operator_warunkowy
		| o = operator_bezwarunkowy
		;

operator_warunkowy returns [COperator *o]
{
	o = NULL;
	COperator *op = NULL;
	CCondition *cond = NULL;
} : Q_MARK op = elementwektora Q_MARK cond = testop
		{
			o = new CCondOper(cond, op);
		}
		;

operator_bezwarunkowy returns [COperator *o]
{
	o = NULL;
} : o = elementwektora ;

elementwektora returns [COperator* o]
{
	CCondition *cond = NULL;
	o = NULL;
}
		: o = prosty_operator 
		| o = tekstowy_operator
		| cond = testop
		{
			o = new CTestOper(cond);
		}
		;

tekstowy_operator returns [COperator* oper]
{
	oper = NULL;
	string op_type = "";
	pos_ptr position;
}
		: op_type = prosty_tekstowy_operator  LBRACKET position = pozycja RBRACKET
		{
			oper = new CSimpleOperator(position, Corpus::CTag::GetAttribIndex(op_type));
		}
		| oper = prefix_tekstowy_operator
		| oper = suffix_tekstowy_operator 
		;

prefix_tekstowy_operator returns [COperator* oper]
{
	throw std::string("No implementation for prefix operator!");
	pos_ptr position, n;
	string inner;
}
		: PREFIX LPAREN inner = prosty_tekstowy_operator LBRACKET position = pozycja RBRACKET COMMA n = num RPAREN;

suffix_tekstowy_operator returns [COperator* oper]
{
	throw std::string("No implementation for suffix operator!");
	pos_ptr position, n;
	string inner;
}
		: SUFFIX! LPAREN! inner = prosty_tekstowy_operator LBRACKET position = pozycja RBRACKET COMMA n = num RPAREN;

prosty_tekstowy_operator returns [string s]
{
	s = "";
}
		: ORTH { s = "orth"; }
		| BASE { s = "base"; }
		;

prosty_operator returns [COperator* oper]
{
	oper = NULL;
	pos_ptr position, pos2;
	string attr_type = "";
	vector<CValue> vals1, vals2;
	CConstValue *constVal = NULL, *constVal2 = NULL;
	RefToken curr = LT(1);
}
				: FLEX LBRACKET position = pozycja RBRACKET
				{
					oper = new CSimpleOperator(position, CTag::GetAttribIndex("flex"));
				}
				| atrybut
					{
						attr_type = curr->getText();
					}
					LBRACKET position = pozycja RBRACKET
					{
						oper = new CSimpleOperator(position, CTag::GetAttribIndex(attr_type));
					}
				| CATFLT LPAREN position = pozycja COMMA wartosc[vals1] COMMA wartosc[vals2] RPAREN
				{
					constVal = CreateConstValue(vals1);
					CValue v = CreateOrValue(vals2);
					constVal->AddValue(v);

					oper = new CCatFlt(position, constVal);
				}
				| AGRFLT LPAREN position = pozycja COMMA pos2 = pozycja COMMA wartosc[vals1] COMMA bits_f : INT COMMA wartosc[vals2] RPAREN
				{
					constVal = CreateConstValue(vals1);
					constVal2 = CreateConstValue(vals2);
					int bits = atoi(bits_f->getText().c_str());

					oper = new CAgrFlt(position, pos2, constVal, bits, constVal2);
				}
				| DISPOS LBRACKET position = pozycja RBRACKET
				{
					oper = new CDisPosOper(position);
				}
				| DISCAT LBRACKET position = pozycja RBRACKET
				{
					oper = new CDisCatOper(position);
				}
				;

testop returns [CCondition* cond]
{
	cond = NULL;
}
		: cond = prosty_test 
		| cond = binarny_test
		| cond = zlozony_test
		;

zlozony_test returns [CCondition* cond]
{
	//pamiec wektora zwlania CComplexCondition
	cond = NULL;
	std::vector<CCondition*>* conditions = new std::vector<CCondition*>();
}
	: AND LPAREN sekwencja_test[conditions] RPAREN
	{
		cond = new CAndCC(conditions);
	}
	| OR LPAREN sekwencja_test[conditions] RPAREN
	{
		cond = new COrCC(conditions);
	}
	| NOT LPAREN sekwencja_test[conditions] RPAREN
	{
		cond = new CNotCC(conditions);
	}
	;

sekwencja_test [vector<CCondition*>* conditions]
{
	CCondition* cond = NULL;
}
		: cond = testop { conditions->push_back(cond); }
			( COMMA cond = testop { conditions->push_back(cond); } )* ;

binarny_test returns [CBinaryComplexC *oper]
{
	oper = NULL;
	pos_ptr pos1, pos2, var;
	CCondition *innerTest= NULL;
	// TODO: kto zwlania wektor?
	vector<pos_ptr> *positions = new vector<pos_ptr>();
	// TODO: kto zwlania wektor?
	vector<CCondition*>* conditions = new vector<CCondition*>();
	// TODO: kto zwlania wektor?
	vector<CValue> vals;
	CConstValue *constval = NULL;
	CValue logor(0,0);
	std::vector<CCondElem*> *cond = NULL;
}
			: LLOOK LPAREN pos1 = pozycja COMMA pos2 = pozycja COMMA var = zmienna COMMA innerTest = testop RPAREN
			{
				positions->push_back(pos1);
				positions->push_back(pos2);
				positions->push_back(var);
				conditions->push_back(innerTest);
				oper = new CLookLeftBinCC(positions,conditions);
			}
			| RLOOK LPAREN pos1 = pozycja COMMA pos2 = pozycja COMMA var = zmienna COMMA innerTest = testop RPAREN
			{
				positions->push_back(pos1);
				positions->push_back(pos2);
				positions->push_back(var);
				conditions->push_back(innerTest);
				oper = new CLookRightBinCC(positions,conditions);
			}
			| ONLY LPAREN pos1 = pozycja COMMA pos2 = pozycja COMMA var = zmienna COMMA innerTest = testop RPAREN
			{
				positions->push_back(pos1);
				positions->push_back(pos2);
				positions->push_back(var);
				conditions->push_back(innerTest);
				oper = new COnlyBinCC(positions,conditions);
			}
			| ATLEAST LPAREN pos1 = pozycja COMMA pos2 = pozycja COMMA var = zmienna COMMA innerTest = testop 
				COMMA i : INT RPAREN
			{
				int num = atoi(i->getText().c_str());
				positions->push_back(pos1);
				positions->push_back(pos2);
				positions->push_back(var);
				conditions->push_back(innerTest);
				oper = new CAtLeastBinCC(positions,conditions, num);
			}
			| AGR LPAREN pos1 = pozycja COMMA pos2 = pozycja COMMA wartosc[vals] COMMA bits_a : INT RPAREN
			{
				int bits = atoi(bits_a->getText().c_str());
				positions->push_back(pos1);
				positions->push_back(pos2);
				//TODO: brzydkie!
				positions->push_back(pos_ptr(new Corpus::CPosition()));
				constval = CreateConstValue(vals);
				
				oper = new CAgrBinCC(positions, constval, bits);
			}
			| WAGR LPAREN pos1 = pozycja COMMA pos2 = pozycja COMMA wartosc[vals] COMMA bits_w : INT RPAREN
			{
				int bits = atoi(bits_w->getText().c_str());
				positions->push_back(pos1);
				positions->push_back(pos2);
				positions->push_back(pos_ptr(new Corpus::CPosition()));
				constval = CreateConstValue(vals);
				oper = new CAgrWeakBinCC(positions, constval, bits);
			}
			| AGRPP LPAREN pos1 = pozycja COMMA pos2 = pozycja COMMA wartosc[vals] COMMA bits_pp : INT RPAREN
			{
				int bits = atoi(bits_pp->getText().c_str());
				positions->push_back(pos1);
				positions->push_back(pos2);
				//TODO: brzydkie!
				positions->push_back(pos_ptr(new Corpus::CPosition()));
				constval = CreateConstValue(vals);

				//std::cout << "agrpp: pos1 = " << pos1->ToString() << ", pos2: " << pos2->ToString()
				//	<<", wartosc = " << constval->ToString() << ", bits: " << bits << std::endl
				
				
				oper = new CAgrPPBinCC(positions, constval, bits);
			}
			;

prosty_test returns [CCondition *oper]
{
	// TODO: kto zwlania wektor?
	std::vector<CCondElem*> *cond = new std::vector<CCondElem*>();
	oper = NULL;
#ifdef JOSKIPI_SHOW_ANTLR_DEBUG_MSG
	std::cout << "prosty_test...enter\n";
#endif
}

			: EQUAL LPAREN prosty_arg[cond] COMMA prosty_arg[cond] RPAREN
			{
				  oper = new CEqualSC(*cond);
			}
			| IN_TEST LPAREN prosty_arg[cond] COMMA prosty_arg[cond] RPAREN
			{
				 oper = new CInSC(*cond);
			}
			| INTER LPAREN prosty_arg[cond] COMMA prosty_arg[cond] RPAREN
			{
#ifdef JOSKIPI_SHOW_ANTLR_DEBUG_MSG
				std::cout << "inter enter\n";
#endif
				oper = new CIntersecSC(*cond);
			}
			| ISBIG LPAREN prosty_arg_krotki[cond] RPAREN
			{
				 oper =new CIsBigSC(*cond);
			}
			| HASNUM LPAREN prosty_arg_krotki[cond] RPAREN
			{
				 oper = new CHasNumSC(*cond);
			}
			;

prosty_arg [std::vector<CCondElem*>* cond]
{
	COperator *oper = NULL;
	vector<CValue> vals;
	CConstValue *constval = NULL;
	CValue logor(0,0);
#ifdef JOSKIPI_SHOW_ANTLR_DEBUG_MSG
	std::cout << "prosty_arg...enter\n";
#endif
}
			: oper = prosty_operator 
			{ 
				cond->push_back(oper);
			}
			| oper = tekstowy_operator 
			{ 
				cond->push_back(oper); 
			}
			| wartosc[vals]
			{
				constval = CreateConstValue(vals);
				cond->push_back(constval);
			}
			;

prosty_arg_krotki [std::vector<CCondElem*>* cond]
{
	COperator *oper = NULL;
	vector<CValue> vals;
	CValue logor(0,0);
	CConstValue *constval = NULL;
}
			: oper = tekstowy_operator { cond->push_back(oper); }
			| wartosc[vals]
			{
				constval = CreateConstValue(vals);
				cond->push_back(constval);
			}
			;

wartosc [vector<CValue>& vals] 
{
	CValue val(0,0);
#ifdef JOSKIPI_SHOW_ANTLR_DEBUG_MSG
	std::cout << "wartosc entered\n";
#endif
}
		: val = prosta_wartosc 
		{ 
			vals.push_back(val); 
		}
		| LCURLY 
			(
			(
			 val = prosta_wartosc 
				{ 
			 		vals.push_back(val); 
				}
			)
			|
			(wartosci[vals])
			)
		 RCURLY

		;

wartosci [vector<CValue>& vals] 
{
	CValue val(0,0);
}
		: val = prosta_wartosc { vals.push_back(val); }
			( COMMA val = prosta_wartosc { vals.push_back(val); } )+;


prosta_wartosc returns [CValue retVal] 
{
	CValue ret(0,0);
#ifdef JOSKIPI_SHOW_ANTLR_DEBUG_MSG
	std::cout << "prosta_wartosc entered... \n";
#endif
	RefToken curr = LT(1);

}
				: kategoriagram 
				{
					DLONG gc = CTag::GetGrammCatValue(curr->getText());
					if(gc != gc_none)
						ret.second |= gc;
					retVal = ret;
				}
				| klasagram 
				{
					TFlexClass flexclass = CTag::GetFlexClass( curr->getText());
					if(flexclass != cf_none)
						retVal.first |= flexclass;
					else throw std::string( string("Unknown flex class: " + curr->getText()).c_str());
					
				}
				| atrybut
				{
					CValue mask;
					CTag::GetAttribMask(CTag::GetAttribIndex(curr->getText()),mask);
					retVal.first |= mask.first;
					retVal.second|= mask.second;
				}
				| NONE
				{
					// std::cerr << "ProstaWartosc = none... \n";
				}
				| d: STRING
				{
					string s = d->getText();
					boost::trim(s);
					boost::erase_all(s, "\"");
					// Corpus::Parser::ConvertToUTFFromCP1250(s,c); no conversion -> JOSKIPI in UTF-8
					retVal = Corpus::CValue(Corpus::CWordDictionary::GetInstance()->Add(s),str_msk);
				}
				;
				
atrybut: 	NMB |CAS|GND|PER|DEG |ASP|ACM|ACN|PPR|AGG|VCL|TNT;


klasagram: rzeczownik | liczebnik | przymiotnik | ADV  | zaimek | SIEBIE | czasownik | WINIEN | PRED | PREP | CONJ | QUB | XXS | XXX | IGN | INTERP | TWF | TNUM | TSYM | TDATE | TTIME | TURI | TMAIL;

rzeczownik: SUBST | DEPR;

liczebnik: NUM | NUMCOL ;

przymiotnik:ADJ | ADJA | ADJP ;

zaimek: PPRON12 | PPRON3 ;

czasownik: FIN | BEDZIE | AGLT | PRAET | IMPT | IMPS | INF | PCON | PANT | GER | PACT | PPAS ;

kategoriagram 
	:	liczba | przypadek | rodzaj | osoba | stopien | aspekt | zanegowanie | akcentowosc | poprzyimkowosc | akomodacyjnosc |
		aglutynacyjnosc | wokalicznosc;

liczba	:	SG|PL;
przypadek
	:	NOM|GEN|DAT|ACC|INST|LOC|VOC;
rodzaj	:	M1|M2|M3|F|N;
osoba	:	PRI|SEC|TER;
stopien	:	POS|COMP|SUP;
aspekt	:	IMPERF|PERF;
zanegowanie
	:	AFF|NEG;
akcentowosc
	:	AKC|NAKC;
poprzyimkowosc
	:	PRAEP|NPRAEP;
akomodacyjnosc
	:	CONGR|REC;
aglutynacyjnosc
	:	NAGL|AGL;
wokalicznosc
	:	WOK|NWOK;
typliczby
	:	INTEGER|FRAC;


pozycja returns [pos_ptr position]
	: position = num | position = zmienna;

num returns [pos_ptr position]
		: BEGIN { position = pos_ptr(new Corpus::CPosition(CPosition::beginning)); }
		| CURRENT{ position = pos_ptr(new Corpus::CPosition(Corpus::CPosition::current));} 
		| END{ position = pos_ptr(new Corpus::CPosition(CPosition::end)); }
		| NOWHERE{ position = pos_ptr(new Corpus::CPosition(CPosition::nowhere)); } 
		| i: INT
		{
			int val = atoi(i->getText().c_str());
			position = pos_ptr(new Corpus::CPosition(val));
		} 
		;

zmienna returns [pos_ptr position] 
		: z : ZMIENNA
		  { 
				char *pos = strdup(z->getText().c_str()+1);// omijamy $
				//std::cout << "zmiennna: " << pos << std::endl;
				if(pos[0] == '+' || pos[0] == '-')
				{
					std::stringstream sOffset;
					sOffset << pos[0];
					size_t i = 1;
					for(; i < strlen(pos); i++)
					{
						if( !isdigit(pos[i]) )
							break;
							
						sOffset << pos[i];
					}
					std::string name = (pos+i);
					int offset = atoi(sOffset.str().c_str());
					//std::cout << "Zmienna o nazwie " << name << " z offsetem " <<  offset <<std::endl;
					
					std::map<std::string,pos_ptr>::iterator find_inner = m_mPositions.find(name) ;
					if( find_inner == m_mPositions.end() )
						throw std::string( (std::string("Offset used for undefined variable - " )  + name ).c_str());
					
					std::map<std::string,pos_ptr>::iterator find =  m_mPositions.find(pos) ;
					if(find == m_mPositions.end() )
					{
						position = pos_ptr(new JOSKIPI::Operators::COffsetPosition(
								offset,find_inner->second));
						//std::cout << "Nazwa zmiennej: " << pos << std::endl;
						m_mPositions[pos] = position;
					}
					else
					{
						//std::cout << "Zmienna znaleziona...\n";
						position = find->second;
					}
				
				}
				else
				{
					//std::cout << "Zmienna o nazwie " << pos << std::endl;
					if(m_mPositions.find(pos) == m_mPositions.end() )
					{
						position = pos_ptr(new Corpus::CPosition());
						m_mPositions[pos] = position;
					}
					else
					{
						//std::cout << "Zmienna znaleziona...\n";
						position = m_mPositions[pos] ;
					}
				}
				
				free(pos);
				
				
			}
		  ;
//------------------------------------
//--------------   LEXER   -----------
//------------------------------------
class ANTLRLexer extends Lexer;
options{
 k = 10;
 charVocabulary = '\3'..'\377'; 
}

tokens{
// operatory
	//prosty test
	EQUAL = "equal";
	INTER = "inter";
	IN_TEST = "in";
	ISBIG = "isbig";
	HASNUM = "hasnum";
	//bin. test
	LLOOK = "llook";
	RLOOK = "rlook";
	ONLY = "only";
	ATLEAST = "atleast";
	AGR = "agr";
	AGRPP = "agrpp";
	WAGR = "wagr";
	//zl test
	AND = "and";
	OR = "or";
	NOT = "not";
	//reguly
	DELETETOK = "delete";

	//proste
	FLEX = "flex";	
	CATFLT = "catflt";
	AGRFLT = "agrflt";
	DISPOS = "dispos";
	DISCAT = "discat";
	//tekstowe
	PREFIX = "prefix";
	SUFFIX = "suffix";
	ORTH = "orth";
	BASE = "base";	
	
//stale pozycje
	BEGIN = "begin";
	CURRENT = "current";
	END = "end";
	NOWHERE = "nowhere";

//czesci mowy
	//rzeczownik
	SUBST = "subst";
	DEPR = "depr";
	//liczebnik
	NUM = "num";
	NUMCOL = "numcol";
	//przymiotnik
	ADJ = "adj";
	ADJA = "adja";
	ADJP = "adjp";
	
	//PPRON3 = "ppron3";
	//czasownik
	FIN = "fin";
	BEDZIE = "bedzie";
	AGLT = "aglt";
	PRAET = "praet";
	IMPT = "impt";
	IMPS = "imps";
	INF = "inf";
	PCON = "pcon";
	PANT = "pant";
	GER = "ger";
	PACT = "pact";
	PPAS = "ppas";
	//pozostale
	ADV = "adv";
	WINIEN = "winien";
	PRED = "pred";
	PREP = "prep";
	CONJ = "conj";
	QUB = "qub";
	XXS = "xxs";
	XXX = "xxx";
	IGN = "ign";
	INTERP = "interp";
	
	TWF = "twf";
	TNUM = "tnum";
	TSYM = "tsym";
	TDATE = "tdate";
	TTIME = "ttime";
	TURI = "turi";
	TMAIL = "tmail";

// atrybuty
	NMB = "nmb";
	CAS = "cas";
	GND = "gnd";
	PER = "per";
	DEG = "deg";
	ASP = "asp";
	ACM = "acm";
	ACN = "acn";
	PPR = "ppr";
	AGG = "agg";
	VCL = "vcl";
	TNT = "tnt";

// kategorie
	//liczba
	SG= "sg";
	PL = "pl";
	//przypadek
	NOM = "nom";
	GEN = "gen";
	DAT = "dat";
	ACC =  "acc";
	INST = "inst";
	LOC =  "loc";
	VOC = "voc";
	//rodzaj
	M1 = "m1";
	M2 = "m2";
	M3 = "m3";
	F = "f";
	N = "n";
	//osoba
	POS = "pos";
	PRI = "pri"; 
	SEC = "sec"; 
	TER = "ter"; 
	//stopien
	COMP = "comp"; 
	SUP = "sup"; 
	//aspekt
	IMPERF = "imperf"; 
	PERF = "perf"; 
	//zanegowanie
	AFF = "aff"; 	
	NEG = "neg"; 
	//akcentowosc
	AKC = "akc"; 
	NAKC = "nakc";
	//poprzyimkowosc
	PRAEP = "praep"; 
	NPRAEP = "npraep"; 
	//akomodacyjnosc
	CONGR = "congr"; 
	REC = "rec"; 
	//aglutynacyjnosc
	AGL = "agl";
	NAGL = "nagl";
	//wokalicznos
	WOK = "wok"; 
	NWOK = "nwok";
	NONE = "none";
	//rodzaj literalu liczbowego
	INTEGER = "integer";
	FRAC = "frac";
}

PPRON3 : "ppron3";
//zaimek
PPRON12 : "ppron12";

INT	: ('-')?('0'..'9')+;

Q_MARK : '?';

LBRACKET		options {
						paraphrase = "'['";
						}	: '[' ;
RBRACKET		options {
						paraphrase = "']'";
						}   : ']' ;
LPAREN			options {
						paraphrase = "'('";
						}   : '(' ;
RPAREN          options {
						paraphrase = "')'";
						}	: ')' ;
LCURLY			options {
						paraphrase = "'{'";
						}   : '{' ;
RCURLY			options {
						paraphrase = "'}'";
						}   : '}' ;

COMMA options	{
					paraphrase = "','";
				}	: ',' ;

ZMIENNA 
		options 
                {
                testLiterals = true; 
				paraphrase = "a var";
                }
		: '$' (('+'|'-')('0'..'9')+)? ( 'a'..'z' | 'A'..'Z' | '_' ) ( 'a'..'z' | 'A'..'Z' | '_' | '0'..'9' )*
		;

STRING
        options 
                {
                testLiterals = true; 
				paraphrase = "a string";
                }
        :	'"' (~'"')* '"'
		|	'\'' (~'\'')* '\''
				
        ;

ID
	options 
    {
		testLiterals = true; 
		paraphrase = "an identifier";
	}
	: ('a'..'z')+ | 'm'('1'..'3')
	;

        
WS : 	(	' ' 
		| 	'\t'
		|	'\r' '\n' 	{newline();}
		|	'\n'		{newline();}
		)
		{ $setType(antlr::Token::SKIP); }
		;
COMMENT
	:	"//" 	(~'\n')* '\n'{ $setType(Token::SKIP); newline(); };

HASH			options {
						paraphrase = "'#'";
						}   : '#' ;

// "oddzielacz" priorytetu od warunku w regule "delete"
DSEPARATOR : ":-" ;

MACROID
		options
				{
				testLiterals = true;
				paraphrase = "an identifier of a macro";
				}
		: '!' ( 'a'..'z' | 'A'..'Z' | '_' | '0'..'9' )+
		;

LCHEVRON			options {
						paraphrase = "'<'";
						}   : '<' ;
RCHEVRON			options {
						paraphrase = "'>'";
						}   : '>' ;

COLON
		options
				{
				paraphrase = "':'";
				}	: ':' ;
