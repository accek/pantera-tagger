/*
 * lexer.h
 *
 *  Created on: Jan 2, 2010
 *      Author: accek
 */

#ifndef PROGRESS_H_
#define PROGRESS_H_

namespace NLPCommon {

class HasProgress
{
private:
    void (*progress_handler)(int);
    int progress_every;
    int progress_value;

protected:
    inline void advanceProgress() {
        progress_value++;
        if (progress_handler && progress_every &&
                (progress_value % progress_every) == 0)
            progress_handler(progress_value);
    }

public:
    HasProgress();

    void setProgressHandler(void (*progress)(int progress_value), int every);
};


} // namespace NLPCommon

#endif /* PROGRESS_H_ */
