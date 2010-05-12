#include "../corpuslib_headers.h"
#include "Position.h"

namespace Corpus
{

	CPosition::CPosition(int position)
		:position(position)
	{
	}

	CPosition::~CPosition(void)
	{
	}

	std::string CPosition::ToString()
	{
		if(position == CPosition::nowhere)
			return std::string("nowhere");
		if(position == CPosition::beginning)
			return std::string("beginning");
		if(position == CPosition::end)
			return std::string("end");

		std::stringstream output;
		output << position;

		return output.str();
	}

	void CPosition::SetPos(int newPosition)
	{
		position = newPosition;
	}

	int CPosition::GetPos() const
	{
		return position;
	}
}
