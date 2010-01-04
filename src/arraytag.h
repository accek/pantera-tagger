/*
 * arraytag.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef ARRAYTAG_H_
#define ARRAYTAG_H_

#include <boost/cstdint.hpp>
#include <boost/functional/hash.hpp>
#include <cstring>
#include "basetag.h"

namespace BTagger {

class ArrayTag : public BaseTag<ArrayTag>
{
private:
	static const int max_num_categories = 15;

	// First max_num_categories entries store values for categories.
	// The last entry is part of speech.
	boost::uint8_t _data[max_num_categories + 1];

public:
	void clear() {
		memset(_data, 0, sizeof(_data));
	}

	static const ArrayTag& getNullTag() {
		static ArrayTag tag;
		static bool initialized = false;
		if (!initialized) {
			tag.clear();
			tag.setPos(255);
			initialized = true;
		}
		return tag;
	}

	void setValue(unsigned int category_index, unsigned int value_index) {
		_data[category_index] = value_index;
	}

	unsigned int getValue(unsigned int category_index) const {
		return _data[category_index];
	}

	void setPos(unsigned int pos_index) {
		_data[max_num_categories] = pos_index;
	}

	unsigned int getPos() const {
		return _data[max_num_categories];
	}

	bool operator==(const ArrayTag& other) const {
		return !memcmp(_data, other._data, sizeof(_data));
	}

	bool operator!=(const ArrayTag& other) const {
		return memcmp(_data, other._data, sizeof(_data));
	}

	bool operator<(const ArrayTag& other) const {
		return memcmp(_data, other._data, sizeof(_data)) < 0;
	}

	bool operator<=(const ArrayTag& other) const {
		return memcmp(_data, other._data, sizeof(_data)) <= 0;
	}

	bool operator>(const ArrayTag& other) const {
		return memcmp(_data, other._data, sizeof(_data)) > 0;
	}

	bool operator>=(const ArrayTag& other) const {
		return memcmp(_data, other._data, sizeof(_data)) >= 0;
	}

	std::size_t hash() const {
		return boost::hash_range(_data, _data + max_num_categories + 1);
	}

	// FIXME: this is not portable:
} __attribute__((packed));

std::size_t hash_value(const ArrayTag& tag) {
	return tag.hash();
}

} // namespace BTagger

#endif /* ARRAYTAG_H_ */
