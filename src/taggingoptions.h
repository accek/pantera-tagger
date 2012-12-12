/* 
 * File:   taggingoptions.h
 * Author: mlenart
 *
 * Created on 28 maj 2012, 11:53
 */

#ifndef TAGGINGOPTIONS_H
#define TAGGINGOPTIONS_H

#include <string>
#include <istream>
#include <taggingoptions.h>

namespace Pantera {

class TaggingOptions {
    
public:
    TaggingOptions();
    virtual ~TaggingOptions();
    
    TaggingOptions& morphDict(std::string morphDict);
    std::string morphDict() const;
    
    TaggingOptions& sentencerRules(std::string segmentRules);
    std::string sentencerRules() const;
    
    TaggingOptions& doTagging(bool doTagging);
    bool doTagging() const;
    
    TaggingOptions& doSegmentDisamb(bool doSegmentDisamb);
    bool doSegmentDisamb() const;
    
    TaggingOptions& doMorphAnalysis(bool doMorphAnalysis);
    bool doMorphAnalysis() const;
    
    TaggingOptions& doSentSplit(bool doSentSplit);
    bool doSentSplit() const;

    TaggingOptions& useGuesser(bool useGuesser);
    bool useGuesser() const;

    TaggingOptions& ignoreSentsInsideSegment(const bool ignore);
    bool ignoreSentsInsideSegment() const;
    
private:
    bool _doSentSplit;
    bool _doMorphAnalysis;
    bool _doSegmentDisamb;
    bool _doTagging;
    bool _useGuesser;
    bool _ignoreSentsInsideSegment;
    
    std::string _segmentRules;
    std::string _morphDict;
};
}

#endif  /* TAGGINGOPTIONS_H */

