/*
 * tagsetloader.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef TAGSETLOADER_H_
#define TAGSETLOADER_H_

#include <iostream>
#include "exception.h"
#include "tagset.h"

namespace BTagger {


class TagsetLoaderError : public Exception
{
public:
	TagsetLoaderError(const string& msg) : Exception(msg) { }
	virtual ~TagsetLoaderError() throw () { };
};


// An abstract base class representing tagset loading classes.
class TagsetLoader
{
public:
	// A method to read tagset specification from a specified
	// stream.
	virtual Tagset* loadTagset(std::istream& stream) = 0;
};

} // namespace BTagger

#endif /* TAGSETLOADER_H_ */
