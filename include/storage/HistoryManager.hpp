#pragma once

#include <string>
#include <vector>

struct CalculationRecord {
    std::string timestamp;
    std::string mode;
    std::string expression;
    std::string result;

    [[nodiscard]] std::string format() const;
};

class HistoryManager {
public:
    explicit HistoryManager(std::string filePath = "history.txt");
    ~HistoryManager() = default;

    void log(const std::string& mode, const std::string& expression, const std::string& result);
    [[nodiscard]] std::vector<CalculationRecord> loadAll();
    void clear();

    [[nodiscard]] const std::vector<CalculationRecord>& getCachedRecords() const noexcept;

private:
    std::string m_filePath;
    std::vector<CalculationRecord> m_cache;

    [[nodiscard]] static std::string currentTimestamp();
};

