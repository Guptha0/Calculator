#include "engine/ScientificMode.hpp"
#include "common/Exceptions.hpp"
#include <cmath>

namespace {
    constexpr double PI_VAL = 3.14159265358979323846;
}

ScientificMode::ScientificMode() 
    : m_angleUnit(AngleUnit::Degrees) {}

std::string ScientificMode::getModeName() const {
    return "Scientific";
}

void ScientificMode::setAngleUnit(AngleUnit unit) noexcept {
    m_angleUnit = unit;
    m_evaluator.setAngleMode(unit == AngleUnit::Degrees 
        ? ExpressionEvaluator::AngleMode::Degrees 
        : ExpressionEvaluator::AngleMode::Radians);
}

ScientificMode::AngleUnit ScientificMode::getAngleUnit() const noexcept {
    return m_angleUnit;
}

double ScientificMode::toRadians(double angle) const noexcept {
    return (m_angleUnit == AngleUnit::Degrees) ? (angle * PI_VAL / 180.0) : angle;
}

double ScientificMode::evaluate(const std::string& expression) {
    try {
        return m_evaluator.evaluate(expression);
    } catch (const MathException& e) {
        throw MalformedExpressionException(e.what());
    }
}

double ScientificMode::sine(double angle) const {
    return std::sin(toRadians(angle));
}

double ScientificMode::cosine(double angle) const {
    return std::cos(toRadians(angle));
}

double ScientificMode::tangent(double angle) const {
    double rad = toRadians(angle);
    if (std::abs(std::cos(rad)) < 1e-15) {
        throw InvalidDomainException("Tangent is undefined for this angle.");
    }
    return std::tan(rad);
}

double ScientificMode::power(double base, double exponent) const {
    if (base < 0.0 && std::floor(exponent) != exponent) {
        throw InvalidDomainException("Negative base with non-integer exponent results in complex number.");
    }
    return std::pow(base, exponent);
}

double ScientificMode::squareRoot(double value) const {
    if (value < 0.0) {
        throw InvalidDomainException("Square root of a negative number is undefined in real numbers.");
    }
    return std::sqrt(value);
}

double ScientificMode::logBase10(double value) const {
    if (value <= 0.0) {
        throw InvalidDomainException("Logarithm base 10 argument must be strictly positive.");
    }
    return std::log10(value);
}

double ScientificMode::naturalLog(double value) const {
    if (value <= 0.0) {
        throw InvalidDomainException("Natural logarithm argument must be strictly positive.");
    }
    return std::log(value);
}

