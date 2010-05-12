
#include "../joskipi_headers.h"
#include "SimpleParser.h"

namespace JOSKIPI
{
	namespace Parsers
	{

		CSimpleParser::CSimpleParser()
		{
		}

		CSimpleParser::~CSimpleParser(void)
		{
		}

		void CSimpleParser::ParseOperatorLines(std::vector<std::string>& opLines, 
			std::vector<Operators::COperator*>* operators,
			std::map<std::string,pos_ptr>* positions)
		{
			using namespace std;
			using namespace Operators;

			bool my_positions_map = false;

			if(positions == 0)
			{
				my_positions_map = true;
				positions = new map<string,pos_ptr>();
				DeclarePositions(positions,opLines);
			}

			if((int)opLines.size() == 1)
			{
				vector<string> operStrings;
				Corpus::Parser::Tokenize(opLines[0],operStrings);
				for(int i = 0; i < (int)operStrings.size(); i++)
				{
					ParseSingleOperator(operStrings[i],operators,positions);
				}
			}
			else
			{
				int questionMarkPos = -1;	
				for(int i = 0; i < (int)opLines.size(); i++)
				{
					if(opLines[i].find("?") != opLines[0].npos)
					{
						questionMarkPos = i;
						break;
					}
				}

				if(questionMarkPos != -1)
				{
					vector<string> temp2,temp3,temp4;
					Corpus::Parser::Tokenize(opLines[questionMarkPos],temp2,"?");

					for(int i = 0; i < questionMarkPos; i++)
						temp3.push_back(opLines[i]);
					temp3.push_back(temp2[0]);
					temp4.push_back(temp2[1]);
					for(int i = questionMarkPos+1; i < (int)opLines.size(); i++)
						temp4.push_back(opLines[i]);

					vector<CCondition*>* conditions = new vector<CCondition*>();
					vector<COperator*>* operVec = new vector<COperator*>();
					ParseOperatorLines(temp3,operVec,positions);
					ParseConditionLines(temp4,conditions,positions);

					operators->push_back(new CCondOper((*conditions)[0],(*operVec)[0]));
				}
				else
				{
					vector<string> temp;
					Corpus::Parser::Tokenize(opLines[0],temp,"(");
#ifdef WIN32
					if( !strcmpi(temp[0].c_str(), "not") 
						|| !strcmpi(temp[0].c_str(), "or")
						|| !strcmpi(temp[0].c_str(), "and"))
						operators->push_back(new CTestOper(ParseAndOrNot(opLines,positions)));
					else if( !strcmpi(temp[0].c_str(), "llook") 
						|| !strcmpi(temp[0].c_str(), "rlook") 
						||!strcmpi(temp[0].c_str(), "only"))
						operators->push_back(new CTestOper(ParseOnlyLook(opLines,positions)));
#else
					if( !strcasecmp(temp[0].c_str(), "not") 
						|| !strcasecmp(temp[0].c_str(), "or")
						|| !strcasecmp(temp[0].c_str(), "and"))
						operators->push_back(new CTestOper(ParseAndOrNot(opLines,positions)));
					else if( !strcasecmp(temp[0].c_str(), "llook") 
						|| !strcasecmp(temp[0].c_str(), "rlook") 
						||!strcasecmp(temp[0].c_str(), "only"))
						operators->push_back(new CTestOper(ParseOnlyLook(opLines,positions)));
#endif
				}
			}

			if(my_positions_map)
				delete positions;
		}

