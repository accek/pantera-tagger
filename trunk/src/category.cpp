/*
 * category.cpp
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#include <boost/format.hpp>
#include "category.h"

namespace BTagger {

using boost::format;
using boost::str;

CategoryValueNotFoundException CategoryValueNotFoundException::ById(
        const Category& cat, unsigned int id) {
    return CategoryValueNotFoundException(
            str(format("Value #%1% does not exist in category '%2%'") % id
                    % cat.getName()));
}

CategoryValueNotFoundException CategoryValueNotFoundException::ByValue(
        const Category& cat, const string& name) {
    return CategoryValueNotFoundException(str(format(
            "Value '%1%' does not exist in category '%2%'.") % name
            % cat.getName()));
}

}
