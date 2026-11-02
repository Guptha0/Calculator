#pragma once

#include "engine/ICalculatorMode.hpp"
#include "engine/StandardMode.hpp"
#include "engine/ScientificMode.hpp"
#include "engine/ProgrammerMode.hpp"
#include "engine/StatisticsEngine.hpp"
#include "storage/HistoryManager.hpp"
#include <memory>
#include <unordered_map>
#include <string>

class Calculator {
public:
    enum class ModeType { Standard, Scientific, Programmer };

    Calculator();
    ~Calculator() = default;

    void setMode(ModeType mode);
    [[nodiscard]] ModeType getCurrentModeType() const noexcept;
    [[nodiscard]] std::string getCurrentModeName() const;

    [[nodiscard]] double calculate(const std::string& expression);

    [[nodiscard]] HistoryManager& getHistory() noexcept;
    [[nodiscard]] StatisticsEngine& getStats() noexcept;
    [[nodiscard]] ScientificMode& getScientificMode();
    [[nodiscard]] ProgrammerMode& getProgrammerMode();
    [[nodiscard]] StandardMode& getStandardMode();

private:
    ModeType m_activeModeType;
    std::unordered_map<ModeType, std::unique_ptr<ICalculatorMode>> m_modes;
    HistoryManager m_historyManager;
    StatisticsEngine m_statsEngine;
};

