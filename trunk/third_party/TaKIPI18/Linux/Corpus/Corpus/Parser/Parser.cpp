#include "../../corpuslib_headers.h"
#include "../tagexception.h"

#include "Parser.h"


namespace Corpus
{

	namespace Parser
	{
		const char *GetEncodingName(Encoding encoding, bool translit)
		{
			switch(encoding)
			{
			case utf8:
				return translit ? "UTF-8//TRANSLIT" : "UTF-8";
			case cp1250:
				return translit ? "CP1250//TRANSLIT" : "CP1250";
			case iso88592:
				return translit ? "ISO-8859-2//TRANSLIT" : "ISO-8859-2";
			case cp852:
				// uwaga: nieobs³ugiwane przez Morfeusza
				return translit ? "CP852//TRANSLIT" : "CP852";
			default:
				// nieobs³ugiwane przez ca³y projekt Corpus
				return "";
			}
		}

		/// Deprecated, avoid using it.
		std::string TrimStart(std::string string)
		{
			std::basic_string <char>::iterator strp_Iter;
			for(strp_Iter = string.begin ();
				strp_Iter != string.end () && isspace((*strp_Iter)&0xFF);
				strp_Iter++)
				;
			string.erase(string.begin(),strp_Iter);
			return string;
		}

		/// Deprecated, avoid using it.
		std::string TrimEnd(std::string s)
		{
			std::basic_string <char>::iterator strp_Iter;
			for(strp_Iter = s.end ();
				strp_Iter != s.begin () && isspace((*(strp_Iter-1))&0xFF);
				strp_Iter--)
				;
			s.erase(strp_Iter,s.end());
			return s;
		}

		/// Deprecated, use boost::algorithm::trim.
		std::string Trim(std::string s)
		{
			return TrimStart(TrimEnd(s));
		}

		// "zjada" podany tag xml ze strumienia - najpierw go szukajï¿½c
		// rzuca wyjï¿½tkami, jeï¿½li nie spotka taga na drodze
		// np. "tag" szuka "<tag>", a "/tag" zjada "</tag>"
		// zwraca to, co przeczytano do '<' bez wiodacych i konczacych spacji
		// dataBeforeTag - bufor na dane wczytane, jest czyszczony na poczatek
		std::string EatXmlTag(std::istream &input, const char * tag, std::string& dataBeforeTag)
		{
			dataBeforeTag.erase();
			const int bufsize = 4095;
			//int taglen = (int)strlen(tag);
			std::char_traits<char>::int_type c = input.get();
			char buffer[bufsize+1];
			memset(buffer,0,bufsize+1);
			int read = 0;
			while(c!='<' && c != std::char_traits<char>::eof())
			{
				buffer[read++] = static_cast<char> (std::char_traits<char>::to_char_type(c));
				if(read >= bufsize)
				{
					dataBeforeTag.append(buffer);
					memset(buffer,0,bufsize+1);
					read = 0;
				}

				c = input.get();
			}
			dataBeforeTag.append(buffer);
			if(c != '<')
			{
				throw tagexception(std::string("Podczas proby odczytu <").append(tag).append("> nie znaleziono '<'! Odczytano:\n").append(dataBeforeTag).c_str());
			}
			memset(buffer,0,bufsize);
			input.getline(buffer,bufsize,'>');

#ifdef WIN32
			if(strlen(buffer) != strlen(tag) && !strnicmp(buffer,tag,strlen(buffer)))
				throw tagexception(std::string("Oczekiwano tagu <").append(tag).append(">! Odczytano: <").append(buffer).c_str());
#else
			if(strlen(buffer) != strlen(tag) && !strncasecmp(buffer,tag,bufsize))
			{
				throw tagexception(std::string("Oczekiwano tagu <").append(tag).append(">! Odczytano: <").append(buffer).c_str());
			}
#endif
			boost::algorithm::trim(dataBeforeTag);

			return dataBeforeTag;
		}


