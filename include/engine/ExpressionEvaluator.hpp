#pragma once

#include <string>
#include <vector>
#include <stack>
#include <stdexcept>
#include <cmath>

class MathException : public std::runtime_error {
public:
    explicit MathException(const std::string& msg) : std::runtime_error(msg) {}
};

class ExpressionEvaluator {
public:
    enum class AngleMode { Degrees, Radians };

    ExpressionEvaluator();

    void setAngleMode(AngleMode mode);
    AngleMode getAngleMode() const;

    /**
     * @brief Evaluates an infix mathematical expression string.
     * Supports +, -, *, /, %, ^, sqrt, sin, cos, tan, log, ln, pi, e
     */
    double evaluate(const std::string& expression);

private:
    AngleMode m_angleMode;

    enum class TokenType { Number, Operator, Function, OpenParen, CloseParen };
    
    struct Token {
        TokenType type;
        std::string value;
        int precedence = 0;
        bool rightAssociative = false;
    };

    std::vector<Token> tokenize(const std::string& expr);
    std::vector<Token> toRPN(const std::vector<Token>& tokens);
    double evaluateRPN(const std::vector<Token>& rpnTokens);
    
    double applyFunction(const std::string& func, double arg);
    double applyOperator(const std::string& op, double b, double a);
    int getPrecedence(const std::string& op);
    bool isOperator(const std::string& token);
    bool isFunction(const std::string& token);
};

