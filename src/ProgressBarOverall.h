#include "ProgressBar.h"

#include <mutex>
#include <vector>

class ProgressBarOverall : public ProgressBar
{
public:
    ProgressBarOverall(int maxYear, int simCount);

    void update(int year, int sim) override;

private:
    bool shouldSkip(int year, int maxYears) const;

    int getCurrentSum(int year, int sim);

    std::vector<int> progresses_;

    std::mutex mutex_;
};
