#pragma once

class Generator
{
public:
    virtual ~Generator() = default;

    virtual int getInt(int low, int high) = 0;
};