		// pobiera tag xml ze strumienia wejsciowego
		// gotTag - jaki tag pobrano, zawartosc miedzy '<' a '>', czyszczony na poczatek, ograniczenie na rozmiar tagu - 255
		// dataBeforeTag - jakie dane znaleziono przed '<', bufor czyszczony na poczatek
		void GetXmlTag(std::istream &input,std::string& gotTag,std::string& dataBeforeTag)
		{
			dataBeforeTag.erase();
			const int bufsize = 4095;
			std::char_traits<char>::int_type c = input.get();
			char buffer[bufsize+1];
			memset(buffer,0,bufsize+1);
			int read = 0;
			while(c!='<' && c !=std::char_traits<char>::eof())
			{
				buffer[read++] = static_cast<char> (std::char_traits<char>::to_char_type(c));
				if(read >= bufsize)
				{
					dataBeforeTag.append(buffer);
					memset(buffer,0,bufsize+1);
					read = 0;
				}
				c = input.get();
			}
			//dataBeforeTag = Trim(dataBeforeTag.append(buffer));
			dataBeforeTag += buffer;
			boost::algorithm::trim(dataBeforeTag);
			
			if(c != '<')
				throw tagexception(std::string("Podczas proby odczytu tagu nie znaleziono '<'! Odczytano:\n").append(dataBeforeTag).c_str());

			//memset(buffer,0,bufsize);
			//input.getline(buffer,bufsize,'>');
			getline(input,gotTag,'>');
			//Trim(gotTag.assign(buffer));
		}

		// zjada se strumienia puste znaki
		void EatWhites(std::istream & input)
		{
			while(isspace(input.peek()))
				input.get();
		}

		int BitCount(DLONG x)
		{
			int b;
			for(b = 0; x!=0; x/=2)
				if(x & 01)
					b++;
			return b;
		}

		void Tokenize(const std::string& str,
			std::vector<std::string>& tokens,
			const std::string& delimiters)
		{
			// We must clear std::vector first
			tokens.clear();

			// Skip delimiters at beginning.
			std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
			// Find first "non-delimiter".
			std::string::size_type pos     = str.find_first_of(delimiters, lastPos);

			while (std::string::npos != pos || std::string::npos != lastPos)
			{
				// Found a token, add it to the std::vector.
				std::string substr  = str.substr(lastPos, pos - lastPos);
				//std::string trimmed = Trim(substr);
				boost::algorithm::trim(substr);
				tokens.push_back(substr);

				// Skip delimiters.  Note the "not_of"
				lastPos = str.find_first_not_of(delimiters, pos);
				// Find next "non-delimiter"
				pos = str.find_first_of(delimiters, lastPos);
			}
		}

