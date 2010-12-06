/*
 * tagset.cpp
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#include <boost/format.hpp>
#include <boost/filesystem/fstream.hpp>
#include <map>

#include <nlpcommon/tagset.h>
#include <nlpcommon/spejdtagsetloader.h>

namespace NLPCommon {

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

vector<const Tagset*> load_tagsets(const string& name) {
    static std::map<string, vector<const Tagset*> > cache;
    std::map<string, vector<const Tagset*> >::const_iterator i
        = cache.find(name);
    if (i != cache.end()) {
        return i->second;
    }

    // this is for the libpantera, which may not have options initialized
    string tagsetPath = options.count("tagset-path")
    		? options["tagset-path"].as<string>()
    		: DEFAULT_TAGSET_PATH;
    fs::path filepath = find_with_path(tagsetPath, name);

    vector<const Tagset*> tagsets;
    SpejdTagsetLoader tagset_loader;
    if (fs::is_directory(filepath)) {
        for (int i = 1; ; i++) {
            fs::path tagset_cfg = filepath
                / boost::str(boost::format("%d.cfg") % i);
            if (!fs::exists(tagset_cfg)) {
                if (i == 1) {
                    throw Exception(boost::str(boost::format(
                                    "Found tagset directory '%1%', but no "
                                    "tagset file found. Expected '1.cfg'.")
                                % filepath));
                }
                break;
            }

            fs::ifstream config_stream(tagset_cfg);
            tagsets.push_back(tagset_loader.loadTagset(config_stream));
        }
    } else {
        fs::ifstream config_stream(filepath);
        tagsets.push_back(tagset_loader.loadTagset(config_stream));
    }

    cache.insert(std::make_pair(name, tagsets));

    return tagsets;
}

const Tagset* load_tagset(const string& name) {
    vector<const Tagset*> tagsets = load_tagsets(name);
    return tagsets[tagsets.size() - 1];
}

const po::options_description& tagset_options() {
    static bool initialized = false;
    static po::options_description options("Tagset options");
    if (!initialized) {
        options.add_options()
            ("tagset-path", po::value<string>()
             ->default_value(DEFAULT_TAGSET_PATH),
             "path to a folder with tagset definitions")
            ;
        initialized = true;
    }
    return options;
}

}