		void CSimpleParser::ParseConditionLines(std::vector<std::string>& condLines, std::vector<Operators::CCondition*>* conditions, 
				std::map<std::string,pos_ptr>* positions)
		{
			using namespace std;
			using namespace Operators;
			using Corpus::CPosition;
			
			if((int)condLines.size() == 0)
				return;

			if((int)condLines.size() == 1)
			{
				vector<string> temp;
				Corpus::Parser::Tokenize(condLines[0],temp,"(");
#ifdef WIN32
				if( !strcmpi(temp[0].c_str(), "equal"))
					conditions->push_back(ParseEqual(condLines[0],positions));
				else if( !strcmpi(temp[0].c_str(), "inter"))
					conditions->push_back(ParseIntersec(condLines[0],positions));
				else if( !strcmpi(temp[0].c_str(), "in"))
					conditions->push_back(ParseIn(condLines[0],positions));
				else if( !strcmpi(temp[0].c_str(), "isbig"))
					conditions->push_back(ParseIsBig(condLines[0],positions));
				else if( !strcmpi(temp[0].c_str(), "hasnum"))
					conditions->push_back(ParseHasNum(condLines[0],positions));
				else if( !strcmpi(temp[0].c_str(), "llook") 
					|| !strcmpi(temp[0].c_str(), "rlook") 
					||!strcmpi(temp[0].c_str(), "only"))
					conditions->push_back(ParseOnlyLook(condLines,positions));
				else if( !strcmpi(temp[0].c_str(), "agr")
					|| !strcmpi(temp[0].c_str(), "agrpp") )
					conditions->push_back(ParseAgreement(condLines,positions));
#else
				if( !strcasecmp(temp[0].c_str(), "equal"))
					conditions->push_back(ParseEqual(condLines[0],positions));
				else if( !strcasecmp(temp[0].c_str(), "inter"))
					conditions->push_back(ParseIntersec(condLines[0],positions));
				else if( !strcasecmp(temp[0].c_str(), "in"))
					conditions->push_back(ParseIn(condLines[0],positions));
				else if( !strcasecmp(temp[0].c_str(), "isbig"))
					conditions->push_back(ParseIsBig(condLines[0],positions));
				else if( !strcasecmp(temp[0].c_str(), "hasnum"))
					conditions->push_back(ParseHasNum(condLines[0],positions));
				else if( !strcasecmp(temp[0].c_str(), "llook") 
					|| !strcasecmp(temp[0].c_str(), "rlook") 
					||!strcasecmp(temp[0].c_str(), "only"))
					conditions->push_back(ParseOnlyLook(condLines,positions));
				else if( !strcasecmp(temp[0].c_str(), "agr")
					|| !strcasecmp(temp[0].c_str(), "agrpp") )
					conditions->push_back(ParseAgreement(condLines,positions));
#endif
			}
			else
			{
				if(condLines[0].find("not(") != string::npos
					|| condLines[0].find("or(") != string::npos
					|| condLines[0].find("and(") != string::npos)
				{
					vector<string> temp;
					int pointer = 0;
					int andOrNot = 0;
					do
					{
						boost::algorithm::trim(condLines[pointer]);
						if(condLines[pointer][0] == ')')
							andOrNot--;
						if(condLines[pointer].find("and(") != string::npos
							|| condLines[pointer].find("or(") != string::npos
							|| condLines[pointer].find("not(") != string::npos)
							andOrNot++;
						temp.push_back(condLines[pointer]);
						pointer++;
					}
					while(andOrNot != 0);

					if(condLines[0].find("llook") != string::npos
						|| condLines[0].find("rlook") != string::npos
						|| condLines[0].find("only") != string::npos)
					{
						conditions->push_back(ParseOnlyLook(temp,positions));
					}
					else
						conditions->push_back(ParseAndOrNot(temp,positions));

					vector<string> temp2;
					for(int i = pointer; i < (int)condLines.size(); i++)
					{
						temp2.push_back(condLines[i]);
					}
					ParseConditionLines(temp2,conditions,positions);
				}
				else
				{
					vector<string> temp3;
					vector<string> temp4;
					temp4.push_back(condLines[0]);
					for(int i = 1; i < (int)condLines.size(); i++)
					{
						temp3.push_back(condLines[i]);
					}
					ParseConditionLines(temp4,conditions,positions);
					ParseConditionLines(temp3,conditions,positions);
				}
			}
		}

		Operators::CComplexCond* CSimpleParser::ParseAndOrNot(std::vector<std::string>& opLines, 
				std::map<std::string,pos_ptr>* positions)
		{
			using namespace std;
			using namespace Operators;
			using Corpus::CPosition;
			vector<string> temp;

			for(int i = 1; i < (int)opLines.size()-1; i++)
			{
				if(opLines[i][opLines[i].size()-1] == ',')
					temp.push_back(opLines[i].substr(0,opLines[i].size()-1));
				else
					temp.push_back(opLines[i]);
			}

			vector<CCondition*>* conditions = new vector<CCondition*>();	
			ParseConditionLines(temp,conditions,positions);

			if(opLines[0].find("and(") != opLines[0].npos)
				return new CAndCC(conditions);
			else if(opLines[0].find("not(") != opLines[0].npos)
				return new CNotCC(conditions);
			else return new COrCC(conditions);
		}

