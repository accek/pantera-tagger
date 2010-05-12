#ifndef CATEGORY_WEIGHTS_H_
#define CATEGORY_WEIGHTS_H_

#include <vector>
#include <string>
#include <utility>

namespace NLPCommon {

using std::string;
using std::vector;

template<class CategoryWeights>
class CategoryWeightsStore
{
private:
	vector<std::pair<string, double> > w_vector;
	double w_vector_sum;

protected:
	CategoryWeightsStore() {
		CategoryWeights::initWeightsVector(w_vector);

		w_vector_sum = 0.0;
		typedef std::pair<string, double> entry_type;
		BOOST_FOREACH(const entry_type& entry, w_vector) {
			w_vector_sum += entry.second;
		}
		std::sort(w_vector.begin(), w_vector.end());
	}

	double getCategoryWeight(const string& name) {
		vector<std::pair<string, double> >::const_iterator i =
			std::lower_bound(w_vector.begin(), w_vector.end(),
					std::make_pair(name, -1.0));
		if (i == w_vector.end() || i->first != name) {
			// We assume one occurence of unknown categories
			return 1.0;
		}
        return i->second;
	}

	double getSumOfCategoryWeights() {
		return w_vector_sum;
	}

};

} // namespace NLPCommon

#endif /* CATEGORY_WEIGHTS_H_ */