		std::string Convert(std::string src, Encoding inputEncoding, Encoding outputEncoding)
		{
			if(inputEncoding == outputEncoding || src.empty())
				return src;
#ifdef WIN32

			WCHAR* buff = new WCHAR[(int)src.length()+1];
			char* buff2 = new char[1024];
			memset(buff,0,sizeof(WCHAR)*((int)src.length()+1));
			memset(buff2,0,sizeof(char)*1024);

			switch(inputEncoding)
			{
			case utf8:
				if(outputEncoding == cp1250)
				{
					MultiByteToWideChar(CP_UTF8,0,src.c_str(),(int)src.length(),buff,(int)src.length()+1);
					WideCharToMultiByte(1250,0,buff,(int)src.length(),buff2,(int)src.length(),NULL,NULL);
				}
				else if(outputEncoding == iso88592) // iso88592
				{
					MultiByteToWideChar(CP_UTF8,0,src.c_str(),(int)src.length(),buff,(int)src.length()+1);
					WideCharToMultiByte(28592,0,buff,(int)src.length(),buff2,(int)src.length(),NULL,NULL);
				}
				else // Win OEM / cp852
				{
					MultiByteToWideChar(CP_UTF8,0,src.c_str(),(int)src.length(),buff,(int)src.length()+1);
					WideCharToMultiByte(CP_OEMCP,0,buff,(int)src.length(),buff2,(int)src.length(),NULL,NULL);
				}
				break;
			case cp1250:
				if(outputEncoding == utf8)
				{
					MultiByteToWideChar(1250,0,src.c_str(),(int)src.length(),buff,(int)src.length());
					int size = WideCharToMultiByte(CP_UTF8,0,buff,(int)src.length()+1,0,0,NULL,NULL);
					delete [] buff2;
					buff2 = new char[size];
					WideCharToMultiByte(CP_UTF8,0,buff,(int)src.length()+1,buff2,size,NULL,NULL);
				}
				else if(outputEncoding == iso88592) // iso88592
				{
					MultiByteToWideChar(1250,0,src.c_str(),(int)src.length(),buff,(int)src.length()+1);
					WideCharToMultiByte(28592,0,buff,(int)src.length(),buff2,(int)src.length(),NULL,NULL);
				}
				else // Win OEM / cp852
				{
					MultiByteToWideChar(1250,0,src.c_str(),(int)src.length(),buff,(int)src.length()+1);
					WideCharToMultiByte(CP_OEMCP,0,buff,(int)src.length(),buff2,(int)src.length(),NULL,NULL);
				}
				break;
			case iso88592:
				if(outputEncoding == cp1250)
				{
					MultiByteToWideChar(28592,0,src.c_str(),(int)src.length(),buff,(int)src.length()+1);
					WideCharToMultiByte(1250,0,buff,(int)src.length(),buff2,(int)src.length(),NULL,NULL);
				}
				else if(outputEncoding == cp852)// Win OEM / cp852
				{
					MultiByteToWideChar(28592,0,src.c_str(),(int)src.length(),buff,(int)src.length()+1);
					WideCharToMultiByte(CP_OEMCP,0,buff,(int)src.length(),buff2,(int)src.length(),NULL,NULL);
				}
				else // UTF8
				{
					MultiByteToWideChar(28592,0,src.c_str(),(int)src.length(),buff,(int)src.length()+1);
					//WideCharToMultiByte(CP_UTF8,0,buff,(int)src.length(),buff2,(int)src.length(),NULL,NULL);
					int size = WideCharToMultiByte(CP_UTF8,0,buff,(int)src.length()+1,0,0,NULL,NULL);
					delete [] buff2;
					buff2 = new char[size];
					WideCharToMultiByte(CP_UTF8,0,buff,(int)src.length()+1,buff2,size,NULL,NULL);
				}
				break;
			case cp852:
				if(outputEncoding == cp1250)
				{
					MultiByteToWideChar(CP_OEMCP,0,src.c_str(),(int)src.length(),buff,(int)src.length()+1);
					WideCharToMultiByte(1250,0,buff,(int)src.length(),buff2,(int)src.length(),NULL,NULL);
				}
				else if(outputEncoding == iso88592) // iso8859-2
				{
					MultiByteToWideChar(CP_OEMCP,0,src.c_str(),(int)src.length(),buff,(int)src.length()+1);
					WideCharToMultiByte(28592,0,buff,(int)src.length(),buff2,(int)src.length(),NULL,NULL);
				}
				else // UTF8
				{
					MultiByteToWideChar(CP_OEMCP,0,src.c_str(),(int)src.length(),buff,(int)src.length()+1);
					//WideCharToMultiByte(CP_UTF8,0,buff,(int)src.length(),buff2,(int)src.length(),NULL,NULL);
					int size = WideCharToMultiByte(CP_UTF8,0,buff,(int)src.length()+1,0,0,NULL,NULL);
					delete [] buff2;
					buff2 = new char[size];
					WideCharToMultiByte(CP_UTF8,0,buff,(int)src.length()+1,buff2,size,NULL,NULL);
				}
				break;
			}

			std::string temp(buff2);
			delete[]buff;
			delete[]buff2;
			return temp;

#else
			iconv_t cd;

			cd = iconv_open(GetEncodingName(outputEncoding, true), GetEncodingName(inputEncoding));
			if(cd == (iconv_t)-1)
				throw tagexception("iconv_open error");

			size_t sizeIn = src.size() + 1;
			size_t sizeOut = 3 * sizeIn + 1;// - 1;
			size_t preSizeOut = sizeOut;

			char *buf1 = new char[sizeIn];
			char *buf2 = new char[sizeOut];
			char *cp1 = buf1;
			char *cp2 = buf2;

			//memset(buf2,0,(int)src.length());
			strcpy(buf1,src.c_str());

			size_t iconv_error = iconv(cd,&cp1,&sizeIn,&cp2,&sizeOut);
			if(iconv_error == (size_t)-1)
			{
				std::stringstream msg;
				msg << "iconv error while converting " << src;
				if(src.size() == 1) msg << " (charcode "
						<< (int((unsigned char)src[0])) << ")";
				msg << " with errno=" << errno;
				if(errno == EILSEQ) msg << " (EILSEQ)";
				if(errno == EINVAL) msg << " (EINVAL)";
				if(errno == E2BIG)
				{
					msg << " (E2BIG: using preSizeIn="
							<< src.size() << ", preSizeOut="
							<< preSizeOut << ")";
				}
				msg << ". Conversion from " << GetEncodingName(inputEncoding)
						<< " to " << GetEncodingName(outputEncoding, true);
				throw tagexception(msg.str().c_str());
			}

			iconv_close(cd);

			std::string temp(buf2);
			delete [] buf1;
			delete [] buf2;
			return temp;
#endif
		}

