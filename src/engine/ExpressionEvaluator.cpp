#include "engine/ExpressionEvaluator.hpp"
#include <sstream>
#include <cctype>
#include <algorithm>

ExpressionEvaluator::ExpressionEvaluator() 
    : m_angleMode(AngleMode::Degrees) {}

void ExpressionEvaluator::setAngleMode(AngleMode mode) {
    m_angleMode = mode;
}

ExpressionEvaluator::AngleMode ExpressionEvaluator::getAngleMode() const {
    return m_angleMode;
}

int ExpressionEvaluator::getPrecedence(const std::string& op) {
    if (op == "+" || op == "-") return 1;
    if (op == "*" || op == "/" || op == "%") return 2;
    if (op == "^") return 3;
    return 0;
}

bool ExpressionEvaluator::isOperator(const std::string& token) {
    return token == "+" || token == "-" || token == "*" || 
           token == "/" || token == "%" || token == "^";
}

bool ExpressionEvaluator::isFunction(const std::string& token) {
    return token == "sin" || token == "cos" || token == "tan" ||
           token == "sqrt" || token == "log" || token == "ln";
}

double ExpressionEvaluator::applyOperator(const std::string& op, double b, double a) {
    if (op == "+") return a + b;
    if (op == "-") return a - b;
    if (op == "*") return a * b;
    if (op == "/") {
        if (std::abs(b) < 1e-15) throw MathException("Division by zero is undefined.");
        return a / b;
    }
    if (op == "%") {
        if (std::abs(b) < 1e-15) throw MathException("Modulo by zero is undefined.");
        return std::fmod(a, b);
    }
    if (op == "^") return std::pow(a, b);
    throw MathException("Unsupported operator: " + op);
}

double ExpressionEvaluator::applyFunction(const std::string& func, double arg) {
    constexpr double PI = 3.14159265358979323846;
    double rad = (m_angleMode == AngleMode::Degrees) ? (arg * PI / 180.0) : arg;

    if (func == "sin") return std::sin(rad);
    if (func == "cos") return std::cos(rad);
    if (func == "tan") {
        if (std::abs(std::cos(rad)) < 1e-15) throw MathException("Tangent is undefined for this angle.");
        return std::tan(rad);
    }
    if (func == "sqrt") {
        if (arg < 0.0) throw MathException("Square root of negative number.");
        return std::sqrt(arg);
    }
    if (func == "log") {
        if (arg <= 0.0) throw MathException("Logarithm of non-positive value.");
        return std::log10(arg);
    }
    if (func == "ln") {
        if (arg <= 0.0) throw MathException("Natural log of non-positive value.");
        return std::log(arg);
    }
    throw MathException("Unsupported function: " + func);
}

std::vector<ExpressionEvaluator::Token> ExpressionEvaluator::tokenize(const std::string& expr) {
    std::vector<Token> tokens;
    size_t i = 0;
    const size_t len = expr.length();

    while (i < len) {
        if (std::isspace(static_cast<unsigned char>(expr[i]))) {
            ++i;
            continue;
        }

        char c = expr[i];

        if (std::isdigit(static_cast<unsigned char>(c)) || c == '.') {
            std::string num;
            while (i < len && (std::isdigit(static_cast<unsigned char>(expr[i])) || expr[i] == '.')) {
                num += expr[i++];
            }
            tokens.push_back({TokenType::Number, num, 0, false});
            continue;
        }

        if (std::isalpha(static_cast<unsigned char>(c))) {
            std::string name;
            while (i < len && std::isalpha(static_cast<unsigned char>(expr[i]))) {
                name += static_cast<char>(std::tolower(expr[i++]));
            }
            if (name == "pi") {
                tokens.push_back({TokenType::Number, "3.141592653589793", 0, false});
            } else if (name == "e") {
                tokens.push_back({TokenType::Number, "2.718281828459045", 0, false});
            } else if (isFunction(name)) {
                tokens.push_back({TokenType::Function, name, 4, true});
            } else {
                throw MathException("Unknown identifier: " + name);
            }
            continue;
        }

        if (c == '(') {
            tokens.push_back({TokenType::OpenParen, "(", 0, false});
            ++i;
            continue;
        }

        if (c == ')') {
            tokens.push_back({TokenType::CloseParen, ")", 0, false});
            ++i;
            continue;
        }

        // Operator handling (including unary minus check)
        std::string op(1, c);
        if (isOperator(op)) {
            // Unary minus check
            if (c == '-' && (tokens.empty() || tokens.back().type == TokenType::OpenParen || tokens.back().type == TokenType::Operator)) {
                // Read as negative number or unary 0 - x
                tokens.push_back({TokenType::Number, "0", 0, false});
            }
            bool rightAssoc = (c == '^');
            tokens.push_back({TokenType::Operator, op, getPrecedence(op), rightAssoc});
            ++i;
            continue;
        }

        throw MathException(std::string("Unrecognized character in expression: ") + c);
    }

    return tokens;
}

