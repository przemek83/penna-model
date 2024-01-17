#include <chrono>
#include <iostream>

#include "NumbersGenerator.h"
#include "Output.h"
#include "Simulation.h"
#include "SimulationData.h"

namespace
{
using std::chrono::duration_cast;
using std::chrono::seconds;
using std::chrono::steady_clock;
using std::chrono::time_point;

class Timer
{
public:
    ~Timer()
    {
        const auto end{steady_clock::now()};
        std::cout << std::endl
                  << "Execution time: "
                  << duration_cast<seconds>(end - start_).count() << "s."
                  << std::endl;
    }

private:
    time_point<steady_clock> start_{steady_clock::now()};
};
};  // namespace

int main()
{
    Config config;
    const float krok_symulacji =
#ifdef SYMULACJA_DORSZY
        static_cast<float>(abs(START_ODLOWOW - KONIEC_ODLOWOW))
#else
        100
#endif
        / static_cast<float>(config.simulationsCount_);

    SimulationData::AvgData simulationAvgData{
        SimulationData::prepareAvgData(config.years_)};

    Output output(krok_symulacji, config.years_);

    NumbersGenerator generator;

    for (int i = 1; i <= config.simulationsCount_; i++)
    {
        const Timer timer;

        output.otworz_pliki2(i);

        Simulation simulation(config, i, krok_symulacji);
        simulation.run(output, generator, simulationAvgData);
    }

    output.otworz_pliki(0);
    output.zapisz_srednie(config.simulationsCount_, simulationAvgData);
    output.zamknij_pliki(0);

    return EXIT_SUCCESS;
}
//============================================================//