		void ConvertToUTFFromCP1250( std::string& src, std::string& dest)
		{
			dest = Convert(src, cp1250, utf8);
		}

		void ConvertFromUTFToCP1250(std::string& src, std::string& dest)
		{
			dest = Convert(src, utf8, cp1250);
		}

		bool IsSpace(char c, char next)
		{
			if(c == (char)(0xc2) && next == -96)
				return true;
			if(c == (char)(0xc2) && next == -1)
				return true;
			if(c == -96)
				return true;
			else if(isspace(c & 0xFF))
				return true;
			else
				return false;
		}
		
		bool IsPolishUTF(char c)
		{
			if ( c==(char)(0xc4)
						  || c==(char)(0xc5)
						  || c==(char)(0xc3))
				return true;

			return false;
		}



		// podejrzane
		bool IsBig(std::string& word)
		{
			if (word[0] == (char)(0xc4)
				|| word[0] == (char)(0xc3)
				|| word[0] == (char)(0xc5))
			{
				switch (word[1])
				{
				case (char)(0x84) : return true;
				case (char)(0x86) : return true;
				case 152 : return true;
				case 129 : return true;
				case 131 : return true;
				case (char)(0x93) : return true;
				case (char)(0x9a) : return true;
				case (char)(0xbb) : return true;
				case (char)(0xb9) : return true;
				}
				return false;
			}
			else if(word[0] > 0 && isalpha(word[0]))
				return !islower(word[0]);
			else
				return false;
		}

		void Spliter(std::string& text,std::vector<std::pair<std::string,bool> >& out)
		{
			std::string word;
			bool foundPunct = false;

			for(int i = 0; i < (int)text.length(); i++)
			{
				if(text[i] > 0 && isspace(text[i]))
				{
					if(!word.empty())
					{
						out.push_back(std::pair<std::string,bool>(word,true));
						word.clear();
					}
				}
				else if(text[i] > 0 && ispunct(text[i]))
				{
					if(!word.empty())
					{
						out.push_back(std::pair<std::string,bool>(word,false));
						word.clear();
					}
					word.append(text,i,1);
					foundPunct = true;
				}
				else
				{
					if(foundPunct)
					{
						if(!word.empty())
						{
							out.push_back(std::pair<std::string,bool>(word,false));
							word.clear();
						}
						foundPunct = false;
					}
					word.append(text,i,1);
				}
			}
			if(!word.empty())
			{
				out.push_back(std::pair<std::string,bool>(word,false));
			}
		}

		bool IsLowerCP1250(char c)
		{
			static char isLowWINDOWS[256] =
			{
				//01 02 03  04 05 06 07  08 09 0A 0B  0C 0D 0E 0F
				0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, // 00
				0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, // 10
				0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, // 20
				0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, // 30

				0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, // 40
				0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, // 50
				0, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1, // 60
				1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 0,  0, 0, 0, 0, // 70

				0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, // 80
				0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 1, 0,  1, 1, 1, 1, // 90
				0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, // A0
				0, 0, 0, 1,  0, 0, 0, 0,  0, 1, 1, 0,  0, 0, 1, 1, // B0

				0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, // C0
				0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 1, // D0
				1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1, // E0
				1, 1, 1, 1,  1, 1, 1, 0,  1, 1, 1, 1,  1, 1, 1, 0  // F0
			};
			return isLowWINDOWS[(unsigned char)c] != 0;
		}
		
