#include "ShuntingYard.h"

std::unordered_map<std::string, int> priorities = {
    {"+", 1},
    {"-", 1},
    {"*", 2},
    {"/", 2},
    {"pow", 3},
    {"abs", 4},
    {"max", 4},
    {"min", 4}
};

void pop_push(std::stack<Token> &stack, std::vector<Token> &result) {
    if (stack.top().content != "," && stack.top().content != "(") {
        result.push_back(stack.top());
    }
    stack.pop();
}

std::vector<Token> ShuntingYard::ConvertPolishNotation(const std::vector<Token> &tokens) {
    if (tokens.empty()) {
        std::cout << "There are no tokens to convert" << std::endl;
        exit(-1);
    }

    std::vector<Token> result;
    std::stack<Token> stack;
    for (auto &token : tokens) {

        if (token.type == "number" || token.type == "string") {
            result.push_back(token);
            continue;
        }

        if (token.type == "function" || token.content == "(") {
            stack.push(token);
            continue;
        }

        if (token.content == ")") {
            while (!stack.empty() && stack.top().content != "(") {
                pop_push(stack, result);
            }

            stack.pop();

            if (!stack.empty() && stack.top().type == "function") {
                pop_push(stack, result);
            }
            continue;
        }

        if (token.type == "operand") {
            while (!stack.empty() &&
                stack.top().content != "(" &&
                (priorities[stack.top().content] >= priorities[token.content] || token.content == ","))
                {
                pop_push(stack, result);
            }
            stack.push(token);
        }
    }

    while (!stack.empty()) {
        pop_push(stack, result);
    }

    return result;
}