std::vector<ExpressionEvaluator::Token> ExpressionEvaluator::toRPN(const std::vector<Token>& tokens) {
    std::vector<Token> output;
    std::stack<Token> opStack;

    for (const auto& token : tokens) {
        if (token.type == TokenType::Number) {
            output.push_back(token);
        } else if (token.type == TokenType::Function) {
            opStack.push(token);
        } else if (token.type == TokenType::Operator) {
            while (!opStack.empty() && (opStack.top().type == TokenType::Operator || opStack.top().type == TokenType::Function)) {
                const auto& top = opStack.top();
                if ((top.type == TokenType::Function) ||
                    (!token.rightAssociative && token.precedence <= top.precedence) ||
                    (token.rightAssociative && token.precedence < top.precedence)) {
                    output.push_back(top);
                    opStack.pop();
                } else {
                    break;
                }
            }
            opStack.push(token);
        } else if (token.type == TokenType::OpenParen) {
            opStack.push(token);
        } else if (token.type == TokenType::CloseParen) {
            bool foundOpen = false;
            while (!opStack.empty()) {
                if (opStack.top().type == TokenType::OpenParen) {
                    foundOpen = true;
                    opStack.pop();
                    break;
                }
                output.push_back(opStack.top());
                opStack.pop();
            }
            if (!foundOpen) throw MathException("Mismatched closing parenthesis.");
            if (!opStack.empty() && opStack.top().type == TokenType::Function) {
                output.push_back(opStack.top());
                opStack.pop();
            }
        }
    }

    while (!opStack.empty()) {
        if (opStack.top().type == TokenType::OpenParen || opStack.top().type == TokenType::CloseParen) {
            throw MathException("Mismatched opening parenthesis.");
        }
        output.push_back(opStack.top());
        opStack.pop();
    }

    return output;
}

double ExpressionEvaluator::evaluateRPN(const std::vector<Token>& rpnTokens) {
    std::stack<double> valStack;

    for (const auto& token : rpnTokens) {
        if (token.type == TokenType::Number) {
            valStack.push(std::stod(token.value));
        } else if (token.type == TokenType::Operator) {
            if (valStack.size() < 2) throw MathException("Malformed expression syntax.");
            double b = valStack.top(); valStack.pop();
            double a = valStack.top(); valStack.pop();
            valStack.push(applyOperator(token.value, b, a));
        } else if (token.type == TokenType::Function) {
            if (valStack.empty()) throw MathException("Function missing argument.");
            double arg = valStack.top(); valStack.pop();
            valStack.push(applyFunction(token.value, arg));
        }
    }

    if (valStack.size() != 1) throw MathException("Invalid mathematical expression.");
    return valStack.top();
}

double ExpressionEvaluator::evaluate(const std::string& expression) {
    if (expression.empty()) return 0.0;
    auto tokens = tokenize(expression);
    auto rpn = toRPN(tokens);
    return evaluateRPN(rpn);
}

