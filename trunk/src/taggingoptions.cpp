/* 
 * File:   taggingoptions.cpp
 * Author: mlenart
 * 
 * Created on 28 maj 2012, 11:53
 */

#include "taggingoptions.h"

namespace Pantera {

TaggingOptions::TaggingOptions() {
    _doSentSplit = true;
    _doMorphAnalysis = true;
    _doSegmentDisamb = true;
    _doTagging = true;
    _useGuesser = false;
    _segmentRules = "";
    _morphDict = "";
    _ignoreSentsInsideSegment = false;
}

TaggingOptions::~TaggingOptions() {
}

TaggingOptions& TaggingOptions::morphDict(std::string morphDict) {
    this->_morphDict = morphDict;
    return *(this);
}

std::string TaggingOptions::morphDict() const {
    return _morphDict;
}

TaggingOptions& TaggingOptions::sentencerRules(
        std::string segmentRules) {
    this->_segmentRules = segmentRules;
    return *(this);
}

std::string TaggingOptions::sentencerRules() const {
    return _segmentRules;
}

TaggingOptions& TaggingOptions::doTagging(bool doTagging) {
    this->_doTagging = doTagging;
    return *(this);
}

bool TaggingOptions::doTagging() const {
    return _doTagging;
}

TaggingOptions& TaggingOptions::doSegmentDisamb(bool doSegmentDisamb) {
    this->_doSegmentDisamb = doSegmentDisamb;
    return *(this);
}

bool TaggingOptions::doSegmentDisamb() const {
    return _doSegmentDisamb;
}

TaggingOptions& TaggingOptions::doMorphAnalysis(bool doMorphAnalysis) {
    this->_doMorphAnalysis = doMorphAnalysis;
    return *(this);
}

bool TaggingOptions::doMorphAnalysis() const {
    return _doMorphAnalysis;
}

TaggingOptions& TaggingOptions::doSentSplit(bool doSentSplit) {
    this->_doSentSplit = doSentSplit;
    return *(this);
}

bool TaggingOptions::doSentSplit() const {
    return _doSentSplit;
}

TaggingOptions& TaggingOptions::useGuesser(bool useGuesser) {
    this->_useGuesser = useGuesser;
    return *(this);
}

bool TaggingOptions::useGuesser() const {
    return _useGuesser;
}

TaggingOptions& TaggingOptions::ignoreSentsInsideSegment(const bool ignore) {
    this->_ignoreSentsInsideSegment = ignore;
}

bool TaggingOptions::ignoreSentsInsideSegment() const {
    return this->_ignoreSentsInsideSegment;
}
}

