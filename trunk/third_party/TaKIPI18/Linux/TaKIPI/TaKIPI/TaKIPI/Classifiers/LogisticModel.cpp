#include "../../takipi_headers.h"
#include "LogisticModel.h"


namespace TaKIPI
{
	namespace Classifiers
	{

		CLogisticModel::CLogisticModel(void)
		{
			model = new std::map< Corpus::CValue, std::pair< double, std::map< int, 
				std::map< Corpus::DLONG, double> * > > > ();
		}

		CLogisticModel::CLogisticModel(std::vector<std::string> modelData)
		{
			this->Parse(modelData);
		}

		CLogisticModel::~CLogisticModel(void)
		{
			if(model)
			{
				while(model->size() > 0)
				{
					std::map< int, std::map< Corpus::DLONG, double > * > coefficients = model->begin()->second.second;
					while(coefficients.size() > 0)
					{
						if(coefficients.begin()->second != NULL)
							delete coefficients.begin()->second;
						coefficients.erase(coefficients.begin());
					}
					model->erase(model->begin());
				}
				delete model;
				model = NULL;

			}
		}


		void CLogisticModel::Parse(std::vector<std::string> modelData)
		{
			// model = klasa -> <wyraz_wolny; atrybut -> wartosc_atrybutu -> wspolczynnik>
			std::vector<std::string>::iterator it = modelData.begin();
			do
			{
				while (it != modelData.end() && it->find("Class") == it->npos)
				{
					it++;
				}
				if(it == modelData.end())
					continue;
				std::vector<std::string> tokens;
				Corpus::Parser::Tokenize(it->c_str(),tokens);

				//atol(tokens[1].c_str())
				Corpus::CValue classValue(Corpus::Parser::ParseDLONG(tokens[1]),0);
				it++;
				Corpus::Parser::Tokenize(it->c_str(),tokens);
				double freeCoeff = atof(tokens[0].c_str());
				it++;
				std::map< int, std::map< Corpus::DLONG, double > * > coefficients;
				while(it != modelData.end() && it->size() > 0)
				{
					Corpus::Parser::Tokenize(it->c_str(),tokens," [=]");
					int attribute = atoi(tokens[0].c_str());

					Corpus::DLONG attrval = Corpus::Parser::ParseDLONG(tokens[1]);
					double coeff = atof(tokens[3].c_str());
					std::map< Corpus::DLONG, double > *pAttribValCoeff = coefficients[attribute];
					if(pAttribValCoeff == NULL)
					{
						pAttribValCoeff = new std::map< Corpus::DLONG, double >();
						coefficients[attribute] = pAttribValCoeff;
					}
					pAttribValCoeff->insert(std::pair<Corpus::DLONG,double>(attrval,coeff));
					it++;
				}
				model->insert(std::pair< Corpus::CValue, std::pair< double, std::map< int, 
					std::map< Corpus::DLONG, double > * > > >(classValue, std::pair<double, std::map< int, 
					std::map< Corpus::DLONG, double > * > >(freeCoeff,coefficients)));

			} while(it != modelData.end());
		}

		std::string CLogisticModel::ToString()
		{
			std::stringstream ss;
			for(std::map< Corpus::CValue, std::pair< double, std::map< int, std::map< Corpus::DLONG, double > * > > >
				::iterator classes = model->begin();classes != model->end(); classes++)
			{
				ss << "Class " << classes->first.first << " :" << std::endl;
				ss << classes->second.first;
				for(std::map < int, std::map< Corpus::DLONG, double > * >::iterator 
					coefficients = classes->second.second.begin(); 
					coefficients != classes->second.second.end(); 
				coefficients ++ )
				{
					if(coefficients->second == NULL)
					{
						ss << " +" << std::endl << "[" << coefficients->first << "=????? NULL!]"; 
					}
					else
					{
						for(std::map< Corpus::DLONG, double>::iterator attrValCoeff = coefficients->second->begin(); 
							attrValCoeff != coefficients->second->end(); attrValCoeff++)
						{
							ss << " +" << std::endl << "[" << coefficients->first << "=" << attrValCoeff->first << "] * " << attrValCoeff->second;
						}
					}
				}
				ss << std::endl << std::endl;
			}
			return ss.str();
		}

		//oblicza 'F' wartosci pomniejszone o max F
		std::map<Corpus::CValue, double> CLogisticModel::GetLogs(JOSKIPI::Patterns::CPattern &pattern, Corpus::CSentence & sentence)
		{
			std::map<Corpus::CValue, double> logs;
			double FMax = -1.7976931348623157E308;
			for(std::map< Corpus::CValue, std::pair< double, std::map< int, std::map< Corpus::DLONG, double > * > > >
				::iterator classes = model->begin();classes != model->end(); classes++)
			{
				double F = classes->second.first;//free coefficient
				for(std::map < int, std::map< Corpus::DLONG, double > * >::iterator 
					coefficients = classes->second.second.begin(); 
					coefficients != classes->second.second.end(); coefficients ++ )
				{
					int attribute = coefficients->first;
					Corpus::CValue attribValue;
					pattern.GetValueAt(attribute,sentence,attribute,attribValue);//tu ewentualnie mozna miec jakis cache?
					std::map<Corpus::DLONG,double>::iterator foundCoeff = 
						coefficients->second->find(attribValue.first==0?attribValue.second:attribValue.first);

					if(foundCoeff != coefficients->second->end())
					{
						F += foundCoeff->second;
					}
				}
				F /= model->size();
				if(FMax < F)
				{
					FMax = F;
				}
				logs[classes->first] = F;
			}
			//double sum = 0;
			for(std::map<Corpus::CValue, double>::iterator it = logs.begin(); it != logs.end(); it++)
			{
				it->second -= FMax;
				//	sum += exp(it->second);
			}
			//sum = log(sum);

			//for(map<CValue, double>::iterator it = logs.begin(); it != logs.end(); it++)
			//{
			//	it->second -= sum;
			//}

			return logs;
		}

	}// namespace Classifiers
}//namespace TaKIPI

