#include "../joskipi_headers.h"
#include "Pattern.h"

namespace JOSKIPI
{
	namespace Patterns
	{

		//Konstruujemy okno u¿ywaj¹c schematu bêd¹cego ci¹giem operatorów
		CPattern::CPattern(std::vector<JOSKIPI::Operators::COperator*>* schema, Corpus::TFlexClass flexClass, 
			Corpus::DLONG gramCat)	:schema(schema),gramaticalCat(gramCat),flexClass(flexClass)
		{
			patternSize = 0;
		}

		//Kasujemy wszystkie operatory zwi¹zane ze wzorcem
		CPattern::~CPattern()
		{
			if(schema)
			{
				for(int i = 0; i < (int)schema->size(); i++)
				{
					JOSKIPI::Operators::COperator* oper = (*schema)[i];
					if(oper)
						delete oper;
				}
				delete schema;
			}

		}
		void CPattern::AddOperator(JOSKIPI::Operators::COperator* oper)
		{
			schema->push_back(oper);
		}

		void CPattern::AddOperators(std::vector<JOSKIPI::Operators::COperator*>* opers)
		{
			std::vector<JOSKIPI::Operators::COperator*>::const_iterator it = opers->begin();
			while(it != opers->end())
			{
				schema->push_back(*it);
				it++;
			}
		}

		//Zwraca wzór ucz¹cy dla danego zdania, potrzebny do tworzenia
		//drzew decyzyjnych c4.5
		//Zapisuje wektor ucz¹cy dla tokenu w formacie tekstowym c4.5.
		void CPattern::GeneratePattern(Corpus::CValue &layerMask, Corpus::CSentence& srcSentence, 
			std::ostream& out, bool disambOnly)
		{
			Corpus::CSentence* sentence = disambOnly ?
				srcSentence.CreateDisambCopy() : &srcSentence;


			std::vector<Corpus::CValue> val;

			for(int i = 0; i < GetLength(); i++)
			{
				(*schema)[i]->GetValue(val,*sentence);

				if (val[0].first > 0ll)
				{
					out << val[0].first;

					/*
					if(val[0].first >= Corpus::cf_tnum)
					{
						std::cout << "$$$$$$$$$$\n"
							<< "$$$$$$$$$$ wartosc: " << val[0].first << "\n";
					}
					*/
				}
				else
				{
					out << val[0].second;
				}
				out << ",";
			}

			Corpus::CValue maskedValue;

			(*sentence)[sentence->GetPosition()]->DisambLexem().GetMaskedValue(layerMask,maskedValue);

			if (maskedValue.first != 0)
			{
				out << maskedValue.first << std::endl;
				if(find(names.begin(),names.end(),maskedValue.first) == names.end())
					names.push_back(maskedValue.first);
			}
			else
			{
				out << maskedValue.second << std::endl;
				if(find(names.begin(),names.end(),maskedValue.second) == names.end())
					names.push_back(maskedValue.second);
			}
			patternSize++;

			if(disambOnly)
				delete sentence;
		}


		//Dla danego okna generuje dane potrzebne do okreœlenia nazw w drzewie QuinLana
		void CPattern::GenerateValuesPattern(std::stringstream& out)
		{
			if((int)names.size() == 0)
				return;

			for(int i = 0; i < (int)names.size()-1; i++)
			{
				out << names[i] << ",";
			}
			out << names[(int)names.size()-1] << std::endl;

			for(int i = 0; i < (int)schema->size(); i++)
			{
				out << i << ":\t";

				(*schema)[i]->GetValueNames(out);

				out << "\n";
			}
		}

		//Zwraca wartoœæ atrybutu reprezentowanego przez stringa
		void CPattern::GetValueAt(int opIndex, Corpus::CSentence& sentence, int pos, Corpus::CValue& out)
		{
			std::vector<Corpus::CValue> val;
			(*schema)[pos]->GetValue(val,sentence);

			if (val[0].first == -1)
				val[0].first = 0;

			out = val[0];
		}

		//Zwraca string reprezentuj¹cy dany operator.
		std::string CPattern::GetName(int index)
		{
			return (*schema)[index]->ToString();
		}

		//Zwraca d³ugoœæ danego okna (schematu okna).
		int CPattern::GetLength()
		{
			return (int)schema->size();
		}

		int CPattern::GetPatternSize()
		{
			return patternSize;
		}

		//Zwraca wektor stringów reprezentuj¹cy dany schemat.
		void CPattern::GetNames(std::vector<std::string>& out)
		{
			// Czyœcimy wyjœciowy wektor.
			out.clear();

			// Kopiujemy schemat okna.
			for(std::vector<JOSKIPI::Operators::COperator*>::iterator i = schema->begin(); i != schema->end(); i++)
			{
				out.push_back( (*i)->ToString() );
			}
		}

		//Zwraca reprezentacje stringow¹ wzorca.
		std::string CPattern::ToString()
		{
			std::stringstream ss;

			ss << flexClass << "/" << gramaticalCat << ":" << std::endl;

			for(int i = 0; i < (int)schema->size(); i++)
				ss << (*schema)[i]->ToString() << std::endl;

			return ss.str();
		}

		//AW!
		//Dla danego okna generuje dane potrzebne do okreœlenia nazw w pliku arf
		void CPattern::GenerateArffValuesPattern(std::stringstream& out)
		{
			if((int)names.size() == 0)
				return;

			out << "@relation disambiguation" << std::endl;
			for(int i = 0; i < (int)schema->size(); i++)
			{
				out << "@attribute " << i << " {";

				(*schema)[i]->GetValueNames(out);

				out << "}" << std::endl;
			}

			out << "@attribute class {";
			for(int i = 0; i < (int)names.size()-1; i++)
			{
				out << names[i] << ",";
			}
			out << names[(int)names.size()-1] << "}" << std::endl;

			out << "@data" << std::endl;
		}


	}//namespace JOSKIPI

}//	namespace Patterns
