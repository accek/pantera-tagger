/*
 * util.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <string>

// Various utility functions.

namespace BTagger {

using std::string;

// Returns a part of the passed haystack before first occurence of needle.
// Updates haystack to contain only the part after needle.
// If needle is not found, entire haystack is returned and updated to empty
// string.
string find_prefix(string& haystack, char needle);

} // namespace BTagger

#endif /* COMMON_H_ */
