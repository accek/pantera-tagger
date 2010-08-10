/*
 * util.cpp
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#include <boost/program_options/detail/convert.hpp>
#include <boost/program_options/detail/utf8_codecvt_facet.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/format.hpp>
#include <string>
#include <algorithm>

#include <nlpcommon/util.h>
#include <nlpcommon/exception.h>

namespace NLPCommon {

string find_prefix(string& haystack, char needle) {
    std::size_t index = haystack.find(needle);
    string ret;
    if (index == string::npos) {
        ret = haystack;
        haystack = "";
    } else {
        ret = haystack.substr(0, index);
        haystack = haystack.substr(index + 1);
    }
    return ret;
}

wstring ascii_to_wstring(const string& wstring) {
    std::wstring res;
    res.resize(wstring.length());
    std::copy(wstring.begin(), wstring.end(), res.begin());
    return res;
}

wstring utf8_to_wstring(const string& s) {
    boost::program_options::detail::utf8_codecvt_facet utf8_facet;
    return boost::from_8_bit(s, utf8_facet);
}

string wstring_to_utf8(const wstring& s) {
    boost::program_options::detail::utf8_codecvt_facet utf8_facet;
    return boost::to_8_bit(s, utf8_facet);
}

fs::path find_with_path(const string& path, const string& filename) {
    if (fs::exists(filename))
        return fs::path(filename);

    vector<string> dirs;
    boost::split(dirs, path, boost::is_any_of(":"));
    for (vector<string>::iterator i = dirs.begin();
            i != dirs.end(); ++i) {
        fs::path candidate = fs::path(*i) / filename;
        if (fs::exists(candidate))
            return candidate;
    }

    throw Exception(boost::str(
                boost::format("Cannot find file '%1%' at path '%2%'")
                % filename % path));
}

}

