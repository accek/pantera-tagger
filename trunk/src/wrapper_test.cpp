/*
 * panteratest.cpp
 *
 *  Created on: 30-11-2010
 *      Author: lennyn
 */

#include <cstdlib>
#include <nlpcommon/util.h>
#include "wrapper.h"

using namespace std;
using namespace Pantera;

int main(int argc, char** argv) {
	PanteraWrapper* pantera = PanteraWrapper::getInstance();
	pantera->tag("Ala ma kota.\nMiałem kota.\n\nLecę.");
	pantera->tag("Zażółć gęślą jaźń.");
	return 0;
}

