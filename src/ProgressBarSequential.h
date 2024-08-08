#include "ProgressBar.h"

class ProgressBarSequential : public ProgressBar
{
public:
    using ProgressBar::ProgressBar;

    void update(int year, int sim) override;
};