		void CSimpleParser::DeclarePositions(std::map<std::string,pos_ptr>* positions, 
			std::vector<std::string>& opLines)
		{
			using namespace std;
			using namespace Operators;
			using Corpus::CPosition;

			for(int i = 0; i < (int)opLines.size(); i++)
			{
				vector<string> temp;
				Corpus::Parser::Tokenize(opLines[i],temp,"[](),");
				for(int j = 0; j < (int)temp.size(); j++)
				{
					if(temp[j][0] == '$')
					{
						string posName = temp[j].substr(1,temp[j].size()-1);

						if(temp[j][1] == '+' || temp[j][1] == '-')
						{
							int offset = atoi(temp[j].substr(1,2).c_str());
							string offPosName = temp[j].substr(3,temp[j].size()-3);
							//CPosition *posXX = (*positions)[offPosName];
							//std::cout << "POZYCJA: " << posXX->ToString() << std::endl;
							(*positions)[posName] = pos_ptr(new JOSKIPI::Operators::COffsetPosition(
								offset,(*positions)[offPosName]));
						}
						else
						{
							if((*positions).find(posName) == (*positions).end())
								(*positions)[posName] = pos_ptr(new Corpus::CPosition());
						}
					}
				}
			}
		}

		void CSimpleParser::ParseSingleOperator(std::string& oper, std::vector<Operators::COperator*>* operators, 
				std::map<std::string,pos_ptr>* positions)
		{
			using namespace std;
			using namespace Operators;
			
			if(oper.find("?") != oper.npos)
			{
				vector<string> temp4;
				vector<string> condOperElems;
				Corpus::Parser::Tokenize(oper,temp4,"?");
				condOperElems.push_back(temp4[1]);

				vector<CCondition*>* conditions = new vector<CCondition*>();
				vector<COperator*>* operVec = new vector<COperator*>();
				ParseConditionLines(condOperElems,conditions,positions);
				ParseSingleOperator(temp4[0],operVec,positions);

				operators->push_back(new CCondOper((*conditions)[0],(*operVec)[0]));
				return;
			}

			vector<string> temp;
			Corpus::Parser::Tokenize(oper,temp,"[]{}");
			if( Corpus::CTag::GetAttribIndex(temp[0]) != -1 
				|| !temp[0].compare("dispos")
				|| !temp[0].compare("discat"))
			{
				operators->push_back(ParseSimpleOperator(temp[0],temp[1],positions));
				return;
			}

			vector<string> temp2;
			Corpus::Parser::Tokenize(oper,temp2,"(");
#ifdef WIN32
			if( !strcmpi(temp2[0].c_str(), "equal"))
				operators->push_back(new CTestOper(ParseEqual(oper,positions)));
			else if( !strcmpi(temp2[0].c_str(), "inter"))
				operators->push_back(new CTestOper(ParseIntersec(oper,positions)));
			else if( !strcmpi(temp2[0].c_str(), "in"))
				operators->push_back(new CTestOper(ParseIn(oper,positions)));
			else if( !strcmpi(temp2[0].c_str(), "isbig"))
				operators->push_back(new CTestOper(ParseIsBig(oper,positions)));
			else if( !strcmpi(temp2[0].c_str(), "hasnum"))
				operators->push_back(new CTestOper(ParseHasNum(oper,positions)));
			else if( !strcmpi(temp2[0].c_str(), "catflt")
				|| !strcmpi(temp2[0].c_str(), "posflt"))
				operators->push_back(ParseFilter(oper,positions));
			else if( !strcmpi(temp2[0].c_str(), "llook") 
				|| !strcmpi(temp2[0].c_str(), "rlook") 
				|| !strcmpi(temp2[0].c_str(), "only"))
			{
				vector<string> temp3;
				temp3.push_back(oper);
				operators->push_back(new CTestOper(ParseOnlyLook(temp3,positions)));
			}
			else if( !strcmpi(temp2[0].c_str(), "agr") 
				|| !strcmpi(temp2[0].c_str(), "agrpp"))
			{
				vector<string> temp3;
				temp3.push_back(oper);
				operators->push_back(new CTestOper(ParseAgreement(temp3,positions)));
			}
#else
			if( !strcasecmp(temp2[0].c_str(), "equal"))
				operators->push_back(new CTestOper(ParseEqual(oper,positions)));
			else if( !strcasecmp(temp2[0].c_str(), "inter"))
				operators->push_back(new CTestOper(ParseIntersec(oper,positions)));
			else if( !strcasecmp(temp2[0].c_str(), "in"))
				operators->push_back(new CTestOper(ParseIn(oper,positions)));
			else if( !strcasecmp(temp2[0].c_str(), "isbig"))
				operators->push_back(new CTestOper(ParseIsBig(oper,positions)));
			else if( !strcasecmp(temp2[0].c_str(), "hasnum"))
				operators->push_back(new CTestOper(ParseHasNum(oper,positions)));
			else if( !strcasecmp(temp2[0].c_str(), "catflt")
				|| !strcasecmp(temp2[0].c_str(), "posflt"))
				operators->push_back(ParseFilter(oper,positions));
			else if( !strcasecmp(temp2[0].c_str(), "llook") 
				|| !strcasecmp(temp2[0].c_str(), "rlook") 
				|| !strcasecmp(temp2[0].c_str(), "only"))
			{
				vector<string> temp3;
				temp3.push_back(oper);
				operators->push_back(new CTestOper(ParseOnlyLook(temp3,positions)));
			}
			else if( !strcasecmp(temp2[0].c_str(), "agr") 
				|| !strcasecmp(temp2[0].c_str(), "agrpp"))
			{
				vector<string> temp3;
				temp3.push_back(oper);
				operators->push_back(new CTestOper(ParseAgreement(temp3,positions)));
			}
#endif
		}

