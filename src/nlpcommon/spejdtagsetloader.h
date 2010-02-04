/*
 * spejdtagsetloader.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef SPEJDTAGSETLOADER_H_
#define SPEJDTAGSETLOADER_H_

#include <nlpcommon/tagsetloader.h>

namespace NLPCommon {

class SpejdTagsetLoader : public TagsetLoader
{
public:
    virtual Tagset* loadTagset(std::istream& stream);
};

} // namespace NLPCommon

#endif /* SPEJDTAGSETLOADER_H_ */
