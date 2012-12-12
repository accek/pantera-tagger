/*
 * panteratest.cpp
 *
 *  Created on: 30-11-2010
 *      Author: lennyn
 */

#include <cstdlib>
#include <nlpcommon/util.h>
#include <wrapper.h>

using namespace std;
using namespace Pantera;
using namespace NLPCommon;

int main(int argc, char** argv) {
    PanteraWrapper* pantera = PanteraWrapper::getInstance();
    vector<DefaultLexeme> lexems;
    pantera->tag(string("Ala ma kota.\n Miałem. \nMiałem kota.\n\nLecę."), TaggingOptions().useGuesser(false).doSegmentDisamb(false), lexems);
    pantera->tag(string("Zażółć gęślą jaźń."), TaggingOptions(), lexems);
    return 0;
}
