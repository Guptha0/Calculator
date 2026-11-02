#include <iostream>
#include <limits>
#include <string>
#include <stdexcept>

// ============================================================================
// 1. Domain Logic: Calculator Class (Encapsulated Arithmetic Computation)
// ============================================================================
class Calculator {
public:
    double add(double a, double b) const {
        return a + b;
    }

    double subtract(double a, double b) const {
        return a - b;
    }

    double multiply(double a, double b) const {
        return a * b;
    }

    double divide(double a, double b) const {
        if (b == 0.0) {
            throw std::invalid_argument("Division by zero is undefined.");
        }
        return a / b;
    }
};

// ============================================================================
// 2. Presentation / Input Validation Layer
// ============================================================================
namespace InputHelper {
    /**
     * @brief Safely reads a valid double from std::cin.
     * Keeps prompting until a valid number is supplied, gracefully handling
     * string/character inputs using std::cin.fail().
     */
    double getValidNumber(const std::string& prompt) {
        double value;
        while (true) {
            std::cout << prompt;
            if (std::cin >> value) {
                // Clear any trailing characters on the line
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return value;
            } else {
                std::cout << "[Input Error] Invalid number. Please enter a valid numerical value.\n";
                std::cin.clear(); // Reset failbit
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid characters
            }
        }
    }

    /**
     * @brief Safely reads an operator (+, -, *, /) or exit command ('q' / 'Q').
     */
    char getValidOperator() {
        char op;
        while (true) {
            std::cout << "Enter operation (+, -, *, /) or 'q' to quit: ";
            if (std::cin >> op) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                if (op == '+' || op == '-' || op == '*' || op == '/' || op == 'q' || op == 'Q') {
                    return op;
                }
                std::cout << "[Input Error] Invalid operator '" << op 
                          << "'. Supported operators: +, -, *, / (or 'q' to quit).\n";
            } else {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
    }
}

// ============================================================================
// 3. Application Driver Loop
// ============================================================================
int main() {
    Calculator calc;
    bool running = true;

    std::cout << "===========================================\n";
    std::cout << "      Auspify C/C++ Calculator Program     \n";
    std::cout << "===========================================\n";
    std::cout << "Supported operations: + (Add), - (Sub), * (Mul), / (Div)\n\n";

    while (running) {
        // Step 1: Prompt for operator or exit command
        char op = InputHelper::getValidOperator();
        if (op == 'q' || op == 'Q') {
            running = false;
            break;
        }

        // Step 2: Prompt for operands
        double num1 = InputHelper::getValidNumber("Enter first number: ");
        double num2 = InputHelper::getValidNumber("Enter second number: ");

        // Step 3: Compute and handle potential exceptions (e.g., division by zero)
        try {
            double result = 0.0;
            switch (op) {
                case '+':
                    result = calc.add(num1, num2);
                    break;
                case '-':
                    result = calc.subtract(num1, num2);
                    break;
                case '*':
                    result = calc.multiply(num1, num2);
                    break;
                case '/':
                    result = calc.divide(num1, num2);
                    break;
                default:
                    std::cout << "[Error] Unexpected operator encountered.\n";
                    continue;
            }

            // Step 4: Display the result cleanly
            std::cout << "\n-------------------------------------------\n";
            std::cout << "Result: " << num1 << " " << op << " " << num2 << " = " << result << "\n";
            std::cout << "-------------------------------------------\n\n";

        } catch (const std::invalid_argument& e) {
            std::cout << "\n-------------------------------------------\n";
            std::cout << "[Runtime Error] " << e.what() << "\n";
            std::cout << "-------------------------------------------\n\n";
        }
    }

    std::cout << "\nThank you for using the Calculator. Goodbye!\n";
    return 0;
}
