#include "../../../corpuslib_headers.h"
#include "BaseEnding.h"
#include "BaseEndingDict.h"

namespace Corpus
{
	namespace GuessingSystem
	{
		namespace BaseEnding
		{
			const int histosize = 8;
			int cutOffsetSum = 0;
			int endingsAnalysed = 0;
			int histogram[histosize + 1];

			void ClearStats()
			{
				cutOffsetSum = 0;
				endingsAnalysed = 0;
				for(int i = 0; i < histosize + 1; i++)
					histogram[i] = 0;
			}

			void DumpStats()
			{
				std::cout << "\nStatystyki koncowek form bazowych\n"
					<< "\nPrzeanalizowanych koncowek: " << endingsAnalysed
					<< "\nSrednia dlugosc napotkanej koncowki: "
					<< ((double)cutOffsetSum / (double)endingsAnalysed)
					<< "\n\n";
				for(int i = 0; i < histosize; i++)
				{
					std::cout << "Koncowek o dlugosci " << i << ": " << histogram[i]
					<< ", " << ((double)histogram[i] / (double) endingsAnalysed) << "\n";
				}
				std::cout << "Dluzszych koncowek: " << histogram[histosize]
					<< ", " << ((double)histogram[histosize] / (double) endingsAnalysed) << "\n";
			}

			void UpdateHistogram(int cutOffset)
			{
				endingsAnalysed++;
				if(cutOffset < histosize)
				{
					if(cutOffset >= 0)
						histogram[cutOffset]++;
				}
				else
				{
					histogram[histosize]++;
				}
			}

			void FindBaseEnding(const char *base, const char *orth,
					unsigned char &cutOffset, unsigned int &baseEndingId, int prefixToSkip)
			{
				// pomijamy prefiks w orth (znaleziony przez PreProcess), np. "nie" w imies³owach
				orth += prefixToSkip;

				// pomijamy wspólny pocz¹tek base i orth
				while(*base == *orth && *base != 0)
				{
					base++;
					orth++;
				}

				// w tym miejscu zaczyna siê koñcowka base
				baseEndingId = BaseEndingDict::GetInstance()->Add(base);

				// liczymy dlugosc koncowki orth
				cutOffset = (unsigned char)strlen(orth);
				UpdateHistogram(cutOffset);
			}

			const std::string &GenerateBase(const char *orth, unsigned char cutOffset, unsigned int baseEndingId, int prefixToSkip)
			{
				// pomijamy prefix w orth (znaleziony przez Postprocess)
				orth += prefixToSkip;
				
				int baselen = ((int)strlen(orth)) - ((int)cutOffset);
				
				static std::string restored;
				
				if(baselen > 0)
				{
					/* Do debugowania... 
					std::cout << "### orth(" << orth << "), cutOffset(" << (int)cutOffset <<"), baseEndingId("
						<< baseEndingId << "), baseEnding(" << BaseEndingDict::GetInstance()->GetAt(baseEndingId)
						<< "), needsNeg(" << needsNeg << ")\n"; */
					
					// wstawiamy baselen pierwszych znaków z orth + koñcówkê ze s³ownika
					restored = std::string(orth, baselen) + BaseEndingDict::GetInstance()->GetAt(baseEndingId);
				}
				else
				{
					// obcielibyœmy wszystkie litery z ortha, wiêc nie robimy tego, a w zamian zwracamy base==orth`
					restored = orth;
					// TODO: prawdopodobnie nale¿a³o by w ogóle nie dodawaæ tego leksemu
					// wtedy trzeba by pomin¹æ ten leksem, i jeœli skoñczy siê na tym,
					// ¿e token nie bêdzie mia³ ¿adnego leksemu, to dodaæ IGN.
					// Wymaga³oby to równie¿ ustalania prawdopodobieñstw na koñcu -- ¿eby dope³nia³y siê do 1.
				}

				return restored;
			}
			
			int PreProcess(TFlexClass &flexClass, TCategories &gramCat,
						   const char *base, const char *orth)
			{
				int prefixToSkip = 0;
				prefixToSkip = PreFixMarkedness(gramCat, gc_neg, gc_aff, "nie", 3, base, orth);
				if(prefixToSkip == 0)
					prefixToSkip = PreFixMarkedness(gramCat, gc_sup, gc_comp, "naj", 3, base, orth);
				return prefixToSkip;
			}
			
			int PostProcess(TFlexClass &flexClass, TCategories &gramCat,
							const char *orth)
			{
				int prefixToSkip;
				prefixToSkip = PostFixMarkedness(gramCat, gc_neg, gc_aff, "nie", 3, orth);
				if(prefixToSkip == 0)
					prefixToSkip = PostFixMarkedness(gramCat, gc_sup, gc_comp, "naj", 3, orth);
				return prefixToSkip;
			}
			
			int PreFixMarkedness(TCategories &gramCat, TCategories gc_marked, TCategories gc_unmarked,
								  const char *prefix, int prefixlen, const char *base, const char *orth)
			{
				// jeœli atrybuty nie zawieraj¹ gc_unmarked (mog¹cego wymagaæ zmiany), wychodzimy
				if((gramCat & gc_marked) == 0)
					return 0;
				// jeœli orth z morfeusza nie zaczyna siê prefiksem, wychodzimy
				if(strncmp(orth, prefix, prefixlen) != 0)
					return 0;
				// jeœli i orth, i base zaczynaj siê prefiksem, wychodzimy
				if(strncmp(base, prefix, prefixlen) == 0)
					return 0;
				// warunki spe³nione, zmieniamy gc_marked na gc_unmarked
				gramCat = (gramCat | gc_unmarked) & ~gc_marked;
				// zwracamy liczbê znaków do pominiêcia
				return prefixlen;
			}
			
			int PostFixMarkedness(TCategories &gramCat, TCategories gc_marked, TCategories gc_unmarked,
							const char *prefix, int prefixlen, const char *orth)
			{
				// jeœli atrybuty nie zawieraj¹ gc_unmarked (mog¹cego wymagaæ zmiany), wychodzimy
				if((gramCat & gc_unmarked) == 0)
					return 0;
				// jeœli zgadywany orth nie zaczyna siê prefiksem, wychodzimy
				if(strncmp(orth, prefix, prefixlen) != 0)
					return 0;
				// warunki spe³nione, zmieniamy gc_unmarked na gc_marked
				gramCat = (gramCat | gc_marked) & ~gc_unmarked;
				// zwracamy liczbê znaków do pominiêcia
				return prefixlen;
			}
			
		}
	}
}
