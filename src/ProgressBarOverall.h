#include "ProgressBar.h"

#include <mutex>
#include <vector>

class ProgressBarOverall : public ProgressBar
{
public:
    ProgressBarOverall(int maxYear, int simCount);

    void update(int year, int sim) override;

private:
    bool shouldCalculateProgress(int year, int maxYears) const;

    std::vector<int> progresses_;

    std::mutex mutex_;
};
