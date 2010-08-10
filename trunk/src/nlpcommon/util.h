/*
 * util.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <string>
#include <vector>
#include <ostream>
#include <algorithm>
#include <utility>
#include <functional>

// Various utility functions.

namespace NLPCommon {

using std::string;
using std::wstring;
using std::vector;
namespace fs = boost::filesystem;
namespace po = boost::program_options;

class Tagset;


// Returns a part of the passed haystack before first occurence of needle.
// Updates haystack to contain only the part after needle.
// If needle is not found, entire haystack is returned and updated to empty
// string.
string find_prefix(string& haystack, char needle);

// String - wstring conversion
wstring ascii_to_wstring(const string& wstring);
wstring utf8_to_wstring(const string& s);
string wstring_to_utf8(const wstring& s);


// Output a difference between actual and expected tags on one token
// in a human-readable way.
template<class Tag>
void writeTagsDifference(std::wostream& stream,
        const Tagset* tagset,
        const vector<Tag>& actual_tags,
        const vector<Tag>& expected_tags) {
    BOOST_FOREACH(const Tag& tag, actual_tags) {
        stream << ' ';
        if (std::find(expected_tags.begin(), expected_tags.end(), tag) ==
                expected_tags.end())
            stream << '*';
        stream << tag.asWString(tagset);
    }
    BOOST_FOREACH(const Tag& tag, expected_tags)
        if (std::find(actual_tags.begin(), actual_tags.end(), tag) ==
                actual_tags.end())
            stream << " [" << tag.asWString(tagset) << ']';
}

// Comparator functions for std::pair objects which compares
// only the first elements.
template<class Pair>
bool less_first(const Pair& a, const Pair& b) {
    return std::less<typename Pair::first_type>()(a.first, b.first);
}
template<class Pair>
bool greater_first(const Pair& a, const Pair& b) {
    return std::greater<typename Pair::first_type>()(a.first, b.first);
}

// Utility path functions.

fs::path find_with_path(const string& path, const string& filename);

} // namespace NLPCommon

extern boost::program_options::variables_map options;

#endif /* COMMON_H_ */
