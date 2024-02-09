#pragma once

class Generator
{
public:
    virtual ~Generator() = default;

    int getPercentChance();

    int getBitPosition();

protected:
    virtual int getInt(int low, int high) = 0;
};