		Operators::COperator* CSimpleParser::ParseFilter(std::string& oper, 
			std::map<std::string,pos_ptr>* positions)
		{
			using namespace std;
			using namespace Operators;
			using Corpus::CPosition;

			vector<string> temp;
			Corpus::Parser::Tokenize(oper,temp,",(");

			pos_ptr position;
			if(temp[1][0] == '$')
			{
				string posName = temp[1].substr(1,temp[1].size()-1);

				if(temp[1][1] == '+' || temp[1][1] == '-')
				{
					int offset = atoi(temp[1].substr(1,2).c_str());
					string offPosName = temp[1].substr(3,temp[1].size()-3);
					position = pos_ptr(new JOSKIPI::Operators::COffsetPosition(
									   offset,(*positions)[offPosName]));
				}
				else
				{
					if((*positions).find(posName) == (*positions).end())
						(*positions)[temp[1].substr(1,temp[1].size()-1)] = pos_ptr(new CPosition());

					position = (*positions)[temp[1].substr(1,temp[1].size()-1)];
				}
			}
			else
			{
#ifdef WIN32
				if(!strcmpi(temp[1].c_str(), "end"))
					position = pos_ptr(new Corpus::CPosition(CPosition::end));
				else if(!strcmpi(temp[1].c_str(), "begin"))
					position = pos_ptr(new Corpus::CPosition(CPosition::beginning));
				else
					position = pos_ptr(new Corpus::CPosition(atoi(temp[1].c_str())));
#else
				if(!strcasecmp(temp[1].c_str(), "end"))
					position = pos_ptr(new Corpus::CPosition(CPosition::end));
				else if(!strcasecmp(temp[1].c_str(), "begin"))
					position = pos_ptr(new Corpus::CPosition(CPosition::beginning));
				else
					position = pos_ptr(new Corpus::CPosition(atoi(temp[1].c_str())));
#endif
			}

			vector<string> temp2;
			Corpus::Parser::Tokenize(oper,temp2,"{}");

			CConstValue* constVal = new CConstValue();
			constVal->AddValue(ParseConst(temp2[1]));
			constVal->AddValue(ParseConst(temp2[3]));

			return new CCatFlt(position,constVal);
		}

		Operators::COperator* CSimpleParser::ParseSimpleOperator(std::string name, std::string posString, 
				std::map<std::string,pos_ptr>* positions)
		{
			using namespace std;
			using namespace Operators;
			using Corpus::CPosition;
			pos_ptr position;

			if(posString[0] == '$')
			{
				position = (*positions)[posString.substr(1,posString.size()-1)];
			}
			else
			{
				position = pos_ptr(new Corpus::CPosition(atoi(posString.c_str())));
			}

			if(!name.compare("dispos"))
			{
				return new CDisPosOper(position);
			}
			else if(!name.compare("discat"))
			{
				return new CDisCatOper(position);
			}
			else
				return new CSimpleOperator(position,Corpus::CTag::GetAttribIndex(name));
		}

