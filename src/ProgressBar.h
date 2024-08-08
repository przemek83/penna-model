#pragma once

#include <string>

class ProgressBar
{
public:
    ProgressBar(int maxYear, int simCount);
    virtual ~ProgressBar() = default;

    virtual void update(int year, int sim) = 0;

    [[maybe_unused]] int getLength() const { return length_; }

protected:
    bool shouldAddProgressMarker(int currentValue, int maxValue) const;

    bool isEnding(int currentValue, int maxValue) const;

    int getMaxYear() const { return maxYear_; }
    int getSimCount() const { return simCount_; }

    std::string getPreffix() const { return preffix_; }
    std::string getSuffix() const { return suffix_; }
    std::string getMarker() const { return marker_; }

private:
    const int maxYear_;
    const int simCount_;

    const std::string preffix_{'['};
    const std::string suffix_{']'};
    const std::string marker_{'*'};

    const int length_{50};
};
