#ifndef POLIQARP_WEIGHTS_H_
#define POLIQARP_WEIGHTS_H_

#include <nlpcommon/cascorer.h>

namespace NLPCommon {

class PoliqarpWeights
{
public:
	static void initWeightsVector(vector<std::pair<string, double> >& v) {
		v.push_back(std::make_pair<string, double>("pos", 37771.0));
		v.push_back(std::make_pair<string, double>("case", 14055.0));
		v.push_back(std::make_pair<string, double>("number", 2074.0));
		v.push_back(std::make_pair<string, double>("gender", 552.0));
		v.push_back(std::make_pair<string, double>("aspect", 222.0));
		v.push_back(std::make_pair<string, double>("person", 186.0));
		v.push_back(std::make_pair<string, double>("degree", 81.0));
		v.push_back(std::make_pair<string, double>("accommodability", 25.0));
		v.push_back(std::make_pair<string, double>("post-prepositionality", 8.0));
		v.push_back(std::make_pair<string, double>("negation", 7.0));
		v.push_back(std::make_pair<string, double>("accentability", 5.0));
		v.push_back(std::make_pair<string, double>("agglutination", 4.0));
	}
};

} // namespace NLPCommon

#endif /* POLIQARP_WEIGHTS_H_ */
