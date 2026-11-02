#pragma once

#include <string>

class ICalculatorMode {
public:
    virtual ~ICalculatorMode() = default;

    [[nodiscard]] virtual std::string getModeName() const = 0;
    [[nodiscard]] virtual double evaluate(const std::string& expression) = 0;
};

