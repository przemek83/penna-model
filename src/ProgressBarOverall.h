#include "ProgressBar.h"

#include <atomic>

class ProgressBarOverall : public ProgressBar
{
public:
    using ProgressBar::ProgressBar;

    void update(int year, int sim) override;

private:
    std::atomic<int> progress_{0};
};
