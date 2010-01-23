/*
 * exception.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include <string>
#include <exception>

namespace NLPCommon {

using std::string;

class Exception : public std::exception
{
private:
    string _message;

public:
    Exception(const string& message) : _message(message) { }
    virtual ~Exception() throw () { }

    virtual const char* what() const throw () {
        return _message.c_str();
    }
};

} // namespace NLPCommon

#endif /* EXCEPTION_H_ */
