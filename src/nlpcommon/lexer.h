/*
 * lexer.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef LEXER_H_
#define LEXER_H_

#include <istream>
#include <vector>

#include <nlpcommon/lexeme.h>
#include <nlpcommon/progress.h>

namespace NLPCommon {

template<class Lexeme = DefaultLexeme>
class LexerCollector
{
private:
    size_t _size;
    const Tagset* _tagset;

public:
    LexerCollector() : _size(0), _tagset(NULL) { }

    virtual void collectLexeme(const Lexeme& lexeme) = 0;
    virtual void setTagset(const Tagset* tagset) { _tagset = tagset; }
    virtual const Tagset* getTagset() const { return _tagset; }
    virtual void setSize(size_t size) { _size = size; }
    virtual size_t getSize() const { return _size; }
};

template<class Lexeme = DefaultLexeme>
class VectorLexerCollector : public LexerCollector<Lexeme>
{
private:
    vector<Lexeme>& vec;

public:
    VectorLexerCollector(vector<Lexeme>& vec)
        : LexerCollector<Lexeme>(), vec(vec) { }

    void collectLexeme(const Lexeme& lexeme) {
        vec.push_back(lexeme);
    }
};

template<class Lexeme = DefaultLexeme>
class Lexer : public HasProgress
{
protected:
    std::istream& stream;
    bool quiet;

public:
    Lexer(std::istream& stream) : stream(stream), quiet(false) { }

    void setQuiet(bool value = true) {
        quiet = value;
    }

    virtual void parseStream(LexerCollector<Lexeme>& collector) = 0;

    void parseStreamToVector(std::vector<Lexeme>& tokens, const Tagset** tagset = NULL)
    {
        tokens.clear();
        VectorLexerCollector<Lexeme> collector(tokens);
        if (tagset != NULL)
            collector.setTagset(*tagset);
        parseStream(collector);
        if (tagset != NULL)
            *tagset = collector.getTagset();
    }
};

} // namespace NLPCommon

#endif /* LEXER_H_ */