		bool IsUpperCP1250(char c)
		{
			static char isUpWINDOWS[256] =
			{
				//01 02 03  04 05 06 07  08 09 0A 0B  0C 0D 0E 0F
				0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, // 00
				0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, // 01
				0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, // 02
				0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, // 03
				0, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1, // 04
				1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 0,  0, 0, 0, 0, // 05
				0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, // 06
				0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, // 07
				0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 1, 0,  1, 1, 1, 1, // 08
				0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, // 09
				0, 0, 0, 1,  0, 1, 0, 0,  0, 0, 1, 0,  0, 0, 0, 1, // 0A
				0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  1, 0, 0, 0, // 0B
				1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1, // 0C
				1, 1, 1, 1,  1, 1, 1, 0,  1, 1, 1, 1,  1, 1, 1, 0, // 0D
				0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, // 0E
				0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0 // 0F
			};
			return isUpWINDOWS[(unsigned char)c] != 0;
		}
		char ToLowerCP1250(char c)
		{
			static unsigned char toLowWINDOWS[256] = //windows-1250
			{
				0   , 1   , 2   , 3   , 4   , 5   , 6   , 7   , 8   , 9   , 10  , 11  , 12  , 13  , 14  , 15  , // 00
				16  , 17  , 18  , 19  , 20  , 21  , 22  , 23  , 24  , 25  , 26  , 27  , 28  , 29  , 30  , 31  , // 01
				32  , 33  , 34  , 35  , 36  , 37  , 38  , 39  , 40  , 41  , 42  , 43  , 44  , 45  , 46  , 47  , // 02
				48  , 49  , 50  , 51  , 52  , 53  , 54  , 55  , 56  , 57  , 58  , 59  , 60  , 61  , 62  , 63  , // 03
				64  , 97  , 98  , 99  , 100 , 101 , 102 , 103 , 104 , 105 , 106 , 107 , 108 , 109 , 110 , 111 , // 04
				112 , 113 , 114 , 115 , 116 , 117 , 118 , 119 , 120 , 121 , 122 , 91  , 92  , 93  , 94  , 95  , // 05
				96  , 97  , 98  , 99  , 100 , 101 , 102 , 103 , 104 , 105 , 106 , 107 , 108 , 109 , 110 , 111 , // 06
				112 , 113 , 114 , 115 , 116 , 117 , 118 , 119 , 120 , 121 , 122 , 123 , 124 , 125 , 126 , 127 , // 07
				128 , 129 , 130 , 131 , 132 , 133 , 134 , 135 , 136 , 137 , 154 , 139 , 156 , 157 , 158 , 159 , // 08
				144 , 145 , 146 , 147 , 148 , 149 , 150 , 151 , 152 , 153 , 154 , 155 , 156 , 157 , 158 , 159 , // 09
				160 , 161 , 162 , 179 , 164 , 185 , 166 , 167 , 168 , 169 , 186 , 171 , 172 , 173 , 174 , 191 , // 0A
				176 , 177 , 178 , 179 , 180 , 181 , 182 , 183 , 184 , 185 , 186 , 187 , 190 , 189 , 190 , 191 , // 0B
				224 , 225 , 226 , 227 , 228 , 229 , 230 , 231 , 232 , 233 , 234 , 235 , 236 , 237 , 238 , 239 , // 0C
				240 , 241 , 242 , 243 , 244 , 245 , 246 , 215 , 248 , 249 , 250 , 251 , 252 , 253 , 254 , 223 , // 0D
				224 , 225 , 226 , 227 , 228 , 229 , 230 , 231 , 232 , 233 , 234 , 235 , 236 , 237 , 238 , 239 , // 0E
				240 , 241 , 242 , 243 , 244 , 245 , 246 , 247 , 248 , 249 , 250 , 251 , 252 , 253 , 254 , 255   // 0F
			};
			return (char)toLowWINDOWS[(unsigned char)c];
		}
		
