/*
 * progress.cpp
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#include <cstdlib>
#include <nlpcommon/progress.h>

namespace NLPCommon {

HasProgress::HasProgress()
    : progress_handler(NULL), progress_value(0) {
}

void HasProgress::setProgressHandler(void (*progress)(int token), int every) {
    progress_handler = progress;
    progress_every = every;
}

}
