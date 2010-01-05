/*
 * spejdtagsetloader.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef SPEJDTAGSETLOADER_H_
#define SPEJDTAGSETLOADER_H_

#include "tagsetloader.h"
#include "simpleini.h"

namespace BTagger {

class SpejdTagsetLoader : public TagsetLoader
{
private:
    void reportError(SI_Error error);

public:
    virtual Tagset* loadTagset(std::istream& stream);
};

} // namespace BTagger

#endif /* SPEJDTAGSETLOADER_H_ */
