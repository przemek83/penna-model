#pragma once

class Generator
{
public:
    explicit Generator(int bits);
    virtual ~Generator() = default;

    Generator(const Generator&) = delete;
    Generator(Generator&&) = delete;
    Generator& operator=(const Generator&) = delete;
    Generator& operator=(Generator&&) = delete;

    int getPercentChance();

    int getBitPosition();

protected:
    virtual int getInt(int low, int high) = 0;

private:
    int bits_;
};