		char ToUpperCP1250(char c)
		{
			static unsigned char toUpWINDOWS[256] = //windows-1250
			{
				0   , 1   , 2   , 3   , 4   , 5   , 6   , 7   , 8   , 9   , 10  , 11  , 12  , 13  , 14  , 15  , // 00
				16  , 17  , 18  , 19  , 20  , 21  , 22  , 23  , 24  , 25  , 26  , 27  , 28  , 29  , 30  , 31  , // 01
				32  , 33  , 34  , 35  , 36  , 37  , 38  , 39  , 40  , 41  , 42  , 43  , 44  , 45  , 46  , 47  , // 02
				48  , 49  , 50  , 51  , 52  , 53  , 54  , 55  , 56  , 57  , 58  , 59  , 60  , 61  , 62  , 63  , // 03
				64  , 65  , 66  , 67  , 68  , 69  , 70  , 71  , 72  , 73  , 74  , 75  , 76  , 77  , 78  , 79  , // 04
				80  , 81  , 82  , 83  , 84  , 85  , 86  , 87  , 88  , 89  , 90  , 91  , 92  , 93  , 94  , 95  , // 05
				96  , 65  , 66  , 67  , 68  , 69  , 70  , 71  , 72  , 73  , 74  , 75  , 76  , 77  , 78  , 79  , // 06
				80  , 81  , 82  , 83  , 84  , 85  , 86  , 87  , 88  , 89  , 90  , 123 , 124 , 125 , 126 , 127 , // 07
				128 , 129 , 130 , 131 , 132 , 133 , 134 , 135 , 136 , 137 , 138 , 139 , 140 , 141 , 142 , 143 , // 08
				144 , 145 , 146 , 147 , 148 , 149 , 150 , 151 , 152 , 153 , 138 , 155 , 140 , 141 , 142 , 143 , // 09
				160 , 161 , 162 , 163 , 164 , 165 , 166 , 167 , 168 , 169 , 170 , 171 , 172 , 173 , 174 , 175 , // 0A
				176 , 177 , 178 , 163 , 180 , 181 , 182 , 183 , 184 , 165 , 170 , 187 , 188 , 189 , 188 , 175 , // 0B
				192 , 193 , 194 , 195 , 196 , 197 , 198 , 199 , 200 , 201 , 202 , 203 , 204 , 205 , 206 , 207 , // 0C
				208 , 209 , 210 , 211 , 212 , 213 , 214 , 215 , 216 , 217 , 218 , 219 , 220 , 221 , 222 , 223 , // 0D
				192 , 193 , 194 , 195 , 196 , 197 , 198 , 199 , 200 , 201 , 202 , 203 , 204 , 205 , 206 , 207 , // 0E
				208 , 209 , 210 , 211 , 212 , 213 , 214 , 247 , 216 , 217 , 218 , 219 , 220 , 221 , 222 , 255   // 0F
			};
			return (char)toUpWINDOWS[(unsigned char)c];
		}

		// TODO: znaki niepolskie z ISO na ma³e
		char ToLowerISO88592(char c)
		{
			static unsigned char toLowISO[256] = 
			{
				0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
				16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
				32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
				48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
				64, 97, 98, 99,100,101,102,103,104,105,106,107,108,109,110,111,
				112,113,114,115,116,117,118,119,120,121,122, 91, 92, 93, 94, 95,
				96, 97, 98, 99,100,101,102,103,104,105,106,107,108,109,110,111,
				112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,
				128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,
				144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,
				160,177,162,179,164,165,182,167,168,169,170,171,188,173,174,191,
				176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,
				192,193,194,195,196,197,230,199,200,201,234,203,204,205,206,207,
				208,241,210,243,212,213,214,215,216,217,218,219,220,221,222,223,
				224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,
				240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255
			};
			return (char)toLowISO[(unsigned char)c];
		}

		// Zamienia ci¹g znaków na ma³ê litery, zwraca wskaŸnik ten sam ci¹g
		char *ToLowerCP1250(char *text)
		{
			for(char *ptr = text; *ptr != 0; ptr++)
			{
				*ptr = ToLowerCP1250(*ptr);
			}
			return text;
		}
		char *ToUpperCP1250(char *text)
		{
			for(char *ptr = text; *ptr != 0; ptr++)
			{
				*ptr = ToUpperCP1250(*ptr);
			}
			return text;
		}
		char *ToLowerISO88592(char *text)
		{
			for(char *ptr = text; *ptr != 0; ptr++)
			{
				*ptr = ToLowerISO88592(*ptr);
			}
			return text;
		}

