#pragma once

#include <vector>
#include <map>

class StatisticsEngine {
public:
    StatisticsEngine() = default;

    [[nodiscard]] static double calculateMean(const std::vector<double>& data);
    [[nodiscard]] static double calculateMedian(std::vector<double> data);
    [[nodiscard]] static std::vector<double> calculateMode(const std::vector<double>& data);
    [[nodiscard]] static double calculateVariance(const std::vector<double>& data);
    [[nodiscard]] static double calculateStandardDeviation(const std::vector<double>& data);
};

