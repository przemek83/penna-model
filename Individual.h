#pragma once

#include <cstdio>

#include "Common.h"

class Individual
{
public:
    int wiek;
    int ilosc_1;
    int przodek;

    static constexpr int intCount_{2};           //*32 bity
    static constexpr int intSize_{sizeof(int)};  // wielkosc integera

    unsigned int ciag[intSize_];

    void czy1(unsigned int miejsce);

    void itob(FILE* plik);

    void inicjuj(unsigned int a, unsigned int bity[intSize_]);

    // private:
    //     static constexpr int size_{2};               //*32 bity
    //     static constexpr int intSize_{sizeof(int)};  // wielkosc integera

    //    unsigned int ciag[intSize_];
};
