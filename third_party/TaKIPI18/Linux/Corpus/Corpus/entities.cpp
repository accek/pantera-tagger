#include "../corpuslib_headers.h"
#include "entities.h"

namespace Corpus
{
	void DecodeEntities(std::string &cx_input)
	{
		size_t indx = cx_input.find_first_of('&');
		if(indx == std::string::npos) // no entities
			return;

		size_t ilen = cx_input.size();
		std::stringstream output;
		output << cx_input.substr(0, indx);
		while(indx < ilen)
		{
			char decoded = cx_input[indx];
			if(decoded == '&')
			{
				if(cx_input.compare(indx + 1, 3, "lt;") == 0)
				{
					decoded = '<';
					indx += 3;
				}
				else if(cx_input.compare(indx + 1, 3, "gt;") == 0)
				{
					decoded = '>';
					indx += 3;
				}
				else if(cx_input.compare(indx + 1, 4, "amp;") == 0)
				{
					decoded = '&';
					indx += 4;
				}
				else if(cx_input.compare(indx + 1, 5,"quot;") == 0)
				{
					decoded = '"';
					indx += 5;
				}
				else if(cx_input.compare(indx + 1, 5, "apos;") == 0)
				{
					decoded = '\'';
					indx += 5;
				}
			}
			output << decoded;
			indx++;
		}
		cx_input = output.str();
	} // DecodeEntities
	
	void EncodeEntities(std::string &cx_input)
	{
		size_t len = cx_input.size(), pos;
		for(pos = 0; pos < len; pos++)
		{
			char cp_atpos = cx_input[pos];
			if(cp_atpos == '<'
				|| cp_atpos == '>'
				|| cp_atpos == '&'
				|| cp_atpos == '"'
				|| cx_input[pos] == '\'')
			break;
		}

		if(pos < len)
		{
			std::ostringstream cx_inputbuff;
			cx_inputbuff << cx_input.substr(0, pos);
			for(; pos < len; pos++)
			{
				char cp_atpos = cx_input[pos];
				switch(cp_atpos)
				{
				case '<':
					cx_inputbuff << "&lt;";
					break;
				case '>':
					cx_inputbuff << "&gt;";
					break;
				case '&':
					cx_inputbuff << "&amp;";
					break;
				case '"':
					cx_inputbuff << "&quot;";
					break;
				case '\'':
					cx_inputbuff << "&apos;";
					break;
				default:
					cx_inputbuff << cp_atpos;
				}
			}
			cx_input = cx_inputbuff.str();
		}
	} // EncodeEntities
	
} // namespace Corpus
