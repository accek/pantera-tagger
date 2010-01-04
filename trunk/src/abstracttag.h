/*
 * tag.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef ABSTRACTTAG_H_
#define ABSTRACTTAG_H_

#include "tagset.h"

namespace BTagger {

// This is only an interface specification. You should not inherit from this
// class to implement tags. Just define a class, which conforms to this
// interface.
//
// All classes, which use tags are templates parameterized by specific
// implementation of AbstractTag interface.

class AbstractTag
{
public:
	AbstractTag(const Tagset* tagset, unsigned int pos_index);

	void setValue(unsigned int category_index, unsigned int value_index);
	unsigned int getValue(unsigned int category_index) const;

	bool operator==(const AbstractTag& other) const;
	bool operator!=(const AbstractTag& other) const;
};

}

#endif /* TAG_H_ */
