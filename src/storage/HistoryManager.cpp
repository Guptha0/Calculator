#include "storage/HistoryManager.hpp"
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>

std::string CalculationRecord::format() const {
    std::stringstream ss;
    ss << "[" << timestamp << "] [" << mode << "] " << expression << " = " << result;
    return ss.str();
}

HistoryManager::HistoryManager(std::string filePath)
    : m_filePath(std::move(filePath)) {
    (void)loadAll();
}

std::string HistoryManager::currentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

void HistoryManager::log(const std::string& mode, const std::string& expression, const std::string& result) {
    CalculationRecord rec{currentTimestamp(), mode, expression, result};
    m_cache.push_back(rec);

    std::ofstream outFile(m_filePath, std::ios::app);
    if (outFile.is_open()) {
        outFile << rec.format() << "\n";
    }
}

std::vector<CalculationRecord> HistoryManager::loadAll() {
    m_cache.clear();
    std::ifstream inFile(m_filePath);
    if (!inFile.is_open()) {
        return m_cache;
    }

    std::string line;
    while (std::getline(inFile, line)) {
        if (line.empty()) continue;
        // Parse format: [timestamp] [mode] expr = res
        size_t tEnd = line.find(']');
        if (tEnd == std::string::npos) continue;
        std::string timeStr = line.substr(1, tEnd - 1);

        size_t mStart = line.find('[', tEnd);
        size_t mEnd = line.find(']', mStart);
        if (mStart == std::string::npos || mEnd == std::string::npos) continue;
        std::string modeStr = line.substr(mStart + 1, mEnd - mStart - 1);

        size_t eqPos = line.find('=', mEnd);
        if (eqPos == std::string::npos) continue;

        std::string expr = line.substr(mEnd + 2, eqPos - (mEnd + 2) - 1);
        std::string res = line.substr(eqPos + 2);

        m_cache.push_back({timeStr, modeStr, expr, res});
    }
    return m_cache;
}

void HistoryManager::clear() {
    m_cache.clear();
    std::ofstream outFile(m_filePath, std::ios::trunc);
}

const std::vector<CalculationRecord>& HistoryManager::getCachedRecords() const noexcept {
    return m_cache;
}
