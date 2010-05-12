/* $ANTLR 2.7.6 (20071205): "joskipi.g" -> "ANTLRParser.cpp"$ */

#include "../../joskipi_headers.h"

//#define JOSKIPI_SHOW_ANTLR_DEBUG_MSG


#include "ANTLRParser.hpp"
#include <antlr/NoViableAltException.hpp>
#include <antlr/SemanticException.hpp>
#include <antlr/ASTFactory.hpp>
ANTLR_BEGIN_NAMESPACE(JOSKIPI)
ANTLR_BEGIN_NAMESPACE(Parsers)
ANTLR_BEGIN_NAMESPACE(antlrgen)
ANTLRParser::ANTLRParser(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf, int k)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(tokenBuf,k)
{
}

ANTLRParser::ANTLRParser(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(tokenBuf,5)
{
}

ANTLRParser::ANTLRParser(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer, int k)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(lexer,k)
{
}

ANTLRParser::ANTLRParser(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(lexer,5)
{
}

ANTLRParser::ANTLRParser(const ANTLR_USE_NAMESPACE(antlr)ParserSharedInputState& state)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(state,5)
{
}

void ANTLRParser::wzorce(
	map<CValue,CPattern*>* patterns
) {
	
		map<string,vector<COperator*>*>* preProcessor = new map<string,vector<COperator*>*>();
	
	
	{ // ( ... )*
	for (;;) {
		if ((LA(1) == MACROID)) {
			makro(preProcessor);
		}
		else {
			goto _loop3;
		}
		
	}
	_loop3:;
	} // ( ... )*
	{ // ( ... )+
	int _cnt5=0;
	for (;;) {
		if ((_tokenSet_0.member(LA(1)))) {
			wzorzec(patterns, preProcessor);
		}
		else {
			if ( _cnt5>=1 ) { goto _loop5; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
		}
		
		_cnt5++;
	}
	_loop5:;
	}  // ( ... )+
}

void ANTLRParser::makro(
	map<string,vector<COperator*>*>* preProcessor
) {
	ANTLR_USE_NAMESPACE(antlr)RefToken  ident = ANTLR_USE_NAMESPACE(antlr)nullToken;
	
		vector<COperator*>* schemat = new vector<COperator*>();
	
	
	ident = LT(1);
	match(MACROID);
	match(LPAREN);
	zbior_operatorow(schemat);
	match(RPAREN);
	
				string mid = ident->getText();
				(*preProcessor)[mid] = schemat;
			
}

void ANTLRParser::wzorzec(
	map<CValue,CPattern*>* patterns, map<string,vector<COperator*>*>* macros
) {
	
		CValue mask(0,0);
		CPattern* pattern = NULL;
	
	
	maska_klas(mask);
	match(COLON);
	maska_kategorii(mask);
	match(HASH);
	match(LCHEVRON);
	pattern=operatory_makra(macros, mask);
	match(RCHEVRON);
	
				(*patterns)[mask] = pattern;
			
}

void ANTLRParser::zbior_operatorow(
	vector<COperator*>* schema
) {
	
		COperator* o = NULL;
	
	
	o=operator_joskipi();
	
				schema->push_back(o);
			
	{ // ( ... )*
	for (;;) {
		if ((LA(1) == COMMA)) {
			match(COMMA);
			o=operator_joskipi();
			
							schema->push_back(o);
						
		}
		else {
			goto _loop9;
		}
		
	}
	_loop9:;
	} // ( ... )*
}

COperator*  ANTLRParser::operator_joskipi() {
	COperator* o;
	
		m_mPositions.clear(); // nazwy zmiennych liczymy od nowa
		o = NULL;
	
	
	switch ( LA(1)) {
	case Q_MARK:
	{
		o=operator_warunkowy();
		break;
	}
	case PREFIX:
	case SUFFIX:
	case ORTH:
	case BASE:
	case FLEX:
	case CATFLT:
	case AGRFLT:
	case DISPOS:
	case DISCAT:
	case AND:
	case OR:
	case NOT:
	case LLOOK:
	case RLOOK:
	case ONLY:
	case ATLEAST:
	case AGR:
	case WAGR:
	case AGRPP:
	case EQUAL:
	case IN_TEST:
	case INTER:
	case ISBIG:
	case HASNUM:
	case NMB:
	case CAS:
	case GND:
	case PER:
	case DEG:
	case ASP:
	case ACM:
	case ACN:
	case PPR:
	case AGG:
	case VCL:
	case TNT:
	{
		o=operator_bezwarunkowy();
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	return o;
}

void ANTLRParser::maska_klas(
	CValue& mask
) {
	
	{ // ( ... )*
	for (;;) {
		if (((LA(1) >= ADV && LA(1) <= PPAS))) {
			klasa(mask);
		}
		else {
			goto _loop13;
		}
		
	}
	_loop13:;
	} // ( ... )*
}

void ANTLRParser::maska_kategorii(
	CValue& mask
) {
	
	{ // ( ... )*
	for (;;) {
		if (((LA(1) >= SG && LA(1) <= NWOK))) {
			kategoria(mask);
		}
		else {
			goto _loop17;
		}
		
	}
	_loop17:;
	} // ( ... )*
}

CPattern*  ANTLRParser::operatory_makra(
	map<string,vector<COperator*>*>* macros, CValue& mask
) {
	CPattern* patt;
	
		vector<COperator*>* schema = new vector<COperator*>();
		patt = new CPattern(schema, mask.first, mask.second);
		COperator* o = NULL;
	
	
	operator_makro(patt, macros);
	{ // ( ... )*
	for (;;) {
		if ((LA(1) == COMMA)) {
			match(COMMA);
			operator_makro(patt, macros);
		}
		else {
			goto _loop21;
		}
		
	}
	_loop21:;
	} // ( ... )*
	return patt;
}

void ANTLRParser::klasa(
	CValue& mask
) {
	
		RefToken curr = LT(1);
	
	
	klasagram();
	
				TFlexClass flexclass = CTag::GetFlexClass( curr->getText());
				if(flexclass != cf_none)
					mask.first |= flexclass;
				else throw string( string("Unknown flex class: " + curr->getText()).c_str());
			
}

void ANTLRParser::klasagram() {
	
	switch ( LA(1)) {
	case SUBST:
	case DEPR:
	{
		rzeczownik();
		break;
	}
	case NUM:
	case NUMCOL:
	{
		liczebnik();
		break;
	}
	case ADJ:
	case ADJA:
	case ADJP:
	{
		przymiotnik();
		break;
	}
	case ADV:
	{
		match(ADV);
		break;
	}
	case PPRON12:
	case PPRON3:
	{
		zaimek();
		break;
	}
	case SIEBIE:
	{
		match(SIEBIE);
		break;
	}
	case FIN:
	case BEDZIE:
	case AGLT:
	case PRAET:
	case IMPT:
	case IMPS:
	case INF:
	case PCON:
	case PANT:
	case GER:
	case PACT:
	case PPAS:
	{
		czasownik();
		break;
	}
	case WINIEN:
	{
		match(WINIEN);
		break;
	}
	case PRED:
	{
		match(PRED);
		break;
	}
	case PREP:
	{
		match(PREP);
		break;
	}
	case CONJ:
	{
		match(CONJ);
		break;
	}
	case QUB:
	{
		match(QUB);
		break;
	}
	case XXS:
	{
		match(XXS);
		break;
	}
	case XXX:
	{
		match(XXX);
		break;
	}
	case IGN:
	{
		match(IGN);
		break;
	}
	case INTERP:
	{
		match(INTERP);
		break;
	}
	case TWF:
	{
		match(TWF);
		break;
	}
	case TNUM:
	{
		match(TNUM);
		break;
	}
	case TSYM:
	{
		match(TSYM);
		break;
	}
	case TDATE:
	{
		match(TDATE);
		break;
	}
	case TTIME:
	{
		match(TTIME);
		break;
	}
	case TURI:
	{
		match(TURI);
		break;
	}
	case TMAIL:
	{
		match(TMAIL);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
}

void ANTLRParser::kategoria(
	CValue& mask
) {
	
		RefToken curr = LT(1);
	
	
	kategoriagram();
	
				DLONG gcat = CTag::GetGrammCatValue(curr->getText());
				if(gcat != gc_none)
					mask.second |= gcat;
				else throw string( string("Unknown gramm category: " + curr->getText()).c_str());
			
}

void ANTLRParser::kategoriagram() {
	
	switch ( LA(1)) {
	case SG:
	case PL:
	{
		liczba();
		break;
	}
	case NOM:
	case GEN:
	case DAT:
	case ACC:
	case INST:
	case LOC:
	case VOC:
	{
		przypadek();
		break;
	}
	case M1:
	case M2:
	case M3:
	case F:
	case N:
	{
		rodzaj();
		break;
	}
	case PRI:
	case SEC:
	case TER:
	{
		osoba();
		break;
	}
	case POS:
	case COMP:
	case SUP:
	{
		stopien();
		break;
	}
	case IMPERF:
	case PERF:
	{
		aspekt();
		break;
	}
	case AFF:
	case NEG:
	{
		zanegowanie();
		break;
	}
	case AKC:
	case NAKC:
	{
		akcentowosc();
		break;
	}
	case PRAEP:
	case NPRAEP:
	{
		poprzyimkowosc();
		break;
	}
	case CONGR:
	case REC:
	{
		akomodacyjnosc();
		break;
	}
	case NAGL:
	case AGL:
	{
		aglutynacyjnosc();
		break;
	}
	case WOK:
	case NWOK:
	{
		wokalicznosc();
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
}

void ANTLRParser::operator_makro(
	CPattern* patt, map<string,vector<COperator*>*>* macros
) {
	ANTLR_USE_NAMESPACE(antlr)RefToken  macro = ANTLR_USE_NAMESPACE(antlr)nullToken;
	
		COperator* o = NULL;
	
	
	switch ( LA(1)) {
	case Q_MARK:
	case PREFIX:
	case SUFFIX:
	case ORTH:
	case BASE:
	case FLEX:
	case CATFLT:
	case AGRFLT:
	case DISPOS:
	case DISCAT:
	case AND:
	case OR:
	case NOT:
	case LLOOK:
	case RLOOK:
	case ONLY:
	case ATLEAST:
	case AGR:
	case WAGR:
	case AGRPP:
	case EQUAL:
	case IN_TEST:
	case INTER:
	case ISBIG:
	case HASNUM:
	case NMB:
	case CAS:
	case GND:
	case PER:
	case DEG:
	case ASP:
	case ACM:
	case ACN:
	case PPR:
	case AGG:
	case VCL:
	case TNT:
	{
		o=operator_joskipi();
		
					patt->AddOperator(o);
				
		break;
	}
	case MACROID:
	{
		macro = LT(1);
		match(MACROID);
		
					string mid = macro->getText();
					vector<COperator*>* opers = (*macros)[mid];
					patt->AddOperators(opers);
				
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
}

void ANTLRParser::reguly(
	CRuleSet* rs
) {
	
	
	
	{ // ( ... )*
	for (;;) {
		if ((LA(1) == DELETETOK)) {
			regula(rs);
		}
		else {
			goto _loop25;
		}
		
	}
	_loop25:;
	} // ( ... )*
}

void ANTLRParser::regula(
	CRuleSet* rules
) {
	ANTLR_USE_NAMESPACE(antlr)RefToken  i = ANTLR_USE_NAMESPACE(antlr)nullToken;
	
		m_mPositions.clear(); // nazwy zmiennych liczymy od nowa
		CCondition* akcja = NULL;
		CCondition* warunek = NULL;
	
	
	match(DELETETOK);
	match(LPAREN);
	akcja=testop();
	match(RPAREN);
	match(HASH);
	i = LT(1);
	match(INT);
	match(DSEPARATOR);
	warunek=testop();
	
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

CCondition*  ANTLRParser::testop() {
	CCondition* cond;
	
		cond = NULL;
	
	
	switch ( LA(1)) {
	case EQUAL:
	case IN_TEST:
	case INTER:
	case ISBIG:
	case HASNUM:
	{
		cond=prosty_test();
		break;
	}
	case LLOOK:
	case RLOOK:
	case ONLY:
	case ATLEAST:
	case AGR:
	case WAGR:
	case AGRPP:
	{
		cond=binarny_test();
		break;
	}
	case AND:
	case OR:
	case NOT:
	{
		cond=zlozony_test();
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	return cond;
}

COperator * ANTLRParser::operator_warunkowy() {
	COperator *o;
	
		o = NULL;
		COperator *op = NULL;
		CCondition *cond = NULL;
	
	
	match(Q_MARK);
	op=elementwektora();
	match(Q_MARK);
	cond=testop();
	
				o = new CCondOper(cond, op);
			
	return o;
}

COperator * ANTLRParser::operator_bezwarunkowy() {
	COperator *o;
	
		o = NULL;
	
	
	o=elementwektora();
	return o;
}

COperator*  ANTLRParser::elementwektora() {
	COperator* o;
	
		CCondition *cond = NULL;
		o = NULL;
	
	
	switch ( LA(1)) {
	case FLEX:
	case CATFLT:
	case AGRFLT:
	case DISPOS:
	case DISCAT:
	case NMB:
	case CAS:
	case GND:
	case PER:
	case DEG:
	case ASP:
	case ACM:
	case ACN:
	case PPR:
	case AGG:
	case VCL:
	case TNT:
	{
		o=prosty_operator();
		break;
	}
	case PREFIX:
	case SUFFIX:
	case ORTH:
	case BASE:
	{
		o=tekstowy_operator();
		break;
	}
	case AND:
	case OR:
	case NOT:
	case LLOOK:
	case RLOOK:
	case ONLY:
	case ATLEAST:
	case AGR:
	case WAGR:
	case AGRPP:
	case EQUAL:
	case IN_TEST:
	case INTER:
	case ISBIG:
	case HASNUM:
	{
		cond=testop();
		
					o = new CTestOper(cond);
				
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	return o;
}

COperator*  ANTLRParser::prosty_operator() {
	COperator* oper;
	ANTLR_USE_NAMESPACE(antlr)RefToken  bits_f = ANTLR_USE_NAMESPACE(antlr)nullToken;
	
		oper = NULL;
		pos_ptr position, pos2;
		string attr_type = "";
		vector<CValue> vals1, vals2;
		CConstValue *constVal = NULL, *constVal2 = NULL;
		RefToken curr = LT(1);
	
	
	switch ( LA(1)) {
	case FLEX:
	{
		match(FLEX);
		match(LBRACKET);
		position=pozycja();
		match(RBRACKET);
		
							oper = new CSimpleOperator(position, CTag::GetAttribIndex("flex"));
						
		break;
	}
	case NMB:
	case CAS:
	case GND:
	case PER:
	case DEG:
	case ASP:
	case ACM:
	case ACN:
	case PPR:
	case AGG:
	case VCL:
	case TNT:
	{
		atrybut();
		
								attr_type = curr->getText();
							
		match(LBRACKET);
		position=pozycja();
		match(RBRACKET);
		
								oper = new CSimpleOperator(position, CTag::GetAttribIndex(attr_type));
							
		break;
	}
	case CATFLT:
	{
		match(CATFLT);
		match(LPAREN);
		position=pozycja();
		match(COMMA);
		wartosc(vals1);
		match(COMMA);
		wartosc(vals2);
		match(RPAREN);
		
							constVal = CreateConstValue(vals1);
							CValue v = CreateOrValue(vals2);
							constVal->AddValue(v);
		
							oper = new CCatFlt(position, constVal);
						
		break;
	}
	case AGRFLT:
	{
		match(AGRFLT);
		match(LPAREN);
		position=pozycja();
		match(COMMA);
		pos2=pozycja();
		match(COMMA);
		wartosc(vals1);
		match(COMMA);
		bits_f = LT(1);
		match(INT);
		match(COMMA);
		wartosc(vals2);
		match(RPAREN);
		
							constVal = CreateConstValue(vals1);
							constVal2 = CreateConstValue(vals2);
							int bits = atoi(bits_f->getText().c_str());
		
							oper = new CAgrFlt(position, pos2, constVal, bits, constVal2);
						
		break;
	}
	case DISPOS:
	{
		match(DISPOS);
		match(LBRACKET);
		position=pozycja();
		match(RBRACKET);
		
							oper = new CDisPosOper(position);
						
		break;
	}
	case DISCAT:
	{
		match(DISCAT);
		match(LBRACKET);
		position=pozycja();
		match(RBRACKET);
		
							oper = new CDisCatOper(position);
						
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	return oper;
}

COperator*  ANTLRParser::tekstowy_operator() {
	COperator* oper;
	
		oper = NULL;
		string op_type = "";
		pos_ptr position;
	
	
	switch ( LA(1)) {
	case ORTH:
	case BASE:
	{
		op_type=prosty_tekstowy_operator();
		match(LBRACKET);
		position=pozycja();
		match(RBRACKET);
		
					oper = new CSimpleOperator(position, Corpus::CTag::GetAttribIndex(op_type));
				
		break;
	}
	case PREFIX:
	{
		oper=prefix_tekstowy_operator();
		break;
	}
	case SUFFIX:
	{
		oper=suffix_tekstowy_operator();
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	return oper;
}

string  ANTLRParser::prosty_tekstowy_operator() {
	string s;
	
		s = "";
	
	
	switch ( LA(1)) {
	case ORTH:
	{
		match(ORTH);
		s = "orth";
		break;
	}
	case BASE:
	{
		match(BASE);
		s = "base";
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	return s;
}

pos_ptr  ANTLRParser::pozycja() {
	pos_ptr position;
	
	switch ( LA(1)) {
	case INT:
	case BEGIN:
	case CURRENT:
	case END:
	case NOWHERE:
	{
		position=num();
		break;
	}
	case ZMIENNA:
	{
		position=zmienna();
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	return position;
}

COperator*  ANTLRParser::prefix_tekstowy_operator() {
	COperator* oper;
	
		throw std::string("No implementation for prefix operator!");
		pos_ptr position, n;
		string inner;
	
	
	match(PREFIX);
	match(LPAREN);
	inner=prosty_tekstowy_operator();
	match(LBRACKET);
	position=pozycja();
	match(RBRACKET);
	match(COMMA);
	n=num();
	match(RPAREN);
	return oper;
}

COperator*  ANTLRParser::suffix_tekstowy_operator() {
	COperator* oper;
	
		throw std::string("No implementation for suffix operator!");
		pos_ptr position, n;
		string inner;
	
	
	match(SUFFIX);
	match(LPAREN);
	inner=prosty_tekstowy_operator();
	match(LBRACKET);
	position=pozycja();
	match(RBRACKET);
	match(COMMA);
	n=num();
	match(RPAREN);
	return oper;
}

pos_ptr  ANTLRParser::num() {
	pos_ptr position;
	ANTLR_USE_NAMESPACE(antlr)RefToken  i = ANTLR_USE_NAMESPACE(antlr)nullToken;
	
	switch ( LA(1)) {
	case BEGIN:
	{
		match(BEGIN);
		position = pos_ptr(new Corpus::CPosition(CPosition::beginning));
		break;
	}
	case CURRENT:
	{
		match(CURRENT);
		position = pos_ptr(new Corpus::CPosition(Corpus::CPosition::current));
		break;
	}
	case END:
	{
		match(END);
		position = pos_ptr(new Corpus::CPosition(CPosition::end));
		break;
	}
	case NOWHERE:
	{
		match(NOWHERE);
		position = pos_ptr(new Corpus::CPosition(CPosition::nowhere));
		break;
	}
	case INT:
	{
		i = LT(1);
		match(INT);
		
					int val = atoi(i->getText().c_str());
					position = pos_ptr(new Corpus::CPosition(val));
				
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	return position;
}

void ANTLRParser::atrybut() {
	
	switch ( LA(1)) {
	case NMB:
	{
		match(NMB);
		break;
	}
	case CAS:
	{
		match(CAS);
		break;
	}
	case GND:
	{
		match(GND);
		break;
	}
	case PER:
	{
		match(PER);
		break;
	}
	case DEG:
	{
		match(DEG);
		break;
	}
	case ASP:
	{
		match(ASP);
		break;
	}
	case ACM:
	{
		match(ACM);
		break;
	}
	case ACN:
	{
		match(ACN);
		break;
	}
	case PPR:
	{
		match(PPR);
		break;
	}
	case AGG:
	{
		match(AGG);
		break;
	}
	case VCL:
	{
		match(VCL);
		break;
	}
	case TNT:
	{
		match(TNT);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
}

void ANTLRParser::wartosc(
	vector<CValue>& vals
) {
	
		CValue val(0,0);
	#ifdef JOSKIPI_SHOW_ANTLR_DEBUG_MSG
		std::cout << "wartosc entered\n";
	#endif
	
	
	switch ( LA(1)) {
	case NONE:
	case STRING:
	case NMB:
	case CAS:
	case GND:
	case PER:
	case DEG:
	case ASP:
	case ACM:
	case ACN:
	case PPR:
	case AGG:
	case VCL:
	case TNT:
	case ADV:
	case SIEBIE:
	case WINIEN:
	case PRED:
	case PREP:
	case CONJ:
	case QUB:
	case XXS:
	case XXX:
	case IGN:
	case INTERP:
	case TWF:
	case TNUM:
	case TSYM:
	case TDATE:
	case TTIME:
	case TURI:
	case TMAIL:
	case SUBST:
	case DEPR:
	case NUM:
	case NUMCOL:
	case ADJ:
	case ADJA:
	case ADJP:
	case PPRON12:
	case PPRON3:
	case FIN:
	case BEDZIE:
	case AGLT:
	case PRAET:
	case IMPT:
	case IMPS:
	case INF:
	case PCON:
	case PANT:
	case GER:
	case PACT:
	case PPAS:
	case SG:
	case PL:
	case NOM:
	case GEN:
	case DAT:
	case ACC:
	case INST:
	case LOC:
	case VOC:
	case M1:
	case M2:
	case M3:
	case F:
	case N:
	case PRI:
	case SEC:
	case TER:
	case POS:
	case COMP:
	case SUP:
	case IMPERF:
	case PERF:
	case AFF:
	case NEG:
	case AKC:
	case NAKC:
	case PRAEP:
	case NPRAEP:
	case CONGR:
	case REC:
	case NAGL:
	case AGL:
	case WOK:
	case NWOK:
	{
		val=prosta_wartosc();
		
					vals.push_back(val); 
				
		break;
	}
	case LCURLY:
	{
		match(LCURLY);
		{
		if (((LA(1) >= NONE && LA(1) <= NWOK)) && (LA(2) == RCURLY)) {
			{
			val=prosta_wartosc();
			
						 		vals.push_back(val); 
							
			}
		}
		else if (((LA(1) >= NONE && LA(1) <= NWOK)) && (LA(2) == COMMA)) {
			{
			wartosci(vals);
			}
		}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		
		}
		match(RCURLY);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
}

CCondition * ANTLRParser::prosty_test() {
	CCondition *oper;
	
		// TODO: kto zwlania wektor?
		std::vector<CCondElem*> *cond = new std::vector<CCondElem*>();
		oper = NULL;
	#ifdef JOSKIPI_SHOW_ANTLR_DEBUG_MSG
		std::cout << "prosty_test...enter\n";
	#endif
	
	
	switch ( LA(1)) {
	case EQUAL:
	{
		match(EQUAL);
		match(LPAREN);
		prosty_arg(cond);
		match(COMMA);
		prosty_arg(cond);
		match(RPAREN);
		
						  oper = new CEqualSC(*cond);
					
		break;
	}
	case IN_TEST:
	{
		match(IN_TEST);
		match(LPAREN);
		prosty_arg(cond);
		match(COMMA);
		prosty_arg(cond);
		match(RPAREN);
		
						 oper = new CInSC(*cond);
					
		break;
	}
	case INTER:
	{
		match(INTER);
		match(LPAREN);
		prosty_arg(cond);
		match(COMMA);
		prosty_arg(cond);
		match(RPAREN);
		
		#ifdef JOSKIPI_SHOW_ANTLR_DEBUG_MSG
						std::cout << "inter enter\n";
		#endif
						oper = new CIntersecSC(*cond);
					
		break;
	}
	case ISBIG:
	{
		match(ISBIG);
		match(LPAREN);
		prosty_arg_krotki(cond);
		match(RPAREN);
		
						 oper =new CIsBigSC(*cond);
					
		break;
	}
	case HASNUM:
	{
		match(HASNUM);
		match(LPAREN);
		prosty_arg_krotki(cond);
		match(RPAREN);
		
						 oper = new CHasNumSC(*cond);
					
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	return oper;
}

CBinaryComplexC * ANTLRParser::binarny_test() {
	CBinaryComplexC *oper;
	ANTLR_USE_NAMESPACE(antlr)RefToken  i = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  bits_a = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  bits_w = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  bits_pp = ANTLR_USE_NAMESPACE(antlr)nullToken;
	
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
	
	
	switch ( LA(1)) {
	case LLOOK:
	{
		match(LLOOK);
		match(LPAREN);
		pos1=pozycja();
		match(COMMA);
		pos2=pozycja();
		match(COMMA);
		var=zmienna();
		match(COMMA);
		innerTest=testop();
		match(RPAREN);
		
						positions->push_back(pos1);
						positions->push_back(pos2);
						positions->push_back(var);
						conditions->push_back(innerTest);
						oper = new CLookLeftBinCC(positions,conditions);
					
		break;
	}
	case RLOOK:
	{
		match(RLOOK);
		match(LPAREN);
		pos1=pozycja();
		match(COMMA);
		pos2=pozycja();
		match(COMMA);
		var=zmienna();
		match(COMMA);
		innerTest=testop();
		match(RPAREN);
		
						positions->push_back(pos1);
						positions->push_back(pos2);
						positions->push_back(var);
						conditions->push_back(innerTest);
						oper = new CLookRightBinCC(positions,conditions);
					
		break;
	}
	case ONLY:
	{
		match(ONLY);
		match(LPAREN);
		pos1=pozycja();
		match(COMMA);
		pos2=pozycja();
		match(COMMA);
		var=zmienna();
		match(COMMA);
		innerTest=testop();
		match(RPAREN);
		
						positions->push_back(pos1);
						positions->push_back(pos2);
						positions->push_back(var);
						conditions->push_back(innerTest);
						oper = new COnlyBinCC(positions,conditions);
					
		break;
	}
	case ATLEAST:
	{
		match(ATLEAST);
		match(LPAREN);
		pos1=pozycja();
		match(COMMA);
		pos2=pozycja();
		match(COMMA);
		var=zmienna();
		match(COMMA);
		innerTest=testop();
		match(COMMA);
		i = LT(1);
		match(INT);
		match(RPAREN);
		
						int num = atoi(i->getText().c_str());
						positions->push_back(pos1);
						positions->push_back(pos2);
						positions->push_back(var);
						conditions->push_back(innerTest);
						oper = new CAtLeastBinCC(positions,conditions, num);
					
		break;
	}
	case AGR:
	{
		match(AGR);
		match(LPAREN);
		pos1=pozycja();
		match(COMMA);
		pos2=pozycja();
		match(COMMA);
		wartosc(vals);
		match(COMMA);
		bits_a = LT(1);
		match(INT);
		match(RPAREN);
		
						int bits = atoi(bits_a->getText().c_str());
						positions->push_back(pos1);
						positions->push_back(pos2);
						//TODO: brzydkie!
						positions->push_back(pos_ptr(new Corpus::CPosition()));
						constval = CreateConstValue(vals);
						
						oper = new CAgrBinCC(positions, constval, bits);
					
		break;
	}
	case WAGR:
	{
		match(WAGR);
		match(LPAREN);
		pos1=pozycja();
		match(COMMA);
		pos2=pozycja();
		match(COMMA);
		wartosc(vals);
		match(COMMA);
		bits_w = LT(1);
		match(INT);
		match(RPAREN);
		
						int bits = atoi(bits_w->getText().c_str());
						positions->push_back(pos1);
						positions->push_back(pos2);
						positions->push_back(pos_ptr(new Corpus::CPosition()));
						constval = CreateConstValue(vals);
						oper = new CAgrWeakBinCC(positions, constval, bits);
					
		break;
	}
	case AGRPP:
	{
		match(AGRPP);
		match(LPAREN);
		pos1=pozycja();
		match(COMMA);
		pos2=pozycja();
		match(COMMA);
		wartosc(vals);
		match(COMMA);
		bits_pp = LT(1);
		match(INT);
		match(RPAREN);
		
						int bits = atoi(bits_pp->getText().c_str());
						positions->push_back(pos1);
						positions->push_back(pos2);
						//TODO: brzydkie!
						positions->push_back(pos_ptr(new Corpus::CPosition()));
						constval = CreateConstValue(vals);
		
						//std::cout << "agrpp: pos1 = " << pos1->ToString() << ", pos2: " << pos2->ToString()
						//	<<", wartosc = " << constval->ToString() << ", bits: " << bits << std::endl
						
						
						oper = new CAgrPPBinCC(positions, constval, bits);
					
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	return oper;
}

CCondition*  ANTLRParser::zlozony_test() {
	CCondition* cond;
	
		//pamiec wektora zwlania CComplexCondition
		cond = NULL;
		std::vector<CCondition*>* conditions = new std::vector<CCondition*>();
	
	
	switch ( LA(1)) {
	case AND:
	{
		match(AND);
		match(LPAREN);
		sekwencja_test(conditions);
		match(RPAREN);
		
				cond = new CAndCC(conditions);
			
		break;
	}
	case OR:
	{
		match(OR);
		match(LPAREN);
		sekwencja_test(conditions);
		match(RPAREN);
		
				cond = new COrCC(conditions);
			
		break;
	}
	case NOT:
	{
		match(NOT);
		match(LPAREN);
		sekwencja_test(conditions);
		match(RPAREN);
		
				cond = new CNotCC(conditions);
			
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	return cond;
}

void ANTLRParser::sekwencja_test(
	vector<CCondition*>* conditions
) {
	
		CCondition* cond = NULL;
	
	
	cond=testop();
	conditions->push_back(cond);
	{ // ( ... )*
	for (;;) {
		if ((LA(1) == COMMA)) {
			match(COMMA);
			cond=testop();
			conditions->push_back(cond);
		}
		else {
			goto _loop40;
		}
		
	}
	_loop40:;
	} // ( ... )*
}

pos_ptr  ANTLRParser::zmienna() {
	pos_ptr position;
	ANTLR_USE_NAMESPACE(antlr)RefToken  z = ANTLR_USE_NAMESPACE(antlr)nullToken;
	
	z = LT(1);
	match(ZMIENNA);
	
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
					
					
				
	return position;
}

void ANTLRParser::prosty_arg(
	std::vector<CCondElem*>* cond
) {
	
		COperator *oper = NULL;
		vector<CValue> vals;
		CConstValue *constval = NULL;
		CValue logor(0,0);
	#ifdef JOSKIPI_SHOW_ANTLR_DEBUG_MSG
		std::cout << "prosty_arg...enter\n";
	#endif
	
	
	if ((_tokenSet_1.member(LA(1))) && (LA(2) == LPAREN || LA(2) == LBRACKET)) {
		oper=prosty_operator();
		
						cond->push_back(oper);
					
	}
	else if (((LA(1) >= PREFIX && LA(1) <= BASE))) {
		oper=tekstowy_operator();
		
						cond->push_back(oper); 
					
	}
	else if ((_tokenSet_2.member(LA(1))) && (_tokenSet_3.member(LA(2)))) {
		wartosc(vals);
		
						constval = CreateConstValue(vals);
						cond->push_back(constval);
					
	}
	else {
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	
}

void ANTLRParser::prosty_arg_krotki(
	std::vector<CCondElem*>* cond
) {
	
		COperator *oper = NULL;
		vector<CValue> vals;
		CValue logor(0,0);
		CConstValue *constval = NULL;
	
	
	switch ( LA(1)) {
	case PREFIX:
	case SUFFIX:
	case ORTH:
	case BASE:
	{
		oper=tekstowy_operator();
		cond->push_back(oper);
		break;
	}
	case LCURLY:
	case NONE:
	case STRING:
	case NMB:
	case CAS:
	case GND:
	case PER:
	case DEG:
	case ASP:
	case ACM:
	case ACN:
	case PPR:
	case AGG:
	case VCL:
	case TNT:
	case ADV:
	case SIEBIE:
	case WINIEN:
	case PRED:
	case PREP:
	case CONJ:
	case QUB:
	case XXS:
	case XXX:
	case IGN:
	case INTERP:
	case TWF:
	case TNUM:
	case TSYM:
	case TDATE:
	case TTIME:
	case TURI:
	case TMAIL:
	case SUBST:
	case DEPR:
	case NUM:
	case NUMCOL:
	case ADJ:
	case ADJA:
	case ADJP:
	case PPRON12:
	case PPRON3:
	case FIN:
	case BEDZIE:
	case AGLT:
	case PRAET:
	case IMPT:
	case IMPS:
	case INF:
	case PCON:
	case PANT:
	case GER:
	case PACT:
	case PPAS:
	case SG:
	case PL:
	case NOM:
	case GEN:
	case DAT:
	case ACC:
	case INST:
	case LOC:
	case VOC:
	case M1:
	case M2:
	case M3:
	case F:
	case N:
	case PRI:
	case SEC:
	case TER:
	case POS:
	case COMP:
	case SUP:
	case IMPERF:
	case PERF:
	case AFF:
	case NEG:
	case AKC:
	case NAKC:
	case PRAEP:
	case NPRAEP:
	case CONGR:
	case REC:
	case NAGL:
	case AGL:
	case WOK:
	case NWOK:
	{
		wartosc(vals);
		
						constval = CreateConstValue(vals);
						cond->push_back(constval);
					
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
}

CValue  ANTLRParser::prosta_wartosc() {
	CValue retVal;
	ANTLR_USE_NAMESPACE(antlr)RefToken  d = ANTLR_USE_NAMESPACE(antlr)nullToken;
	
		CValue ret(0,0);
	#ifdef JOSKIPI_SHOW_ANTLR_DEBUG_MSG
		std::cout << "prosta_wartosc entered... \n";
	#endif
		RefToken curr = LT(1);
	
	
	
	switch ( LA(1)) {
	case SG:
	case PL:
	case NOM:
	case GEN:
	case DAT:
	case ACC:
	case INST:
	case LOC:
	case VOC:
	case M1:
	case M2:
	case M3:
	case F:
	case N:
	case PRI:
	case SEC:
	case TER:
	case POS:
	case COMP:
	case SUP:
	case IMPERF:
	case PERF:
	case AFF:
	case NEG:
	case AKC:
	case NAKC:
	case PRAEP:
	case NPRAEP:
	case CONGR:
	case REC:
	case NAGL:
	case AGL:
	case WOK:
	case NWOK:
	{
		kategoriagram();
		
							DLONG gc = CTag::GetGrammCatValue(curr->getText());
							if(gc != gc_none)
								ret.second |= gc;
							retVal = ret;
						
		break;
	}
	case ADV:
	case SIEBIE:
	case WINIEN:
	case PRED:
	case PREP:
	case CONJ:
	case QUB:
	case XXS:
	case XXX:
	case IGN:
	case INTERP:
	case TWF:
	case TNUM:
	case TSYM:
	case TDATE:
	case TTIME:
	case TURI:
	case TMAIL:
	case SUBST:
	case DEPR:
	case NUM:
	case NUMCOL:
	case ADJ:
	case ADJA:
	case ADJP:
	case PPRON12:
	case PPRON3:
	case FIN:
	case BEDZIE:
	case AGLT:
	case PRAET:
	case IMPT:
	case IMPS:
	case INF:
	case PCON:
	case PANT:
	case GER:
	case PACT:
	case PPAS:
	{
		klasagram();
		
							TFlexClass flexclass = CTag::GetFlexClass( curr->getText());
							if(flexclass != cf_none)
								retVal.first |= flexclass;
							else throw std::string( string("Unknown flex class: " + curr->getText()).c_str());
							
						
		break;
	}
	case NMB:
	case CAS:
	case GND:
	case PER:
	case DEG:
	case ASP:
	case ACM:
	case ACN:
	case PPR:
	case AGG:
	case VCL:
	case TNT:
	{
		atrybut();
		
							CValue mask;
							CTag::GetAttribMask(CTag::GetAttribIndex(curr->getText()),mask);
							retVal.first |= mask.first;
							retVal.second|= mask.second;
						
		break;
	}
	case NONE:
	{
		match(NONE);
		
							// std::cerr << "ProstaWartosc = none... \n";
						
		break;
	}
	case STRING:
	{
		d = LT(1);
		match(STRING);
		
							string s = d->getText();
							boost::trim(s);
							boost::erase_all(s, "\"");
							// Corpus::Parser::ConvertToUTFFromCP1250(s,c); no conversion -> JOSKIPI in UTF-8
							retVal = Corpus::CValue(Corpus::CWordDictionary::GetInstance()->Add(s),str_msk);
						
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	return retVal;
}

void ANTLRParser::wartosci(
	vector<CValue>& vals
) {
	
		CValue val(0,0);
	
	
	val=prosta_wartosc();
	vals.push_back(val);
	{ // ( ... )+
	int _cnt51=0;
	for (;;) {
		if ((LA(1) == COMMA)) {
			match(COMMA);
			val=prosta_wartosc();
			vals.push_back(val);
		}
		else {
			if ( _cnt51>=1 ) { goto _loop51; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
		}
		
		_cnt51++;
	}
	_loop51:;
	}  // ( ... )+
}

void ANTLRParser::rzeczownik() {
	
	switch ( LA(1)) {
	case SUBST:
	{
		match(SUBST);
		break;
	}
	case DEPR:
	{
		match(DEPR);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
}

void ANTLRParser::liczebnik() {
	
	switch ( LA(1)) {
	case NUM:
	{
		match(NUM);
		break;
	}
	case NUMCOL:
	{
		match(NUMCOL);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
}

void ANTLRParser::przymiotnik() {
	
	switch ( LA(1)) {
	case ADJ:
	{
		match(ADJ);
		break;
	}
	case ADJA:
	{
		match(ADJA);
		break;
	}
	case ADJP:
	{
		match(ADJP);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
}

void ANTLRParser::zaimek() {
	
	switch ( LA(1)) {
	case PPRON12:
	{
		match(PPRON12);
		break;
	}
	case PPRON3:
	{
		match(PPRON3);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
}

void ANTLRParser::czasownik() {
	
	switch ( LA(1)) {
	case FIN:
	{
		match(FIN);
		break;
	}
	case BEDZIE:
	{
		match(BEDZIE);
		break;
	}
	case AGLT:
	{
		match(AGLT);
		break;
	}
	case PRAET:
	{
		match(PRAET);
		break;
	}
	case IMPT:
	{
		match(IMPT);
		break;
	}
	case IMPS:
	{
		match(IMPS);
		break;
	}
	case INF:
	{
		match(INF);
		break;
	}
	case PCON:
	{
		match(PCON);
		break;
	}
	case PANT:
	{
		match(PANT);
		break;
	}
	case GER:
	{
		match(GER);
		break;
	}
	case PACT:
	{
		match(PACT);
		break;
	}
	case PPAS:
	{
		match(PPAS);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
}

void ANTLRParser::liczba() {
	
	switch ( LA(1)) {
	case SG:
	{
		match(SG);
		break;
	}
	case PL:
	{
		match(PL);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
}

void ANTLRParser::przypadek() {
	
	switch ( LA(1)) {
	case NOM:
	{
		match(NOM);
		break;
	}
	case GEN:
	{
		match(GEN);
		break;
	}
	case DAT:
	{
		match(DAT);
		break;
	}
	case ACC:
	{
		match(ACC);
		break;
	}
	case INST:
	{
		match(INST);
		break;
	}
	case LOC:
	{
		match(LOC);
		break;
	}
	case VOC:
	{
		match(VOC);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
}

void ANTLRParser::rodzaj() {
	
	switch ( LA(1)) {
	case M1:
	{
		match(M1);
		break;
	}
	case M2:
	{
		match(M2);
		break;
	}
	case M3:
	{
		match(M3);
		break;
	}
	case F:
	{
		match(F);
		break;
	}
	case N:
	{
		match(N);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
}

void ANTLRParser::osoba() {
	
	switch ( LA(1)) {
	case PRI:
	{
		match(PRI);
		break;
	}
	case SEC:
	{
		match(SEC);
		break;
	}
	case TER:
	{
		match(TER);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
}

void ANTLRParser::stopien() {
	
	switch ( LA(1)) {
	case POS:
	{
		match(POS);
		break;
	}
	case COMP:
	{
		match(COMP);
		break;
	}
	case SUP:
	{
		match(SUP);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
}

void ANTLRParser::aspekt() {
	
	switch ( LA(1)) {
	case IMPERF:
	{
		match(IMPERF);
		break;
	}
	case PERF:
	{
		match(PERF);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
}

void ANTLRParser::zanegowanie() {
	
	switch ( LA(1)) {
	case AFF:
	{
		match(AFF);
		break;
	}
	case NEG:
	{
		match(NEG);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
}

void ANTLRParser::akcentowosc() {
	
	switch ( LA(1)) {
	case AKC:
	{
		match(AKC);
		break;
	}
	case NAKC:
	{
		match(NAKC);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
}

void ANTLRParser::poprzyimkowosc() {
	
	switch ( LA(1)) {
	case PRAEP:
	{
		match(PRAEP);
		break;
	}
	case NPRAEP:
	{
		match(NPRAEP);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
}

void ANTLRParser::akomodacyjnosc() {
	
	switch ( LA(1)) {
	case CONGR:
	{
		match(CONGR);
		break;
	}
	case REC:
	{
		match(REC);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
}

void ANTLRParser::aglutynacyjnosc() {
	
	switch ( LA(1)) {
	case NAGL:
	{
		match(NAGL);
		break;
	}
	case AGL:
	{
		match(AGL);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
}

void ANTLRParser::wokalicznosc() {
	
	switch ( LA(1)) {
	case WOK:
	{
		match(WOK);
		break;
	}
	case NWOK:
	{
		match(NWOK);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
}

void ANTLRParser::typliczby() {
	
	switch ( LA(1)) {
	case INTEGER:
	{
		match(INTEGER);
		break;
	}
	case FRAC:
	{
		match(FRAC);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
}

void ANTLRParser::initializeASTFactory( ANTLR_USE_NAMESPACE(antlr)ASTFactory& )
{
}
const char* ANTLRParser::tokenNames[] = {
	"<0>",
	"EOF",
	"<2>",
	"NULL_TREE_LOOKAHEAD",
	"an identifier of a macro",
	"\'(\'",
	"\')\'",
	"\',\'",
	"\':\'",
	"\'#\'",
	"\'<\'",
	"\'>\'",
	"\"delete\"",
	"INT",
	"DSEPARATOR",
	"Q_MARK",
	"\'[\'",
	"\']\'",
	"\"prefix\"",
	"\"suffix\"",
	"\"orth\"",
	"\"base\"",
	"\"flex\"",
	"\"catflt\"",
	"\"agrflt\"",
	"\"dispos\"",
	"\"discat\"",
	"\"and\"",
	"\"or\"",
	"\"not\"",
	"\"llook\"",
	"\"rlook\"",
	"\"only\"",
	"\"atleast\"",
	"\"agr\"",
	"\"wagr\"",
	"\"agrpp\"",
	"\"equal\"",
	"\"in\"",
	"\"inter\"",
	"\"isbig\"",
	"\"hasnum\"",
	"\'{\'",
	"\'}\'",
	"\"none\"",
	"a string",
	"\"nmb\"",
	"\"cas\"",
	"\"gnd\"",
	"\"per\"",
	"\"deg\"",
	"\"asp\"",
	"\"acm\"",
	"\"acn\"",
	"\"ppr\"",
	"\"agg\"",
	"\"vcl\"",
	"\"tnt\"",
	"\"adv\"",
	"SIEBIE",
	"\"winien\"",
	"\"pred\"",
	"\"prep\"",
	"\"conj\"",
	"\"qub\"",
	"\"xxs\"",
	"\"xxx\"",
	"\"ign\"",
	"\"interp\"",
	"\"twf\"",
	"\"tnum\"",
	"\"tsym\"",
	"\"tdate\"",
	"\"ttime\"",
	"\"turi\"",
	"\"tmail\"",
	"\"subst\"",
	"\"depr\"",
	"\"num\"",
	"\"numcol\"",
	"\"adj\"",
	"\"adja\"",
	"\"adjp\"",
	"PPRON12",
	"PPRON3",
	"\"fin\"",
	"\"bedzie\"",
	"\"aglt\"",
	"\"praet\"",
	"\"impt\"",
	"\"imps\"",
	"\"inf\"",
	"\"pcon\"",
	"\"pant\"",
	"\"ger\"",
	"\"pact\"",
	"\"ppas\"",
	"\"sg\"",
	"\"pl\"",
	"\"nom\"",
	"\"gen\"",
	"\"dat\"",
	"\"acc\"",
	"\"inst\"",
	"\"loc\"",
	"\"voc\"",
	"\"m1\"",
	"\"m2\"",
	"\"m3\"",
	"\"f\"",
	"\"n\"",
	"\"pri\"",
	"\"sec\"",
	"\"ter\"",
	"\"pos\"",
	"\"comp\"",
	"\"sup\"",
	"\"imperf\"",
	"\"perf\"",
	"\"aff\"",
	"\"neg\"",
	"\"akc\"",
	"\"nakc\"",
	"\"praep\"",
	"\"npraep\"",
	"\"congr\"",
	"\"rec\"",
	"\"nagl\"",
	"\"agl\"",
	"\"wok\"",
	"\"nwok\"",
	"\"integer\"",
	"\"frac\"",
	"\"begin\"",
	"\"current\"",
	"\"end\"",
	"\"nowhere\"",
	"a var",
	"an identifier",
	"WS",
	"COMMENT",
	0
};

const unsigned long ANTLRParser::_tokenSet_0_data_[] = { 256UL, 4227858432UL, 4294967295UL, 1UL, 0UL, 0UL, 0UL, 0UL };
// COLON "adv" SIEBIE "winien" "pred" "prep" "conj" "qub" "xxs" "xxx" "ign" 
// "interp" "twf" "tnum" "tsym" "tdate" "ttime" "turi" "tmail" "subst" 
// "depr" "num" "numcol" "adj" "adja" "adjp" PPRON12 PPRON3 "fin" "bedzie" 
// "aglt" "praet" "impt" "imps" "inf" "pcon" "pant" "ger" "pact" "ppas" 
const ANTLR_USE_NAMESPACE(antlr)BitSet ANTLRParser::_tokenSet_0(_tokenSet_0_data_,8);
const unsigned long ANTLRParser::_tokenSet_1_data_[] = { 130023424UL, 67092480UL, 0UL, 0UL, 0UL, 0UL };
// "flex" "catflt" "agrflt" "dispos" "discat" "nmb" "cas" "gnd" "per" "deg" 
// "asp" "acm" "acn" "ppr" "agg" "vcl" "tnt" 
const ANTLR_USE_NAMESPACE(antlr)BitSet ANTLRParser::_tokenSet_1(_tokenSet_1_data_,6);
const unsigned long ANTLRParser::_tokenSet_2_data_[] = { 0UL, 4294964224UL, 4294967295UL, 4294967295UL, 7UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// LCURLY "none" STRING "nmb" "cas" "gnd" "per" "deg" "asp" "acm" "acn" 
// "ppr" "agg" "vcl" "tnt" "adv" SIEBIE "winien" "pred" "prep" "conj" "qub" 
// "xxs" "xxx" "ign" "interp" "twf" "tnum" "tsym" "tdate" "ttime" "turi" 
// "tmail" "subst" "depr" "num" "numcol" "adj" "adja" "adjp" PPRON12 PPRON3 
// "fin" "bedzie" "aglt" "praet" "impt" "imps" "inf" "pcon" "pant" "ger" 
// "pact" "ppas" "sg" "pl" "nom" "gen" "dat" "acc" "inst" "loc" "voc" "m1" 
// "m2" "m3" "f" "n" "pri" "sec" "ter" "pos" "comp" "sup" "imperf" "perf" 
// "aff" "neg" "akc" "nakc" "praep" "npraep" "congr" "rec" "nagl" "agl" 
// "wok" "nwok" 
const ANTLR_USE_NAMESPACE(antlr)BitSet ANTLRParser::_tokenSet_2(_tokenSet_2_data_,12);
const unsigned long ANTLRParser::_tokenSet_3_data_[] = { 192UL, 4294963200UL, 4294967295UL, 4294967295UL, 7UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// RPAREN COMMA "none" STRING "nmb" "cas" "gnd" "per" "deg" "asp" "acm" 
// "acn" "ppr" "agg" "vcl" "tnt" "adv" SIEBIE "winien" "pred" "prep" "conj" 
// "qub" "xxs" "xxx" "ign" "interp" "twf" "tnum" "tsym" "tdate" "ttime" 
// "turi" "tmail" "subst" "depr" "num" "numcol" "adj" "adja" "adjp" PPRON12 
// PPRON3 "fin" "bedzie" "aglt" "praet" "impt" "imps" "inf" "pcon" "pant" 
// "ger" "pact" "ppas" "sg" "pl" "nom" "gen" "dat" "acc" "inst" "loc" "voc" 
// "m1" "m2" "m3" "f" "n" "pri" "sec" "ter" "pos" "comp" "sup" "imperf" 
// "perf" "aff" "neg" "akc" "nakc" "praep" "npraep" "congr" "rec" "nagl" 
// "agl" "wok" "nwok" 
const ANTLR_USE_NAMESPACE(antlr)BitSet ANTLRParser::_tokenSet_3(_tokenSet_3_data_,12);


ANTLR_END_NAMESPACE
ANTLR_END_NAMESPACE
ANTLR_END_NAMESPACE
