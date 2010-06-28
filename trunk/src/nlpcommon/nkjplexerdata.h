/*
 * nkjplexerdata.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef NKJPLEXTERDATA_H_
#define NKJPLEXTERDATA_H_

#include <boost/regex.hpp>
#include <string>
#include <iostream>
#include <locale>
#include <stack>

#include <nlpcommon/lexer.h>

namespace NLPCommon {

using std::string;

class NKJPIdData : public LexerData {
private:
    string _id;

public:
    NKJPIdData(const string& id)
        : LexerData(), _id(id) { }

    const string& getId() const { return _id; }
};

class NKJPParagraphData : public NKJPIdData {
public:
    NKJPParagraphData(const string& id) : NKJPIdData(id) { }
};

class NKJPSentenceData : public NKJPIdData {
public:
    NKJPSentenceData(const string& id) : NKJPIdData(id) { }
};

class NKJPSegmentData : public NKJPIdData {
private:
    size_t _start, _end;
public:
    NKJPSegmentData(const string& id, size_t start, size_t end)
        : NKJPIdData(id), _start(start), _end(end) { }

    size_t getStart() const { return _start; }
    size_t getEnd() const { return _end; }
};

} // namespace NLPCommon

#endif /* NKJPLEXTERDATA_H_ */
