#include "Individual.h"

#include <cstdlib>
#include <cstring>

void Individual::czy1(unsigned int miejsce)
{
    unsigned long int pomoc = ciag[miejsce / Config::intSize_];
    pomoc = pomoc >> (Config::intCount_ - (miejsce % Config::intSize_));
    if (pomoc & 1)
        ilosc_1++;
}

void Individual::itob(FILE* plik)
{
    char ciag_binarny[Config::intSize_ + 1];
    for (int x = 0; x < Config::intCount_; x++)
    {
        _itoa(ciag[x], ciag_binarny, 2);
        for (int i = Config::intSize_ - strlen(ciag_binarny); i > 0; i--)
            fprintf(plik, "0");
        fprintf(plik, "%s", ciag_binarny);
    }
    fprintf(plik, "\n");
}

void Individual::inicjuj(unsigned int a,
                         std::array<int, Config::intCount_>& bity)
{
    wiek = 0;
    ilosc_1 = 0;
    przodek = a;
    for (int i = 0; i < Config::intCount_; i++)
        ciag[i] = bity[i];
}
