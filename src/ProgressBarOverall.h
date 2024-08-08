#include "ProgressBar.h"

class ProgressBarOverall : public ProgressBar
{
public:
    using ProgressBar::ProgressBar;

    void update(int year, int sim) override;

    static int getSensitivity(int years) { return years / 100; }
};
