#include "engine/ProgrammerEngine.hpp"
#include <sstream>
#include <iomanip>
#include <bitset>

ProgrammerEngine::ProgrammerEngine()
    : m_value(0), m_wordSize(WordSize::QWORD) {}

void ProgrammerEngine::setValue(int64_t val) {
    m_value = maskValue(val);
}

int64_t ProgrammerEngine::getValue() const {
    return m_value;
}

void ProgrammerEngine::setWordSize(WordSize size) {
    m_wordSize = size;
    m_value = maskValue(m_value);
}

ProgrammerEngine::WordSize ProgrammerEngine::getWordSize() const {
    return m_wordSize;
}

int64_t ProgrammerEngine::maskValue(int64_t val) const {
    switch (m_wordSize) {
        case WordSize::BYTE:  return static_cast<uint8_t>(val);
        case WordSize::WORD:  return static_cast<uint16_t>(val);
        case WordSize::DWORD: return static_cast<uint32_t>(val);
        case WordSize::QWORD: return val;
    }
    return val;
}

std::string ProgrammerEngine::toHex() const {
    std::stringstream ss;
    ss << std::hex << std::uppercase << m_value;
    return ss.str();
}

std::string ProgrammerEngine::toDec() const {
    return std::to_string(m_value);
}

std::string ProgrammerEngine::toOct() const {
    std::stringstream ss;
    ss << std::oct << m_value;
    return ss.str();
}

std::string ProgrammerEngine::toBin() const {
    std::string binStr = std::bitset<64>(m_value).to_string();
    return binStr.substr(64 - static_cast<int>(m_wordSize));
}

void ProgrammerEngine::bitwiseAnd(int64_t operand) {
    m_value = maskValue(m_value & operand);
}

void ProgrammerEngine::bitwiseOr(int64_t operand) {
    m_value = maskValue(m_value | operand);
}

void ProgrammerEngine::bitwiseXor(int64_t operand) {
    m_value = maskValue(m_value ^ operand);
}

void ProgrammerEngine::bitwiseNot() {
    m_value = maskValue(~m_value);
}

void ProgrammerEngine::shiftLeft(int bits) {
    m_value = maskValue(m_value << bits);
}

void ProgrammerEngine::shiftRight(int bits) {
    m_value = maskValue(static_cast<uint64_t>(m_value) >> bits);
}

