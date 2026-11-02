#pragma once

#include "engine/ICalculatorMode.hpp"
#include "engine/ExpressionEvaluator.hpp"
#include <cmath>

class ScientificMode : public ICalculatorMode {
public:
    enum class AngleUnit { Degrees, Radians };

    ScientificMode();
    ~ScientificMode() override = default;

    [[nodiscard]] std::string getModeName() const override;
    [[nodiscard]] double evaluate(const std::string& expression) override;

    void setAngleUnit(AngleUnit unit) noexcept;
    [[nodiscard]] AngleUnit getAngleUnit() const noexcept;

    [[nodiscard]] double sine(double angle) const;
    [[nodiscard]] double cosine(double angle) const;
    [[nodiscard]] double tangent(double angle) const;
    [[nodiscard]] double power(double base, double exponent) const;
    [[nodiscard]] double squareRoot(double value) const;
    [[nodiscard]] double logBase10(double value) const;
    [[nodiscard]] double naturalLog(double value) const;

private:
    AngleUnit m_angleUnit;
    ExpressionEvaluator m_evaluator;

    [[nodiscard]] double toRadians(double angle) const noexcept;
};

