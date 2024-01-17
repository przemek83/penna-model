#include "Individual.h"

#include <cstdlib>
#include <cstring>

void Individual::czy1(unsigned int miejsce)
{
    unsigned long int pomoc = ciag[miejsce / Config::intCount_];
    pomoc = pomoc >> (Config::intCount_ - (miejsce % Config::intCount_));
    if (pomoc & 1)
        this->ilosc_1++;
}

void Individual::itob(FILE* plik)
{
    char ciag_binarny[Config::intCount_ + 1];
    for (int x = 0; x < Config::intSize_; x++)
    {
        _itoa(ciag[x], ciag_binarny, 2);
        for (int i = Config::intCount_ - strlen(ciag_binarny); i > 0; i--)
            fprintf(plik, "0");
        fprintf(plik, "%s", ciag_binarny);
    }
    fprintf(plik, "\n");
}

void Individual::inicjuj(unsigned int a, unsigned int bity[Config::intSize_])
{
    this->wiek = 0;
    this->ilosc_1 = 0;
    this->przodek = a;
    for (int i = 0; i < Config::intSize_; i++)
        this->ciag[i] = bity[i];
}