		Operators::CBinaryComplexC* CSimpleParser::ParseOnlyLook(std::vector<std::string>& opLines, 
				std::map<std::string,pos_ptr>* positions)
		{
			using namespace std;
			using namespace Operators;
			using Corpus::CPosition;
			vector<string> temp;
			Corpus::Parser::Tokenize(opLines[0],temp,",(");

			vector<pos_ptr>* posVec = new vector<pos_ptr>();
			if(temp[1][0] == '$')
			{
				string posName = temp[1].substr(1,temp[1].size()-1);

				if(temp[1][1] == '+' || temp[1][1] == '-')
				{
					int offset = atoi(temp[1].substr(1,2).c_str());
					string offPosName = temp[1].substr(3,temp[1].size()-3);
					posVec->push_back(pos_ptr(new JOSKIPI::Operators::COffsetPosition(
						offset,(*positions)[offPosName])));
				}
				else
				{
					if((*positions).find(posName) == (*positions).end())
						(*positions)[temp[1].substr(1,temp[1].size()-1)] = pos_ptr(new CPosition());

					posVec->push_back((*positions)[temp[1].substr(1,temp[1].size()-1)]);
				}
			}
			else
			{
#ifdef WIN32
				if(!strcmpi(temp[1].c_str(), "end"))
					posVec->push_back(pos_ptr(new Corpus::CPosition(CPosition::end)));
				else if(!strcmpi(temp[1].c_str(), "begin"))
					posVec->push_back(pos_ptr(new Corpus::CPosition(CPosition::beginning)));
				else
					posVec->push_back(pos_ptr(new Corpus::CPosition(atoi(temp[1].c_str()))));
#else
				if(!strcasecmp(temp[1].c_str(), "end"))
					posVec->push_back(pos_ptr(new Corpus::CPosition(CPosition::end)));
				else if(!strcasecmp(temp[1].c_str(), "begin"))
					posVec->push_back(pos_ptr(new Corpus::CPosition(CPosition::beginning)));
				else
					posVec->push_back(pos_ptr(new Corpus::CPosition(atoi(temp[1].c_str()))));
#endif
			}
			if(temp[2][0] == '$')
			{
				string posName = temp[2].substr(1,temp[2].size()-1);

				if(temp[2][1] == '+' || temp[2][1] == '-')
				{
					int offset = atoi(temp[2].substr(1,2).c_str());
					string offPosName = temp[2].substr(3,temp[2].size()-3);
					posVec->push_back(pos_ptr(new JOSKIPI::Operators::COffsetPosition(
						offset,(*positions)[offPosName])));
				}
				else
				{
					if((*positions).find(posName) == (*positions).end())
						(*positions)[temp[2].substr(1,temp[2].size()-1)] = pos_ptr(new CPosition());

					posVec->push_back((*positions)[temp[2].substr(1,temp[2].size()-1)]);
				}
			}
			else
			{
#ifdef WIN32
				if(!strcmpi(temp[2].c_str(), "end"))
					posVec->push_back(pos_ptr(new Corpus::CPosition(CPosition::end)));
				else if(!strcmpi(temp[2].c_str(), "begin"))
					posVec->push_back(pos_ptr(new Corpus::CPosition(CPosition::beginning)));
				else
					posVec->push_back(pos_ptr(new Corpus::CPosition(atoi(temp[2].c_str()))));
#else
				if(!strcasecmp(temp[2].c_str(), "end"))
					posVec->push_back(pos_ptr(new Corpus::CPosition(CPosition::end)));
				else if(!strcasecmp(temp[2].c_str(), "begin"))
					posVec->push_back(pos_ptr(new Corpus::CPosition(CPosition::beginning)));
				else
					posVec->push_back(pos_ptr(new Corpus::CPosition(atoi(temp[2].c_str()))));
#endif
			}
			posVec->push_back((*positions)[temp[3].substr(1,temp[3].size()-1)]);

			int pointer = 0;
			for(int i = 0; i < 3; i++)
			{
				pointer = (int)opLines[0].find(",",pointer+1);
			}

			vector<string> temp2;

			if(opLines.size() == 1)
			{
				temp2.push_back( opLines[0].substr(pointer+1,opLines[0].size()-pointer-2) );
			}
			else
			{
				temp2.push_back( opLines[0].substr(pointer+1,opLines[0].size()-pointer-1) );

				for(int i = 1; i < (int)opLines.size(); i++)
				{
					temp2.push_back(opLines[i]);
				}
			}

			vector<CCondition*>* conditions = new vector<CCondition*>();
			ParseConditionLines(temp2,conditions,positions);

#ifdef WIN32
			if( !strcmpi(temp[0].c_str(), "only"))
				return new COnlyBinCC(posVec,conditions);
			else if( !strcmpi(temp[0].c_str(), "llook"))
				return new CLookLeftBinCC(posVec,conditions);
			else 
				return new CLookRightBinCC(posVec,conditions);
#else
			if( !strcasecmp(temp[0].c_str(), "only"))
				return new COnlyBinCC(posVec,conditions);
			else if( !strcasecmp(temp[0].c_str(), "llook"))
				return new CLookLeftBinCC(posVec,conditions);
			else 
				return new CLookRightBinCC(posVec,conditions);
#endif
		}

