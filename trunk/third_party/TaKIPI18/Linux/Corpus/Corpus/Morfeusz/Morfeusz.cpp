#include "../../corpuslib_headers.h"
#include "morfeusz.h"

#define MAXCAT 13
#define MAXVAL 10
#define LEX_PRETAG_BEG "<lex><base>"
#define LEX_PRETAG_END "</base><ctag>" // pmorf->haslo; for tag splitting
#define LEX_POSTTAG "</ctag></lex>\n" // for tag splitting
#define TOK_BEG "<tok>\n"
#define TOK_END "</tok>\n"
#define ORTH_BEG "<orth>"
#define ORTH_END "</orth>\n"
#define LEX_BEG "<lex><base>"
#define LEX_MID "</base><ctag>"
#define LEX_END "</ctag></lex>\n" // pmorf->haslo, pmorf->interp
#define IGN_OUT "ign"

namespace Corpus
{
	namespace Morfeusz
	{
		bool splitTags = true;

		void ReplaceUnredLine(std::string &tag)
		{
			std::vector<std::string> tagElems;

			Corpus::Parser::Tokenize(tag,tagElems,":");

			int i = 0;
			for(; i < (int)tagElems.size(); i++)
			{
				if(!tagElems[i].compare("_"))
					break;
			}

			if(!tagElems[0].compare("subst")
				|| !tagElems[0].compare("depr")
				|| !tagElems[0].compare("xxs")
				|| !tagElems[0].compare("ppron12")
				|| !tagElems[0].compare("ppron3"))
			{
				switch(i)
				{
				case 1:
					boost::replace_first(tag,"_","sg.pl");
					break;
				case 2:
					boost::replace_first(tag,"_","nom.gen.dat.acc.inst.loc.voc");
					break;
				case 3:
					boost::replace_first(tag,"_","m1.m2.m3.f.n");
					break;
				case 4:
					boost::replace_first(tag,"_","pri.sec.ter");
					break;
				case 5:
					boost::replace_first(tag,"_","akc.nakc");
					break;
				case 6:
					boost::replace_first(tag,"_","praep.npraep");
					break;
				}
			}
			else if(!tagElems[0].compare("num")
				|| !tagElems[0].compare("numcol"))
			{
				switch(i)
				{
				case 1:
					boost::replace_first(tag,"_","sg.pl");
					break;
				case 2:
					boost::replace_first(tag,"_","nom.gen.dat.acc.inst.loc.voc");
					break;
				case 3:
					boost::replace_first(tag,"_","m1.m2.m3.f.n");
					break;
				case 4:
					boost::replace_first(tag,"_","congr.rec");
					break;
				}
			}
			else if(!tagElems[0].compare("adj"))
			{
				switch(i)
				{
				case 1:
					boost::replace_first(tag,"_","sg.pl");
					break;
				case 2:
					boost::replace_first(tag,"_","nom.gen.dat.acc.inst.loc.voc");
					break;
				case 3:
					boost::replace_first(tag,"_","m1.m2.m3.f.n");
					break;
				case 4:
					boost::replace_first(tag,"_","pos.comp.sup");
					break;
				}
			}
			else if(!tagElems[0].compare("adv"))
			{
				boost::replace_first(tag,"_","pos.comp.sup");
			}
			else if(!tagElems[0].compare("siebie"))
			{
				boost::replace_first(tag,"_","nom.gen.dat.acc.inst.loc.voc");
			}
			else if(!tagElems[0].compare("fin")
				|| !tagElems[0].compare("bedzie")
				|| !tagElems[0].compare("aglt")
				|| !tagElems[0].compare("praet")
				|| !tagElems[0].compare("impt"))
			{
				switch(i)
				{
				case 1:
					boost::replace_first(tag,"_","sg.pl");
					break;
				case 2:
					if(tagElems[0].compare("praet"))
						boost::replace_first(tag,"_","m1.m2.m3.f.n");
					else
						boost::replace_first(tag,"_","pri.sec.ter");
					break;
				case 3:
					boost::replace_first(tag,"_","imperf.perf");
					break;
				case 4:
					if(tagElems[0].compare("praet"))
						boost::replace_first(tag,"_","wok.nwok");
					else
						boost::replace_first(tag,"_","nagl.agl");
					break;
				}
			}
			else if(!tagElems[0].compare("pcon")
				|| !tagElems[0].compare("pant")
				|| !tagElems[0].compare("inf")
				|| !tagElems[0].compare("imps"))
			{
				boost::replace_first(tag,"_","imperf.perf");
			}
			else if(!tagElems[0].compare("prep"))
			{
				boost::replace_first(tag,"_","nom.gen.dat.acc.inst.loc.voc");
			}
			else if(!tagElems[0].compare("ger")
				||!tagElems[0].compare("pact")
				||!tagElems[0].compare("ppas"))
			{
				switch(i)
				{
				case 1:
					boost::replace_first(tag,"_","sg.pl");
					break;
				case 2:
					boost::replace_first(tag,"_","nom.gen.dat.acc.inst.loc.voc");
					break;
				case 3:
					boost::replace_first(tag,"_","m1.m2.m3.f.n");
					break;
				case 4:
					boost::replace_first(tag,"_","imperf.perf");
					break;
				case 5:
					boost::replace_first(tag,"_","aff.neg");
					break;
				}
			}
			else if(!tagElems[0].compare("winien"))
			{
				switch(i)
				{		case 1:
				boost::replace_first(tag,"_","sg.pl");
				break;
				case 2:
					boost::replace_first(tag,"_","m1.m2.m3.f.n");
					break;
				case 3:
					boost::replace_first(tag,"_","imperf.perf");
					break;
				}
			}
		}

