/*
 * PanteraWrapper.h
 *
 *  Created on: 30-11-2010
 *      Author: lennyn
 *
 *  Modified by: bz
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
  static PanteraWrapper* createInstance(std::string enginePath, std::string tagsetName);
  static PanteraWrapper* getInstance();

	virtual std::vector<NLPCommon::DefaultLexeme> tag(const std::string& text) = 0;
	virtual NLPCommon::Tagset* getTagset();

	virtual std::vector<NLPCommon::DefaultLexeme> tag(const
      std::vector<NLPCommon::DefaultLexeme>& lexems, bool doSentSplit,
      bool doMorphAnalysis, bool doSegmentDisamb, bool doTagging) = 0;

protected:
  PanteraWrapper(std::string tagsetName = "");

	NLPCommon::Tagset* tagset;
	std::vector<const NLPCommon::Tagset*> tagsets;

};
}

#endif /* PANTERAWRAPPER_H_ */
