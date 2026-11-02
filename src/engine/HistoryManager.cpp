#include "engine/HistoryManager.hpp"

HistoryManager::HistoryManager(size_t maxCapacity)
    : m_maxCapacity(maxCapacity) {}

void HistoryManager::addRecord(const std::string& expression, const std::string& result) {
    if (m_records.size() >= m_maxCapacity) {
        m_records.erase(m_records.begin());
    }
    m_records.push_back({expression, result, std::chrono::system_clock::now()});
}

const std::vector<HistoryRecord>& HistoryManager::getRecords() const {
    return m_records;
}

void HistoryManager::clear() {
    m_records.clear();
}

