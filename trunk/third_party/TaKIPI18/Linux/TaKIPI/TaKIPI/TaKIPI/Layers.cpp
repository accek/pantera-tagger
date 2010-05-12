#include "../takipi_headers.h"
#include "Layers.h"


namespace TaKIPI
{
	CLayers::CLayers(std::vector<int> attributes, std::vector<int> layers)
		:attributes(attributes),layers(layers)
	{
		layerIndex = -1;
		layerPointer = -1;
	}
	
	CLayers::CLayers()
	{
		layerIndex = -1;
		layerPointer = -1;
	}
	
	CLayers::~CLayers(void)
	{
	}
	
	void CLayers::Clear()
	{
		attributes.clear();
		layers.clear();
		layerIndex = -1;
		layerPointer = -1;
	}
	
	void CLayers::SetUpDefaultLayers(int upper)
	{
		Clear();
		
		switch(upper)
		{
		case 2:
			attributes.push_back(Corpus::CToken::flex);
			attributes.push_back(Corpus::CToken::gnd);
			attributes.push_back(Corpus::CToken::nmb);
			attributes.push_back(Corpus::CToken::cas);
			layers.push_back(0);
			layers.push_back(1);
			layers.push_back(3);
			break;
		case 1:
			attributes.push_back(Corpus::CToken::flex);
			attributes.push_back(Corpus::CToken::gnd);
			attributes.push_back(Corpus::CToken::nmb);
			layers.push_back(0);
			layers.push_back(1);
			break;
		case 0:
			attributes.push_back(Corpus::CToken::flex);
			layers.push_back(0);
			break;
		default:
			throw Corpus::tagexception("Invalid layer number.");
		}
	}
	
	void CLayers::SetUpDefaultSingleLayer(int which)
	{
		Clear();
		
		switch(which)
		{
		case 0:
			attributes.push_back(Corpus::CToken::flex);
			break;
		case 1:
			attributes.push_back(Corpus::CToken::nmb);
			attributes.push_back(Corpus::CToken::gnd);
			break;
		case 2:
			attributes.push_back(Corpus::CToken::cas);
			break;
		default:
			throw Corpus::tagexception("Invalid layer number.");
		}

		layers.push_back(0);
	}

	const int CLayers::GetCurrentAttribute() const
	{
		if (layerPointer == -1)
			return -1;
		return attributes[layerPointer];
	}

	bool CLayers::GotoNextLayer()
	{
		if ( ++layerIndex == (int)layers.size() )
			return false;

		layerPointer = -1;
		return true;
	}

	bool CLayers::GotoFirst()
	{
		if (layerIndex == -1 || layers[layerIndex] == -1)
			return false;
		else
		{
			layerPointer = layers[layerIndex];
			return true;
		}
	}

	bool CLayers::GotoNext()
	{
		if (layerIndex == -1 && layers[layerIndex] == -1)
			return false;
		else
		{
			layerPointer++;
			if (layerPointer == (int)attributes.size() 
				|| (layerIndex+1 != (int)layers.size() && layerPointer == layers[layerIndex+1]))
			{
				layerPointer = -1;
				return false;
			}
			return true;
		}
	}

	bool CLayers::GotoPreviousFirst()
	{
		if (layerIndex < 1)
		{
			return false;
		}
		else
		{
			layerPointer = layers[layerIndex-1];
			return true;
		}
	}

	bool CLayers::GotoPreviousNext()
	{
		if (layerIndex < 1)
		{
			return false;
		}
		else
		{
			layerPointer++;
			if (layerPointer == layers[layerIndex])
			{
				layerPointer = -1;		
				return false;
			}
			return true;
		}
	}

	bool CLayers::GotoFirstLayer()
	{
		if ((int)layers.size() == 0)
			return false;

		layerIndex = 0;
		GotoFirst();
		return true;
	}

	void CLayers::GetAllMasks(Corpus::CValue& mask)
	{
		int tempLayerIndex = layerIndex;
		int tempLayerPointer = layerPointer;

		if(!GotoFirstLayer())
			throw Corpus::tagexception("No layers defined.");

		Corpus::CValue tempMask;
		mask.first = 0;
		mask.second = 0;

		do
		{
			GetLayerMask(tempMask);
			mask.first += tempMask.first;
			mask.second += tempMask.second;
		}
		while(GotoNextLayer());

		layerIndex = tempLayerIndex;
		layerPointer = tempLayerPointer;
	}

	void CLayers::GetLayerMask(Corpus::CValue& mask, bool previous)
	{
		using namespace Corpus;

		int tempLayerIndex = layerIndex;
		int tempLayerPointer = layerPointer;

		if(!previous)
		{
			if(!this->GotoFirst())
				return;
		}
		else
		{
			if(!this->GotoPreviousFirst())
				return;
		}


		mask.first = 0;
		mask.second = 0;
		bool temp = true;

		do
		{
			switch(this->GetCurrentAttribute())
			{
			case CToken::flex:
				mask.first = cf_interp|cf_subst|cf_depr|cf_adj|cf_adja|cf_adjp		
					|cf_adv|cf_num|cf_ppron12|cf_ppron3|cf_siebie	
					|cf_fin|cf_bedzie|cf_aglt|cf_praet|cf_impt|cf_imps		
					|cf_inf|cf_pcon|cf_pant|cf_ger|cf_pact|cf_ppas		
					|cf_winien|cf_pred|cf_prep|cf_conj|cf_qub|cf_xxs|cf_xxx|cf_ign
					|cf_numcol|cf_twf|cf_tnum|cf_tsym|cf_tdate|cf_ttime|cf_turi|cf_tmail;
				break;
			case CToken::acm:
				mask.second |= gcm_akomod;
				break;
			case CToken::acn:
				mask.second |= gcm_akcent;
				break;
			case CToken::agg:
				mask.second |= gcm_agluta;
				break;
			case CToken::asp:
				mask.second |= gcm_aspekt;
				break;
			case CToken::cas:
				mask.second |= gcm_przypadek;
				break;
			case CToken::deg:
				mask.second |= gcm_stopien;
				break;
			case CToken::gnd:
				mask.second |= gcm_rodzaj;
				break;
			case CToken::inp:
				mask.second |= gcm_interp;
				break;
			case CToken::neg:
				mask.second |= gcm_negacja;
				break;
			case CToken::nmb:
				mask.second |= gcm_liczba;
				break;
			case CToken::per:
				mask.second |= gcm_osoba;
				break;
			case CToken::ppr:
				mask.second |= gcm_poprzyim;
				break;
			case CToken::vcl:
				mask.second |= gcm_wok;
				break;
			case CToken::tnt:
				mask.second |= gcm_numtype;
				break;
			default:
				throw tagexception("Wrong argument.");
			}

			if(!previous)
			{
				temp = GotoNext();
			}
			else
			{
				temp = GotoPreviousNext();
			}

		}
		while(temp);

		layerIndex = tempLayerIndex;
		layerPointer = tempLayerPointer;
	}
}//namespace TaKIPI
