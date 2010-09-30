#ifndef SEGM_DISAMB_H_
#define SEGM_DISAMB_H_

#include <vector>
#include <nlpcommon/lexeme.h>

namespace NLPCommon {

template<class Lexeme>
class SegmDisambiguator
{
public:
    SegmDisambiguator() { }

    virtual void disambiguateSegmentation(std::vector<Lexeme>& text) = 0;
};

} // namespace NLPCommon

#endif /* SEGM_DISAMB_H_ */
