#pragma once

#include <cstdio>

#include "Common.h"

class Individual
{
public:
    int wiek;
    int ilosc_1;
    int przodek;
    unsigned int ciag[WIELKOSC];

    void czy1(unsigned int miejsce);

    void itob(FILE* plik);

    void inicjuj(unsigned int a, unsigned int bity[WIELKOSC]);
};
