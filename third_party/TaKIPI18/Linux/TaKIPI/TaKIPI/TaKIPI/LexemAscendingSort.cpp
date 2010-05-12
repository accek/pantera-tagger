#include "../takipi_headers.h"
#include "LexemAscendingSort.h"

namespace TaKIPI
{


	CLexemAscendingSort::CLexemAscendingSort(TaKIPI::CLayers& layers)
		:layers(layers)
	{
	}

	CLexemAscendingSort::~CLexemAscendingSort(void)
	{
	}

	bool CLexemAscendingSort::operator()(Corpus::CLexem* l1, Corpus::CLexem* l2)
	{
		using namespace Corpus;
		if (!layers.GotoFirstLayer())
			throw tagexception("There are no attributes in CLayer.");

		DLONG  l1BinValue = 0;
		DLONG  l2BinValue = 0;

		do
		{
			l1BinValue = (*l1)[layers.GetCurrentAttribute()];
			l2BinValue = (*l2)[layers.GetCurrentAttribute()];

			if (l1BinValue != l2BinValue)
			{				
				if (l1BinValue < l2BinValue)
				{
					layers.GotoFirst();
					return true;
				}
				else
				{
					layers.GotoFirst();
					return false;
				}
			}
		}
		while(layers.GotoNext());

		layers.GotoFirst();
		return false;
	}

}//namespace TaKIPI
