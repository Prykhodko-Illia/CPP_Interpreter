#include "Calculator.h"

double getVarValue(const std::string &name) {
    FILE *file = fopen("../variables.txt", "r");
    if (!file) {
        std::cout << "Error opening file" << std::endl;
        exit(-1);
    }

    char buffer[40];
    while (fgets(buffer, 40, file)) {
        std::string toCheck;
        for (int j = 0; j < name.size(); ++j) toCheck += buffer[j];

        if (toCheck == name) {
            std::string resultStr;
            for (size_t j = name.size() + 1; buffer[j] != '\n'; ++j) resultStr += buffer[j];

            return std::stoi(resultStr);
        }
    }

    return -1;
}

double Calculator::CalculatePolishNotation(const std::vector<Token> &tokens,
                                            const std::unordered_map<std::string, double> &map) {
    if (tokens.empty()) {
        std::cout << "There are no tokens to calculate" << std::endl;
        exit(-1);
    }

    std::stack<double> stack;

    for (auto &token : tokens) {
        if (token.type == "number") stack.push(std::stod(token.content));
        if (token.type == "string") {
            if (!map.empty()) {
                if (map.contains(token.content)) {
                    stack.push(map.find(token.content)->second);
                }
            } else {
                stack.push(getVarValue(token.content));
            }
        }

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