		void SplitTags(Corpus::CToken *tok, const std::string &cx_base, char *cx_tag)
		{
			std::string s(cx_tag);

			if(s.find(".") == s.npos && s.find("|") == s.npos && s.find("_") == s.npos)
			{
				if(s.find("ppron") != s.npos)
				{
					if(s.find("ter") != s.npos)
						boost::replace_first(s,"ppron","ppron3");
					else
						boost::replace_first(s,"ppron","ppron12");
				}

				boost::replace_first(s,":n1",":n");
				boost::replace_first(s,":n2",":n");
				boost::replace_first(s,":p1",":m1");
				boost::replace_first(s,":p2",":n");
				boost::replace_first(s,":p3",":n");

				tok->AddLexem(cx_base, s);
			}
			else
			{
				bool wasChange = true;

				std::vector<std::string> tags;
				std::vector<std::string> newTags;
				std::vector<std::string> tagParts;

				if(s.find("|") == s.npos)
				{
					tags.push_back(s);
				}
				else
				{
					Corpus::Parser::Tokenize(s,tags,"|");
				}

				while(wasChange)
				{
					newTags.clear();
					wasChange = false;

					for(int i = 0; i < (int)tags.size(); i++)
					{
						if(tags[i].find("ppron") != tags[i].npos)
						{
							if(tags[i].find("ter") != tags[i].npos)
								boost::replace_first(tags[i],"ppron:","ppron3:");
							else
								boost::replace_first(tags[i],"ppron:","ppron12:");
						}

						if(tags[i].find("_") != tags[i].npos)
						{
							ReplaceUnredLine(tags[i]);
						}

						if(tags[i].find(".") == tags[i].npos)
						{
							newTags.push_back(tags[i]);
							continue;
						}

						wasChange = true;

						std::string befDot,aftDot = ":";
						std::vector<std::string> dotParts;
						Corpus::Parser::Tokenize(tags[i],tagParts,":");
						int j = 0;
						//wszystko do pierwszej kropki wrzucamy do  befDot.
						//a fragment z kropk� dzielimy
						for(; j < (int)tagParts.size(); j++)
						{
							if(tagParts[j].find(".") != tagParts[j].npos)
							{
								Corpus::Parser::Tokenize(tagParts[j],dotParts,".");
								break;
							}
							else
							{
								befDot.append(tagParts[j]);
								befDot.append(":");
							}
						}
						//wszystko po wrzucamy do aftDot
						if(j != (int)tagParts.size()-1)
						{
							j++;
							for(; j < (int)tagParts.size()-1; j++)
							{
								aftDot.append(tagParts[j]);
								aftDot.append(":");
							}
							aftDot.append(tagParts.back());
						}

						//tworzymy tagi do nowego zdania.
						for(int k = 0; k < (int)dotParts.size(); k++)
						{
							std::string newTag;
							if(aftDot.length() == 1)
								newTag = befDot + dotParts[k];
							else
								newTag = befDot + dotParts[k] + aftDot;


							newTags.push_back(newTag);
						}
					}

					tags.clear();
					tags.insert(tags.begin(),newTags.begin(),newTags.end());
				}

				newTags.clear();

				for(int i = 0; i < (int)tags.size(); i++)
				{
					std::string tagOut = tags[i];

					boost::replace_first(tagOut,":n1",":n");
					boost::replace_first(tagOut,":n2",":n");
					boost::replace_first(tagOut,":p1",":m1");
					boost::replace_first(tagOut,":p2",":n");
					boost::replace_first(tagOut,":p3",":n");

					if(find(newTags.begin(),newTags.end(),tagOut) == newTags.end())
						newTags.push_back(tagOut);
				}

				for(int i = 0; i < (int)newTags.size(); i++)
				{
					tok->AddLexem(cx_base, newTags[i]);
				}
			}
		}

		void SetEncoding(int encoding)
		{
			morfeusz_set_option(MORFOPT_ENCODING,encoding);
		}
		