		bool EqualsIgnoreCaseCP1250(const std::string &s1, const std::string &s2)
		{
			const char *p1 = s1.c_str();
			const char *p2 = s2.c_str();

			while(ToLowerCP1250(*p1) == ToLowerCP1250(*p2))
			{
				if(*p1 == 0)
					return true;
				p1++;
				p2++;
			}
			
			return false;
		}

		bool IsLowerCP1250(const std::string &text)
		{
			for(size_t ch = 0; ch < text.size(); ch++)
			{
				if(IsUpperCP1250(text[ch]))
					return false;
			}
			return true;
		}
		
		bool IsUpperCP1250(const std::string &text)
		{
			for(size_t ch = 0; ch < text.size(); ch++)
			{
				if(IsLowerCP1250(text[ch]))
					return false;
			}
			return true;
		}
		void ToLowerCP1250(std::string &text)
		{
			for(size_t ch = 0; ch < text.size(); ch++)
			{
				text[ch] = ToLowerCP1250(text[ch]);
			}
		}
		void ToUpperCP1250(std::string &text)
		{
			for(size_t ch = 0; ch < text.size(); ch++)
			{
				text[ch] = ToUpperCP1250(text[ch]);
			}
		}

		DLONG ParseDLONG(std::string &text)
		{
			DLONG out = 0ll;
			std::stringstream buff;
			buff << text;
			buff >> out;
			return out;
		}

		INTEGER ParseINTEGER(std::string &text)
		{
			INTEGER out = 0;
			std::stringstream buff;
			buff << text;
			buff >> out;
			return out;
		}

		void ToLowerUTF8(std::string &utf8_what)
		{
			// TODO keep a buffer to be freed finally, don't use new&delete everytime
			int buffsize = (int)utf8_what.size() * 3 + 1; // just in case
			char *cbuff = new char[buffsize];
			UChar *ubuff = new UChar[buffsize];
			UErrorCode err = U_ZERO_ERROR;
			
			// convert source to UChar *
			// the LENIENT variant would be more suitable, but it's not present in version 34
			u_strFromUTF8(ubuff, buffsize, NULL, utf8_what.c_str(), -1, &err);
			if(U_FAILURE(err))
				throw Corpus::tagexception("ICU conversion error while creating UChar *");
			
			// convert UChar * to lowercase using the default locale
			u_strToLower(ubuff, buffsize, ubuff, -1, "", &err);
			
			// convert UChar * back to char * and put it inside utf8_what
			u_strToUTF8(cbuff, buffsize, NULL, ubuff, -1, &err);
			if(U_FAILURE(err))
				throw Corpus::tagexception("ICU conversion error while converting back to char *");
			
			utf8_what = cbuff;
			
			delete [] cbuff;
			delete [] ubuff;
		}
		
		void ToUpperUTF8(std::string &utf8_what)
		{
			// TODO keep a buffer to be freed finally, don't use new&delete everytime
			int buffsize = (int)utf8_what.size() * 3 + 1; // just in case
			char *cbuff = new char[buffsize];
			UChar *ubuff = new UChar[buffsize];
			UErrorCode err = U_ZERO_ERROR;
			
			// convert source to UChar *
			// the LENIENT variant would be more suitable, but it's not present in version 34
			u_strFromUTF8(ubuff, buffsize, NULL, utf8_what.c_str(), -1, &err);
			if(U_FAILURE(err))
				throw Corpus::tagexception("ICU conversion error while creating UChar *");
			
			// convert UChar * to uppercase using the default locale
			u_strToUpper(ubuff, buffsize, ubuff, -1, "", &err);
			
			// convert UChar * back to char * and put it inside utf8_what
			u_strToUTF8(cbuff, buffsize, NULL, ubuff, -1, &err);
			if(U_FAILURE(err))
				throw Corpus::tagexception("ICU conversion error while converting back to char *");
			
			utf8_what = cbuff;
			
			delete [] cbuff;
			delete [] ubuff;
		}
	}
}


