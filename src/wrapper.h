/*
 * PanteraWrapper.h
 *
 *  Created on: 30-11-2010
 *      Author: lennyn
 */

#ifndef PANTERAWRAPPER_H_
#define PANTERAWRAPPER_H_

#include <vector>
#include <string>
#include <nlpcommon/tag.h>
#include <nlpcommon/lexeme.h>

namespace Pantera {

class PanteraWrapper {
public:
    static PanteraWrapper* getInstance();

	virtual std::vector<NLPCommon::DefaultLexeme> tag(const std::string& text);
	virtual NLPCommon::Tagset* getTagset();

private:
	PanteraWrapper();
	virtual ~PanteraWrapper();

	NLPCommon::Tagset* tagset;
	std::vector<const NLPCommon::Tagset*> tagsets;
};
}

#endif /* PANTERAWRAPPER_H_ */
