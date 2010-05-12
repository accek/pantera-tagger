#ifndef INC_ANTLRLexer_hpp_
#define INC_ANTLRLexer_hpp_


#include "../../joskipi_headers.h"
#include "../../all_operators.h"
#include "../../all_rules.h"
#include "../../all_patterns.h"

#include <antlr/config.hpp>
/* $ANTLR 2.7.6 (20071205): "joskipi.g" -> "ANTLRLexer.hpp"$ */
#include <antlr/CommonToken.hpp>
#include <antlr/InputBuffer.hpp>
#include <antlr/BitSet.hpp>
#include "ANTLRParserTokenTypes.hpp"
#include <antlr/CharScanner.hpp>
ANTLR_BEGIN_NAMESPACE(JOSKIPI)
ANTLR_BEGIN_NAMESPACE(Parsers)
ANTLR_BEGIN_NAMESPACE(antlrgen)

ANTLR_USING_NAMESPACE(std)
ANTLR_USING_NAMESPACE(antlr)
using namespace Operators;
using namespace Rules;
using namespace Patterns;
using namespace Corpus;

class CUSTOM_API ANTLRLexer : public ANTLR_USE_NAMESPACE(antlr)CharScanner, public ANTLRParserTokenTypes
{
private:
	void initLiterals();
public:
	bool getCaseSensitiveLiterals() const
	{
		return true;
	}
public:
	ANTLRLexer(ANTLR_USE_NAMESPACE(std)istream& in);
	ANTLRLexer(ANTLR_USE_NAMESPACE(antlr)InputBuffer& ib);
	ANTLRLexer(const ANTLR_USE_NAMESPACE(antlr)LexerSharedInputState& state);
	ANTLR_USE_NAMESPACE(antlr)RefToken nextToken();
	public: void mPPRON3(bool _createToken);
	public: void mPPRON12(bool _createToken);
	public: void mINT(bool _createToken);
	public: void mQ_MARK(bool _createToken);
	public: void mLBRACKET(bool _createToken);
	public: void mRBRACKET(bool _createToken);
	public: void mLPAREN(bool _createToken);
	public: void mRPAREN(bool _createToken);
	public: void mLCURLY(bool _createToken);
	public: void mRCURLY(bool _createToken);
	public: void mCOMMA(bool _createToken);
	public: void mZMIENNA(bool _createToken);
	public: void mSTRING(bool _createToken);
	public: void mID(bool _createToken);
	public: void mWS(bool _createToken);
	public: void mCOMMENT(bool _createToken);
	public: void mHASH(bool _createToken);
	public: void mDSEPARATOR(bool _createToken);
	public: void mMACROID(bool _createToken);
	public: void mLCHEVRON(bool _createToken);
	public: void mRCHEVRON(bool _createToken);
	public: void mCOLON(bool _createToken);
private:
	
	static const unsigned long _tokenSet_0_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_0;
	static const unsigned long _tokenSet_1_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_1;
	static const unsigned long _tokenSet_2_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_2;
};

ANTLR_END_NAMESPACE
ANTLR_END_NAMESPACE
ANTLR_END_NAMESPACE
#endif /*INC_ANTLRLexer_hpp_*/