		Operators::CBinaryComplexC* CSimpleParser::ParseAgreement(std::vector<std::string>& opLines, 
			std::map<std::string,pos_ptr>* positions)
		{
			using namespace std;
			using namespace Operators;
			using Corpus::CPosition;
			vector<string> temp;
			Corpus::Parser::Tokenize(opLines[0],temp,",(");

			vector<pos_ptr>* posVec = new vector<pos_ptr>();
			if(temp[1][0] == '$')
			{
				string posName = temp[1].substr(1,temp[1].size()-1);

				if(temp[1][1] == '+' || temp[1][1] == '-')
				{
					int offset = atoi(temp[1].substr(1,2).c_str());
					string offPosName = temp[1].substr(3,temp[1].size()-3);
					posVec->push_back(pos_ptr(new JOSKIPI::Operators::COffsetPosition(
						offset,(*positions)[offPosName])));
				}
				else
				{
					if((*positions).find(posName) == (*positions).end())
						(*positions)[temp[1].substr(1,temp[1].size()-1)] = pos_ptr(new CPosition());

					posVec->push_back((*positions)[temp[1].substr(1,temp[1].size()-1)]);
				}
			}
			else
			{
#ifdef WIN32
				if(!strcmpi(temp[1].c_str(), "end"))
					posVec->push_back(pos_ptr(new Corpus::CPosition(CPosition::end)));
				else if(!strcmpi(temp[1].c_str(), "begin"))
					posVec->push_back(pos_ptr(new Corpus::CPosition(CPosition::beginning)));
				else
					posVec->push_back(pos_ptr(new Corpus::CPosition(atoi(temp[1].c_str()))));
#else
				if(!strcasecmp(temp[1].c_str(), "end"))
					posVec->push_back(pos_ptr(new Corpus::CPosition(CPosition::end)));
				else if(!strcasecmp(temp[1].c_str(), "begin"))
					posVec->push_back(pos_ptr(new Corpus::CPosition(CPosition::beginning)));
				else
					posVec->push_back(pos_ptr(new Corpus::CPosition(atoi(temp[1].c_str()))));
#endif
			}
			if(temp[2][0] == '$')
			{
				string posName = temp[2].substr(1,temp[2].size()-1);

				if(temp[2][1] == '+' || temp[2][1] == '-')
				{
					int offset = atoi(temp[2].substr(1,2).c_str());
					string offPosName = temp[2].substr(3,temp[2].size()-3);
					posVec->push_back(pos_ptr(new JOSKIPI::Operators::COffsetPosition(
						offset,(*positions)[offPosName])));
				}
				else
				{
					if((*positions).find(posName) == (*positions).end())
						(*positions)[temp[2].substr(1,temp[2].size()-1)] = pos_ptr(new CPosition());

					posVec->push_back((*positions)[temp[2].substr(1,temp[2].size()-1)]);
				}
			}
			else
			{
#ifdef WIN32
				if(!strcmpi(temp[2].c_str(), "end"))
					posVec->push_back(pos_ptr(new Corpus::CPosition(CPosition::end)));
				else if(!strcmpi(temp[2].c_str(), "begin"))
					posVec->push_back(pos_ptr(new Corpus::CPosition(CPosition::beginning)));
				else
					posVec->push_back(pos_ptr(new Corpus::CPosition(atoi(temp[2].c_str()))));
#else
				if(!strcasecmp(temp[2].c_str(), "end"))
					posVec->push_back(pos_ptr(new Corpus::CPosition(CPosition::end)));
				else if(!strcasecmp(temp[2].c_str(), "begin"))
					posVec->push_back(pos_ptr(new Corpus::CPosition(CPosition::beginning)));
				else
					posVec->push_back(pos_ptr(new Corpus::CPosition(atoi(temp[2].c_str()))));
#endif
			}
			posVec->push_back(pos_ptr(new Corpus::CPosition()));

			CConstValue* constVal = new CConstValue();
			vector<string> temp2;
			Corpus::Parser::Tokenize(opLines[0],temp2,"{}");
			constVal->AddValue(ParseConst(temp2[1]));

			vector<string> temp3;
			Corpus::Parser::Tokenize(temp2[2],temp3,",)");
			int bits = atoi(temp3[0].c_str());

#ifdef WIN32
			if( !strcmpi(temp[0].c_str(), "agr"))
#else
			if( !strcasecmp(temp[0].c_str(), "agr"))
#endif
				return new CAgrBinCC(posVec,constVal,bits);
			else 
				return new CAgrPPBinCC(posVec,constVal,bits);
		}

