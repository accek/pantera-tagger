/*
 * PanteraWrapper.h
 *
 *  Created on: 30-11-2010
 *      Author: lennyn
 *
 *  Modified by: bz
 */

#ifndef PANTERAWRAPPER_H_
#define PANTERAWRAPPER_H_

#include <vector>
#include <string>
#include <nlpcommon/tag.h>
#include <nlpcommon/lexeme.h>
#include <taggingoptions.h>

namespace Pantera {

class PanteraWrapper {
public:
    static PanteraWrapper* createInstance(std::string enginePath, std::string tagsetName);
    static PanteraWrapper* getInstance();

    virtual NLPCommon::Tagset* getTagset();
    
    virtual void tag(
        const std::string& text,
        const TaggingOptions& options,
        std::vector<NLPCommon::DefaultLexeme>& result) = 0;
    
    virtual void tag(
        std::vector<NLPCommon::DefaultLexeme>& lexems,
        const TaggingOptions& options) = 0;
    
    // DEPRECATED
    virtual std::vector<NLPCommon::DefaultLexeme> tag(const std::string& text) = 0;
    
    // DEPRECATED
    virtual std::vector<NLPCommon::DefaultLexeme> tag(
        const std::vector<NLPCommon::DefaultLexeme>& lexems, 
        bool doSentSplit, bool doMorphAnalysis, bool doSegmentDisamb, bool doTagging) = 0;

    virtual ~PanteraWrapper() {}

protected:
    PanteraWrapper(std::string tagsetName = "");

    NLPCommon::Tagset* tagset;
    std::vector<const NLPCommon::Tagset*> tagsets;

};

}

#endif /* PANTERAWRAPPER_H_ */
