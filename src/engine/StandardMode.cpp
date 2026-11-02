#include "engine/StandardMode.hpp"
#include "common/Exceptions.hpp"
#include <cmath>

StandardMode::StandardMode() = default;

std::string StandardMode::getModeName() const {
    return "Standard";
}

double StandardMode::evaluate(const std::string& expression) {
    try {
        return m_evaluator.evaluate(expression);
    } catch (const MathException& e) {
        throw MalformedExpressionException(e.what());
    }
}

double StandardMode::add(double a, double b) const {
    return a + b;
}

double StandardMode::subtract(double a, double b) const {
    return a - b;
}

double StandardMode::multiply(double a, double b) const {
    return a * b;
}

double StandardMode::divide(double a, double b) const {
    if (std::abs(b) < 1e-15) {
        throw DivisionByZeroException();
    }
    return a / b;
}

double StandardMode::percentage(double value, double total) const {
    if (std::abs(total) < 1e-15) {
        throw DivisionByZeroException();
    }
    return (value / total) * 100.0;
}

