#include "ProgressBar.h"

#include <atomic>

class ProgressBarOverall : public ProgressBar
{
public:
    ProgressBarOverall(int maxYear, int simCount);

    void update(int year, int sim) override;

private:
    std::atomic<int> progress_{0};
};
