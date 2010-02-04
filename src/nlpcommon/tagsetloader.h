/*
 * tagsetloader.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef TAGSETLOADER_H_
#define TAGSETLOADER_H_

#include <iostream>
#include <nlpcommon/exception.h>
#include <nlpcommon/tagset.h>

namespace NLPCommon {


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
    virtual ~TagsetLoader() { }

    // A method to read tagset specification from a specified
    // stream.
    virtual const Tagset* loadTagset(std::istream& stream) = 0;
};

} // namespace NLPCommon

#endif /* TAGSETLOADER_H_ */
