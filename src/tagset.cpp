/*
 * tagset.cpp
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#include <boost/format.hpp>
#include "tagset.h"

namespace BTagger {

using boost::format;
using boost::str;

CategoryNotFoundException CategoryNotFoundException::ById(unsigned int id) {
    return CategoryNotFoundException(
            str(format("Category #%1% does not exist.") % id));
}

CategoryNotFoundException CategoryNotFoundException::ByName(
        const string& name) {
    return CategoryNotFoundException(
            str(format("Category '%1%' does not exist.") % name));
}

PartOfSpeechNotFoundException PartOfSpeechNotFoundException::ById(
        unsigned int id) {
    return PartOfSpeechNotFoundException(
            str(format("Part of speech #%1% does not exist.") % id));
}

PartOfSpeechNotFoundException PartOfSpeechNotFoundException::ByName(
        const string& name) {
    return PartOfSpeechNotFoundException(
            str(format("Part of speech '%1%' does not exist.") % name));
}

}