		Operators::CEqualSC* CSimpleParser::ParseEqual(std::string& oper, std::map<std::string,
			pos_ptr>* positions)
		{
			using namespace std;
			using namespace Operators;
			vector<CCondElem*>* condElems = new vector<CCondElem*>();
			vector<string> temp;
			Corpus::Parser::Tokenize(oper,temp,"()");

			vector<string> temp2;
			Corpus::Parser::Tokenize(temp[1],temp2,",");
			if(temp2[0][0] != '{')
			{
				if(oper.find("catflt") != string::npos)
				{
					string catflt = oper.substr(
						oper.find_first_of('c'),oper.find_last_of('{')-oper.find_first_of('c')-1);
					condElems->push_back(ParseFilter(catflt,positions));

					vector<string> temp5;
					Corpus::Parser::Tokenize(oper,temp5,"{}");
					if(temp5[temp5.size()-2].find("\"") == temp5[temp5.size()-2].npos)
					{
						CConstValue* constVal = new CConstValue();
						constVal->AddValue(ParseConst(temp5[temp5.size()-2]));
						condElems->push_back(constVal);
					}
					else
						condElems->push_back(ParseStringsConst(temp5[temp5.size()-2]));

					return new CEqualSC(*condElems);
				}
				else
				{
					vector<string> temp3;
					Corpus::Parser::Tokenize(temp2[0],temp3,"[]");
					condElems->push_back(ParseSimpleOperator(temp3[0],temp3[1],positions));
				}

				if(temp2[1][0] != '{')
				{
					vector<string> temp4;
					Corpus::Parser::Tokenize(temp2[1],temp4,"[]");
					condElems->push_back(ParseSimpleOperator(temp4[0],temp4[1],positions));
				}
				else
				{
					vector<string> temp5;
					Corpus::Parser::Tokenize(temp[1],temp5,"{}");
					if(temp5[1].find("\"") == string::npos)
					{
						CConstValue* constVal = new CConstValue();
						constVal->AddValue(ParseConst(temp5[1]));
						condElems->push_back(constVal);
					}
					else
						condElems->push_back(ParseStringsConst(temp5[1]));
				}
			}
			else
			{
				vector<string> temp6;
				Corpus::Parser::Tokenize(temp[1],temp6,"{}");

				if(temp6[0][0] != '\"')
				{
					CConstValue* constVal = new CConstValue();
					constVal->AddValue(ParseConst(temp6[0]));
					condElems->push_back(constVal);
				}
				else
					condElems->push_back(ParseStringsConst(temp6[0]));
				vector<string> temp7;
				Corpus::Parser::Tokenize(temp6[1],temp7,"[]");
				condElems->push_back(ParseSimpleOperator(temp7[0],temp7[1],positions));
			}

			return new CEqualSC(*condElems);
		}

		Corpus::CValue CSimpleParser::ParseConst(std::string& constStr)
		{
			using namespace std;
			using namespace Operators;
			using namespace Corpus;

			vector<string> temp;
			Corpus::Parser::Tokenize(constStr,temp,",");

			CValue retVal(0,0);

			for(int i = 0; i < (int)temp.size(); i++)
			{
				if(CTag::GetFlexClass(temp[i]) != cf_none)
				{
					retVal.first |= CTag::GetFlexClass(temp[i]);
				}
				if(CTag::GetGrammCatValue(temp[i]) != gc_none)
				{
					retVal.second |= CTag::GetGrammCatValue(temp[i]);
				}
				if(CTag::GetAttribIndex(temp[i]) != -1)
				{
					CValue mask;
					CTag::GetAttribMask(CTag::GetAttribIndex(temp[i]),mask);
					retVal.first |= mask.first;
					retVal.second|= mask.second;
				}
			}

			return retVal;
		}

		Operators::CConstValue* CSimpleParser::ParseStringsConst(std::string& constString)
		{
			using namespace std;
			using namespace Operators;
			vector<string> temp;
			Corpus::Parser::Tokenize(constString,temp,"\"");

			CConstValue* constVal = new CConstValue();

			for(int i = 0; i < (int)temp.size(); i+=2)
			{
				//string s;
				// Corpus::Parser::ConvertToUTFFromCP1250(temp[i],s);
				constVal->AddValue(Corpus::CValue(Corpus::CWordDictionary::GetInstance()->Add(temp[i]),str_msk));
			}

			return constVal;
		}

		Operators::CInSC* CSimpleParser::ParseIn(std::string& oper, std::map<std::string,pos_ptr>* positions)
		{
			using namespace std;
			using namespace Operators;
			using Corpus::CPosition;
			vector<CCondElem*>* condElems = new vector<CCondElem*>();
			vector<string> temp;
			Corpus::Parser::Tokenize(oper,temp,"()");

			vector<string> temp2;
			Corpus::Parser::Tokenize(temp[1],temp2,",");
			if(temp2[0][0] != '{')
			{
				vector<string> temp3;
				Corpus::Parser::Tokenize(temp2[0],temp3,"[]");
				condElems->push_back(ParseSimpleOperator(temp3[0],temp3[1],positions));

				if(temp2[1][0] != '{')
				{
					vector<string> temp4;
					Corpus::Parser::Tokenize(temp2[1],temp4,"[]");
					condElems->push_back(ParseSimpleOperator(temp4[0],temp4[1],positions));
				}
				else
				{
					vector<string> temp5;
					Corpus::Parser::Tokenize(temp[1],temp5,"{}");
					if(temp5[1][0] != '\"')
					{
						CConstValue* constVal = new CConstValue();
						constVal->AddValue(ParseConst(temp5[1]));
						condElems->push_back(constVal);
					}
					else
						condElems->push_back(ParseStringsConst(temp5[1]));
				}
			}
			else
			{
				vector<string> temp6;
				Corpus::Parser::Tokenize(temp[1],temp6,"{}");
				if(temp6[0][0] != '\"')
				{
					CConstValue* constVal = new CConstValue();
					constVal->AddValue(ParseConst(temp6[0]));
					condElems->push_back(constVal);
				}
				else
					condElems->push_back(ParseStringsConst(temp6[0]));
				vector<string> temp7;
				Corpus::Parser::Tokenize(temp6[1],temp7,",[]");
				condElems->push_back(ParseSimpleOperator(temp7[0],temp7[1],positions));
			}

			return new CInSC(*condElems);}

