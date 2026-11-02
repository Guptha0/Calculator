#include "engine/StatisticsEngine.hpp"
#include "common/Exceptions.hpp"
#include <numeric>
#include <algorithm>
#include <cmath>

double StatisticsEngine::calculateMean(const std::vector<double>& data) {
    if (data.empty()) {
        throw EmptyDatasetException();
    }
    double sum = std::accumulate(data.begin(), data.end(), 0.0);
    return sum / static_cast<double>(data.size());
}

double StatisticsEngine::calculateMedian(std::vector<double> data) {
    if (data.empty()) {
        throw EmptyDatasetException();
    }
    std::sort(data.begin(), data.end());
    size_t n = data.size();
    if (n % 2 != 0) {
        return data[n / 2];
    }
    return (data[(n / 2) - 1] + data[n / 2]) / 2.0;
}

std::vector<double> StatisticsEngine::calculateMode(const std::vector<double>& data) {
    if (data.empty()) {
        throw EmptyDatasetException();
    }

    std::map<double, size_t> frequencyMap;
    size_t maxCount = 0;

    for (double val : data) {
        size_t count = ++frequencyMap[val];
        if (count > maxCount) {
            maxCount = count;
        }
    }

    std::vector<double> modes;
    for (const auto& [val, count] : frequencyMap) {
        if (count == maxCount) {
            modes.push_back(val);
        }
    }
    return modes;
}

double StatisticsEngine::calculateVariance(const std::vector<double>& data) {
    if (data.size() < 2) {
        throw CalculatorException("Statistical Error: Variance requires at least 2 data points.");
    }

    double mean = calculateMean(data);
    double sumSquaredDiff = 0.0;
    for (double val : data) {
        double diff = val - mean;
        sumSquaredDiff += diff * diff;
    }
    // Bessel's correction (N-1) for sample variance
    return sumSquaredDiff / static_cast<double>(data.size() - 1);
}

double StatisticsEngine::calculateStandardDeviation(const std::vector<double>& data) {
    return std::sqrt(calculateVariance(data));
}

