#include "engine/Calculator.hpp"
#include "common/Exceptions.hpp"
#include <sstream>

Calculator::Calculator()
    : m_activeModeType(ModeType::Standard),
      m_historyManager("history.txt") {
    m_modes[ModeType::Standard] = std::make_unique<StandardMode>();
    m_modes[ModeType::Scientific] = std::make_unique<ScientificMode>();
    m_modes[ModeType::Programmer] = std::make_unique<ProgrammerMode>();
}

void Calculator::setMode(ModeType mode) {
    m_activeModeType = mode;
}

Calculator::ModeType Calculator::getCurrentModeType() const noexcept {
    return m_activeModeType;
}

std::string Calculator::getCurrentModeName() const {
    auto it = m_modes.find(m_activeModeType);
    if (it != m_modes.end()) {
        return it->second->getModeName();
    }
    return "Unknown";
}

double Calculator::calculate(const std::string& expression) {
    auto it = m_modes.find(m_activeModeType);
    if (it == m_modes.end()) {
        throw CalculatorException("Active calculation mode is not initialized.");
    }

    double result = it->second->evaluate(expression);

    // Format result and save to history.txt
    std::stringstream ss;
    ss << result;
    m_historyManager.log(it->second->getModeName(), expression, ss.str());

    return result;
}

HistoryManager& Calculator::getHistory() noexcept {
    return m_historyManager;
}

StatisticsEngine& Calculator::getStats() noexcept {
    return m_statsEngine;
}

ScientificMode& Calculator::getScientificMode() {
    return *static_cast<ScientificMode*>(m_modes[ModeType::Scientific].get());
}

ProgrammerMode& Calculator::getProgrammerMode() {
    return *static_cast<ProgrammerMode*>(m_modes[ModeType::Programmer].get());
}

StandardMode& Calculator::getStandardMode() {
    return *static_cast<StandardMode*>(m_modes[ModeType::Standard].get());
}

