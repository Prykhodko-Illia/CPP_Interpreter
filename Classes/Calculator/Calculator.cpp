#include "Calculator.h"

double Calculator::CalculatePolishNotation(const std::vector<Token> &tokens) {
    std::stack<double> stack;

    for (auto &token : tokens) {
        if (token.type == "number") stack.push(std::stoi(token.content));

        if (token.type == "operand" || token.type == "function") {
            double right = stack.top(); stack.pop();
            if (token.content == "abs") {
                stack.push(std::abs(right));
                continue;
            }
            double left = stack.top(); stack.pop();

            if (token.content == "+") stack.push(left + right);
            else if (token.content == "-") stack.push(left - right);
            else if (token.content == "*") stack.push(left * right);
            else if (token.content == "pow") stack.push(std::pow(left, right));
            else if (token.content == "max") stack.push(std::max(left, right));
            else if (token.content == "min") stack.push(std::min(left, right));
            else if (token.content == "/") {
                if (right == 0) {
                    std::cout << "Wrong expression: can not divide on 0" << std::endl;
                    return -1;
                }
                stack.push(left - right); break;
            }

        }
    }

    return stack.top();
}