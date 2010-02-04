/*
 * spejdtagsetloader.cpp
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#include <boost/format.hpp>
#include <memory>
#include <iostream>
#include <sstream>
#include <iterator>

#include <nlpcommon/spejdtagsetloader.h>

#include "simpleini.h"

using boost::format;
using boost::str;
using std::auto_ptr;

namespace NLPCommon {

Tagset* SpejdTagsetLoader::loadTagset(std::istream& stream) {
    CSimpleIniA parser;
    SI_Error err;

    err = parser.Load(stream);
    if (err != SI_OK)
        throw TagsetLoaderError(str(
                format("INI parser returned error %2%.") % (int)err));

    auto_ptr<Tagset> tagset(new Tagset());

    const CSimpleIniA::TKeyVal* categories = parser.GetSection("ATTR");
    typedef CSimpleIni::TKeyVal::const_iterator section_iterator;
    for (section_iterator i = categories->begin();
            i != categories->end(); ++i) {
        Category* category = new Category(i->first.pItem);
        std::istringstream values_stream(i->second);
        while (!values_stream.eof()) {
            string value;
            values_stream >> value;
            category->addValue(value);
        }
        tagset->addCategory(category);
    }

    const CSimpleIniA::TKeyVal* poss = parser.GetSection("POS");
    for (section_iterator i = poss->begin();
            i != poss->end(); ++i) {
        PartOfSpeech* pos = new PartOfSpeech(i->first.pItem);
        std::istringstream values_stream(i->second);
        while (!values_stream.eof()) {
            string value;
            bool required = true;

            values_stream >> value;

            if (value.empty())
                continue;

            if (value[0] == '[' && value[value.length() - 1] == ']') {
                value = value.substr(1, value.length() - 2);
                required = false;
            }

            pos->addCategory(tagset->getCategory(value), required);
        }
        tagset->addPartOfSpeech(pos);
    }

    return tagset.release();
}

}
