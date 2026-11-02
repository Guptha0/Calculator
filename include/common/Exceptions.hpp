#pragma once

#include <stdexcept>
#include <string>

class CalculatorException : public std::runtime_error {
public:
    explicit CalculatorException(const std::string& message) 
        : std::runtime_error(message) {}
};

class DivisionByZeroException : public CalculatorException {
public:
    DivisionByZeroException() 
        : CalculatorException("Math Error: Division by zero is undefined.") {}
};

class InvalidDomainException : public CalculatorException {
public:
    explicit InvalidDomainException(const std::string& details) 
        : CalculatorException("Domain Error: " + details) {}
};

class MalformedExpressionException : public CalculatorException {
public:
    explicit MalformedExpressionException(const std::string& details) 
        : CalculatorException("Syntax Error: " + details) {}
};

class EmptyDatasetException : public CalculatorException {
public:
    EmptyDatasetException() 
        : CalculatorException("Statistical Error: Dataset cannot be empty.") {}
};

