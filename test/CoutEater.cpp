#include "CoutEater.h"

CoutEater::CoutEater() { std::cout.rdbuf(fakeOutput_.rdbuf()); }
CoutEater::~CoutEater() { std::cout.rdbuf(stdBuffer_); }

std::string CoutEater::getOutput() const { return fakeOutput_.str(); }