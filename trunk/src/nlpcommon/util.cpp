/*
 * util.cpp
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#include <boost/program_options/detail/utf8_codecvt_facet.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/format.hpp>
#include <boost/scoped_array.hpp>
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

wstring ascii_to_wstring(const string& str) {
    std::wstring res;
    res.resize(str.length());
    std::copy(str.begin(), str.end(), res.begin());
    return res;
}

wstring utf8_to_wstring(const string& s) {
    std::locale& locale = get_utf8_locale();
    std::mbstate_t state;

    const std::codecvt<wchar_t, char, std::mbstate_t>& utf8_facet = 
        std::use_facet<std::codecvt<wchar_t, char, std::mbstate_t> >(locale);

    const char* pstr = s.c_str();
    const size_t size = s.length();
    boost::scoped_array<wchar_t> pwstr(new wchar_t[size + 1]);

    const char* pc;
    wchar_t* pwc;

    // translate characters:
    std::codecvt<wchar_t, char, std::mbstate_t>::result result =
        utf8_facet.in(state, pstr, pstr + size, pc,
            pwstr.get(), pwstr.get() + size, pwc);
    *pwc = L'\0';

    if (result != std::codecvt<wchar_t, char, std::mbstate_t>::ok) {
        throw Exception(boost::str(boost::format(
                        "Error %1% converting from UTF-8 to std::wstring") %
                    (int)result));
    } 

    return wstring(pwstr.get());
}

string wstring_to_utf8(const wstring& s) {
    std::locale& locale = get_utf8_locale();
    std::mbstate_t state;

    const std::codecvt<wchar_t, char, std::mbstate_t>& utf8_facet = 
        std::use_facet<std::codecvt<wchar_t, char, std::mbstate_t> >(locale);

    const wchar_t* pwstr = s.c_str();
    const size_t size = s.length();
    boost::scoped_array<char> pstr(new char[4 * (size + 1)]);

    const wchar_t* pwc;
    char* pc;

    // translate characters:
    std::codecvt<wchar_t, char, std::mbstate_t>::result result =
        utf8_facet.out(state, pwstr, pwstr + size + 1, pwc,
            pstr.get(), pstr.get() + 4 * (size + 1), pc);
    *pc = '\0';

    if (result != std::codecvt<wchar_t, char, std::mbstate_t>::ok) {
        throw Exception(boost::str(boost::format(
                        "Error %1% converting from std::wstring to UTF-8") %
                    (int)result));
    } 

    return string(pstr.get());
}

string wstring_to_xml(const wstring& s) {
    wstring buf(s);
    boost::replace_all(buf, L"&", L"&amp;");
    boost::replace_all(buf, L"\"", L"&quot;");
    boost::replace_all(buf, L"'", L"&apos;");
    boost::replace_all(buf, L"<", L"&lt;");
    boost::replace_all(buf, L">", L"&gt;");
    return wstring_to_utf8(buf);
}

wstring xml_to_wstring(const string& s) {
    string buf(s);
    boost::replace_all(buf, "&quot;", "\"");
    boost::replace_all(buf, "&apos;", "'");
    boost::replace_all(buf, "&lt;", "<");
    boost::replace_all(buf, "&gt;", ">");
    boost::replace_all(buf, "&amp;", "&");
    return utf8_to_wstring(buf);
}

wstring wxml_to_wstring(const wstring& s) {
    wstring buf(s);
    boost::replace_all(buf, L"&quot;", L"\"");
    boost::replace_all(buf, L"&apos;", L"'");
    boost::replace_all(buf, L"&lt;", L"<");
    boost::replace_all(buf, L"&gt;", L">");
    boost::replace_all(buf, L"&amp;", L"&");
    return buf;
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

std::locale& get_locale(const char* name) {
    static std::map<string, std::locale> locales;
    std::map<string, std::locale>::iterator i = locales.find(name);
    if (i == locales.end()) {
        std::locale l;
        try {
            l = std::locale(name);
        } catch (...) {
            const char* human_name = name[0] == '\0' ? "(default)" : name;
            std::cerr << "Warning: system does not support required locale '"
                << human_name << "'. We will continue with the '"
                << l.name() << "' locale, but things may not work as "
                "expected." << std::endl;
        }
        locales.insert(std::make_pair(string(name), l));
    }
    return locales[string(name)];
}

std::locale& get_utf8_locale() {
    static std::locale utf8_locale;
    bool initialized = false;

    if (!initialized) {
        try {
            // Commented code below does not work correctly on
            // chopin.ipipan.waw.pl, for unknown reason.
            // Uncommented code is a woraround, in which we use some
            // implementation detail classes from Boost which we shouldn't use.
            //typedef std::codecvt_byname<wchar_t, char, std::mbstate_t> Cvt;
            //utf8_locale = std::locale(std::locale::classic(),
            //           new Cvt("en_US.UTF-8"));
            utf8_locale = std::locale(std::locale::classic(),
                        new boost::program_options::detail::utf8_codecvt_facet);
        } catch (...) {
            throw Exception("System does not support "
                "required 'en_US.UTF-8' locale.");
        }
        initialized = true;
    }
    return utf8_locale;
}


} // namespace NLPCommon
