#include "../corpuslib_headers.h"
#include "Value.h"

namespace Corpus
{

	CValue::CValue(DLONG f, DLONG s)
	{
		first = f;
		second = s;
	}

	CValue::~CValue(void)
	{
	}

}
