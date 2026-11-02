#pragma once

#include "engine/ICalculatorMode.hpp"
#include "engine/ExpressionEvaluator.hpp"

class StandardMode : public ICalculatorMode {
public:
    StandardMode();
    ~StandardMode() override = default;

    [[nodiscard]] std::string getModeName() const override;
    [[nodiscard]] double evaluate(const std::string& expression) override;

    [[nodiscard]] double add(double a, double b) const;
    [[nodiscard]] double subtract(double a, double b) const;
    [[nodiscard]] double multiply(double a, double b) const;
    [[nodiscard]] double divide(double a, double b) const;
    [[nodiscard]] double percentage(double value, double total) const;

private:
    ExpressionEvaluator m_evaluator;
};