		void Analyse(std::vector<Corpus::CToken *> &out, const std::string &form)
		{
			InterpMorf *pmorf = morfeusz_analyse(const_cast<char*>(form.c_str()));
			
			std::vector<Part> parts;
			
			// create a part for each node
			while(pmorf->p != -1)
			{
				parts.push_back(Part(pmorf));
				++pmorf; // next node
			}
			
			// join parts where possible
			for(int pos_src = 0; pos_src < (int)parts.size(); ++pos_src)
			{
				for(int pos_base = 0; pos_base < (int)parts.size(); ++pos_base)
				{
					if(pos_base != pos_src)
					{
						parts[pos_base].TryJoining(parts[pos_src]);
					}
				}
			}
			
			for(int pos_src = 0; pos_src < (int)parts.size(); ++pos_src)
			{
				parts[pos_src].HarvestTokens(out);
			}
		}
		
		Part::Part(InterpMorf *pmorf)
		{
			beg = pmorf->p;
			end = pmorf->k;
			content.push_back(pmorf);
		}
		bool Part::Disjoint(const Part &other) const
		{
			return (end <= other.beg) || (beg >= other.end);
		}
		bool Part::TryJoining(Part &other)
		{
			if(Disjoint(other))
			{
				return false;
			}
			else
			{
				Join(other);
				return true;
			}
		}
		
		void Part::Join(Part &other)
		{
			// move elements from other.content to this->content
			content.splice(content.end(), other.content);
			// update this range
			if(beg > other.beg) beg = other.beg;
			if(end < other.end) end = other.end;
			// update that range (mark it defunct)
			other.beg = -1;
			other.end = -1;
		}
		
		void Part::HarvestTokens(std::vector<Corpus::CToken *> &out)
		{
			static Corpus::CTag igntag(Corpus::cf_ign);
			// create once, then reuse (for storing tokens before flushing them finally to out vector)
			static std::deque<Corpus::CToken *> preque;
			
			// just in case clear the content
			preque.clear();
			
			// variantNum will be set to 1 at first and then reverted to 0 if no ambiguity (one-token part)
			int last_p = -1, last_k = -1;
			UBYTE lastvarnum = 0;
			Corpus::CToken *tok = NULL;
			
			for(std::list<InterpMorf *>::iterator iter = content.begin(); iter != content.end(); ++iter)
			{
				InterpMorf *pmorf = *iter;
				
				// cx_ <= don't know if it's CConfig::inputEncoding or always UTF-8
				if(pmorf->p != last_p || pmorf->k != last_k)
				{
					// create a new token
					std::string cx_orth(pmorf->forma);
					
					if(tok) preque.push_back(tok);
					tok = new Corpus::CToken(false);
					tok->SetOrth(cx_orth);
					// if this is part-initial token, increase the variant number
					if(pmorf->p == this->beg)
					{
						++lastvarnum;
					}
					// mark this token as belonging to last variant (to be reverted to zero if one token only)
					tok->variantNum = lastvarnum;
				}
				else
				{
					// keep old token, just append new lexems
					if(tok == NULL)
					{
						// should NEVER happen
						throw Corpus::tagexception("Internal error in Morfeusz.cpp, Part::HarvestTokens needs debugging!");
					}
				}
				
				if(pmorf->interp == NULL)
				{
					// unrecognised => create an IGN lexem (lemma := orth)
					
					Corpus::CLexem *lex = new Corpus::CLexem();
					lex->SetTag(igntag);
					lex->SeBase(tok->GetOrth());
					tok->AddLexem(lex);
				}
				else
				{
					std::string cx_base(pmorf->haslo);
					
					if(splitTags)
					{
						SplitTags(tok, cx_base, pmorf->interp);
					}
					else
					{
						tok->AddLexem(cx_base, pmorf->interp);
					}
				}
				last_p = pmorf->p;
				last_k = pmorf->k;
			}
			// flush last one
			if(tok) preque.push_back(tok);
			
			if(preque.size() == 1)
			{
				// no segmentation ambiguity;s revert variantNum to 0
				Corpus::CToken *toFlush = preque.front();
				preque.pop_front();
				toFlush->variantNum = 0;
				out.push_back(toFlush);
			}
			else
			{
				// there is some segmentation ambiguity, flush it as it is
				while(!preque.empty())
				{
					out.push_back(preque.front());
					preque.pop_front();
				}
			}
		}
		
		void Part::Dump()
		{
			std::cout << "Part <" << beg << "," << end << ">:\n";
			for(std::list<InterpMorf *>::iterator iter = content.begin(); iter != content.end(); ++iter)
			{
				std::cout << "\t" << (*iter)->p << "," << (*iter)->k << "," << (*iter)->forma << "\n";
			}
		}
	} // namespace Morfeusz

}// namespace Corpus
