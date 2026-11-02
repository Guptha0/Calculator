#pragma once

#include <cstdint>
#include <string>

class ProgrammerEngine {
public:
    enum class WordSize { QWORD = 64, DWORD = 32, WORD = 16, BYTE = 8 };
    enum class Radix { BIN = 2, OCT = 8, DEC = 10, HEX = 16 };

    ProgrammerEngine();

    void setValue(int64_t val);
    int64_t getValue() const;

    void setWordSize(WordSize size);
    WordSize getWordSize() const;

    // Radix conversions
    std::string toHex() const;
    std::string toDec() const;
    std::string toOct() const;
    std::string toBin() const;

    // Bitwise operations
    void bitwiseAnd(int64_t operand);
    void bitwiseOr(int64_t operand);
    void bitwiseXor(int64_t operand);
    void bitwiseNot();
    void shiftLeft(int bits);
    void shiftRight(int bits);

private:
    int64_t m_value;
    WordSize m_wordSize;

    int64_t maskValue(int64_t val) const;
};

