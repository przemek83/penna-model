#pragma once

#include <ostream>
#include <vector>

class SimulationData
{
public:
    explicit SimulationData(std::size_t years);

    struct BasicMetrics
    {
        inline int getLivingAtEnd() const
        {
            return livingAtStart_ - deaths_ + births_;
        }

        int families_{0};
        int livingAtStart_{0};
        int births_{0};
        int deaths_{0};
    };

    const BasicMetrics& getBasicBasicMetrics(std::size_t year) const;
    void setBasicMetrics(std::vector<BasicMetrics> basicBasicMetrics);

    const std::vector<float>& getDeathsDistribution() const;
    void setDeathDistribution(
        const std::vector<int>& gompertzDeathsDistribution,
        const std::vector<int>& gompertzAgeDistribution);

    const std::vector<float>& getBitsDistribution() const;
    void setBitDistribution(const std::vector<int>& bitsDistribution,
                            int populationCount);

    const std::vector<int>& getAgeDistribution() const;
    void setAgeDistribution(const std::vector<int>& ageDistribution);

    void saveFamilies(std::ostream& stream, char separator) const;
    void saveBasicMetrics(std::ostream& stream, char separator) const;
    void saveBitsDistibution(std::ostream& stream, char separator) const;
    void saveAgeDistibution(std::ostream& stream, char separator) const;
    void saveDeathsDistibution(std::ostream& stream, char separator) const;

private:
    std::vector<float> deathsDistribution_;
    std::vector<float> bitsDistribution_;
    std::vector<int> ageDistribution_;

    std::vector<BasicMetrics> basicMetrics_;

    std::size_t years_;
};
