#include "engine/ProgrammerMode.hpp"
#include "common/Exceptions.hpp"
#include <stdexcept>

ProgrammerMode::ProgrammerMode() = default;

std::string ProgrammerMode::getModeName() const {
    return "Programmer";
}

double ProgrammerMode::evaluate(const std::string& expression) {
    try {
        int64_t val = std::stoll(expression, nullptr, 0);
        m_engine.setValue(val);
        return static_cast<double>(val);
    } catch (const std::exception& e) {
        throw MalformedExpressionException("Failed to parse integer value in programmer mode.");
    }
}

void ProgrammerMode::setBaseValue(int64_t val) {
    m_engine.setValue(val);
}

int64_t ProgrammerMode::getBaseValue() const {
    return m_engine.getValue();
}

void ProgrammerMode::setWordSize(ProgrammerEngine::WordSize size) {
    m_engine.setWordSize(size);
}

ProgrammerEngine::WordSize ProgrammerMode::getWordSize() const {
    return m_engine.getWordSize();
}

std::string ProgrammerMode::toHex() const {
    return m_engine.toHex();
}

std::string ProgrammerMode::toDec() const {
    return m_engine.toDec();
}

std::string ProgrammerMode::toOct() const {
    return m_engine.toOct();
}

std::string ProgrammerMode::toBin() const {
    return m_engine.toBin();
}

void ProgrammerMode::bitwiseAnd(int64_t operand) {
    m_engine.bitwiseAnd(operand);
}

void ProgrammerMode::bitwiseOr(int64_t operand) {
    m_engine.bitwiseOr(operand);
}

void ProgrammerMode::bitwiseXor(int64_t operand) {
    m_engine.bitwiseXor(operand);
}

void ProgrammerMode::bitwiseNot() {
    m_engine.bitwiseNot();
}

void ProgrammerMode::shiftLeft(int bits) {
    m_engine.shiftLeft(bits);
}

void ProgrammerMode::shiftRight(int bits) {
    m_engine.shiftRight(bits);
}

