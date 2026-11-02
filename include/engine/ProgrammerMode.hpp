#pragma once

#include "engine/ICalculatorMode.hpp"
#include "engine/ProgrammerEngine.hpp"
#include <string>

class ProgrammerMode : public ICalculatorMode {
public:
    ProgrammerMode();
    ~ProgrammerMode() override = default;

    [[nodiscard]] std::string getModeName() const override;
    [[nodiscard]] double evaluate(const std::string& expression) override;

    void setBaseValue(int64_t val);
    [[nodiscard]] int64_t getBaseValue() const;

    void setWordSize(ProgrammerEngine::WordSize size);
    [[nodiscard]] ProgrammerEngine::WordSize getWordSize() const;

    [[nodiscard]] std::string toHex() const;
    [[nodiscard]] std::string toDec() const;
    [[nodiscard]] std::string toOct() const;
    [[nodiscard]] std::string toBin() const;

    void bitwiseAnd(int64_t operand);
    void bitwiseOr(int64_t operand);
    void bitwiseXor(int64_t operand);
    void bitwiseNot();
    void shiftLeft(int bits);
    void shiftRight(int bits);

private:
    ProgrammerEngine m_engine;
};

