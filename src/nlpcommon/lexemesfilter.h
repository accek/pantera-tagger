/*
 * writer.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef LEXEMESFILTER_H_
#define LEXEMESFILTER_H_

#include <istream>
#include <vector>

#include <nlpcommon/lexeme.h>
#include <nlpcommon/progress.h>
#include <nlpcommon/exception.h>

namespace NLPCommon {

using std::vector;

class LexemesFilterException : public Exception
{
public:
    LexemesFilterException(const string& msg) : Exception(msg) { }
    virtual ~LexemesFilterException() throw () { }
};

template<class Lexeme = DefaultLexeme>
class LexemesFilter
{
protected:
	vector<Lexeme> _original_text;
	int _type_mask;

public:
	LexemesFilter(int type_mask) : _type_mask(type_mask) { }

	vector<Lexeme> filterText(vector<Lexeme>& original) {
		this->_original_text = original;
		vector<Lexeme> ret;

		BOOST_FOREACH(const Lexeme& lex, this->_original_text) {
			if (((int)lex.getType() & this->_type_mask) != 0) {
				ret.push_back(lex);
			}
		}
		
		return ret;
	}

	vector<Lexeme> unfilterText(const vector<Lexeme>& text) {
		int i = 0;
		vector<Lexeme> ret;

		BOOST_FOREACH(const Lexeme& lex, this->_original_text) {
			if (((int)lex.getType() & this->_type_mask) != 0) {
				const Lexeme& tlex = text[i++];
				if (lex.getType() != tlex.getType()) {
					throw LexemesFilterException(boost::str(
								boost::format("Lexeme type mismatch at "
									"position %1% while unfiltering text.")
								% i));
				}
				if (lex.getOrth() != tlex.getOrth()) {
					throw LexemesFilterException(boost::str(
								boost::format("Lexeme orth mismatch at "
									"position %1% while unfiltering text.")
								% i));
				}
				ret.push_back(tlex);
			} else {
				ret.push_back(lex);
			}
		}

		return ret;
	}
};


} // namespace NLPCommon

#endif /* LEXEMESFILTER_H_ */