		Operators::CIntersecSC* CSimpleParser::ParseIntersec(std::string& oper, 
			std::map<std::string,pos_ptr>* positions)
		{
			using namespace std;
			using namespace Operators;
			using Corpus::CPosition;

			vector<CCondElem*>* condElems = new vector<CCondElem*>();
			vector<string> temp;
			Corpus::Parser::Tokenize(oper,temp,"()");

			vector<string> temp2;
			Corpus::Parser::Tokenize(temp[1],temp2,",");
			if(temp2[0][0] != '{')
			{
				vector<string> temp3;
				Corpus::Parser::Tokenize(temp2[0],temp3,"[]");
				condElems->push_back(ParseSimpleOperator(temp3[0],temp3[1],positions));

				if(temp2[1][0] != '{')
				{
					if(temp2[1][3] == '[' || temp2[1][4] == '[')
					{
						vector<string> temp4;
						Corpus::Parser::Tokenize(temp2[1],temp4,"[]");
						condElems->push_back(ParseSimpleOperator(temp4[0],temp4[1],positions));
					}
					else
					{	
						string innerOper = oper.substr(oper.find(",") + 1,oper.length() - oper.find(",") - 2);
						vector<COperator*> *opers = new vector<COperator*>();
						ParseSingleOperator(innerOper,opers,positions);
						condElems->push_back((*opers)[0]);
					}
				}
				else
				{
					vector<string> temp5;
					Corpus::Parser::Tokenize(temp[1],temp5,"{}");
					if(temp5[1][0] != '\"')
					{
						CConstValue* constVal = new CConstValue();
						constVal->AddValue(ParseConst(temp5[1]));
						condElems->push_back(constVal);
					}
					else
						condElems->push_back(ParseStringsConst(temp5[1]));
				}
			}
			else
			{
				vector<string> temp6;
				Corpus::Parser::Tokenize(temp[1],temp6,"{}");
				if(temp6[0][0] != '\"')
				{
					CConstValue* constVal = new CConstValue();
					constVal->AddValue(ParseConst(temp6[0]));
					condElems->push_back(constVal);
				}
				else
					condElems->push_back(ParseStringsConst(temp6[0]));
				vector<string> temp7;
				Corpus::Parser::Tokenize(temp6[1],temp7,"[]");
				condElems->push_back(ParseSimpleOperator(temp7[0],temp7[1],positions));
			}

			return new CIntersecSC(*condElems);
		}

		Operators::CIsBigSC* CSimpleParser::ParseIsBig(std::string& oper, 
			std::map<std::string,pos_ptr>* positions)
		{
			using namespace std;
			using namespace Operators;
			using Corpus::CPosition;
			vector<string> temp;
			Corpus::Parser::Tokenize(oper,temp,"()");
			vector<CCondElem*>* condElems = new vector<CCondElem*>();
			vector<string> temp2;
			Corpus::Parser::Tokenize(temp[1],temp2,"[]");

			condElems->push_back(ParseSimpleOperator(temp2[0],temp2[1],positions));

			return new CIsBigSC(*condElems);
		}

		Operators::CHasNumSC* CSimpleParser::ParseHasNum(std::string& oper, 
			std::map<std::string,pos_ptr>* positions)
		{
			using namespace std;
			using namespace Operators;
			using Corpus::CPosition;
			vector<string> temp;
			Corpus::Parser::Tokenize(oper,temp,"()");
			vector<CCondElem*>* condElems = new vector<CCondElem*>();
			vector<string> temp2;
			Corpus::Parser::Tokenize(temp[1],temp2,"[]");

			condElems->push_back(ParseSimpleOperator(temp2[0],temp2[1],positions));

			return new CHasNumSC(*condElems);
		}

	}
}

