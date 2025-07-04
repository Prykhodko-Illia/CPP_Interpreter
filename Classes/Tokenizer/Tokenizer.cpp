#include "Tokenizer.h"

bool isNumber(const char c) {
    for (const auto el : {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'}) {
        if (c == el) return true;
    }

    return false;
}

bool isOperand(const char c) {
    for (const auto el : {'+', '-', '*', '/', '='}) {
        if (c == el) return true;
    }

    return false;
}

bool isBracket(const char c) {
    for (const auto el : {'(', ')', '{', '}'}) {
        if (c == el) return true;
    }

    return false;
}

bool isFunc(const std::string &t) {
    for (const auto el : {"pow", "min", "max", "abs"}) {
        if (t == el) return true;
    }

    return false;
}

bool isCommand(const std::string &t) {
    for (const auto el : {"var", "def"}) {
        if (t == el) return true;
    }
    return false;
}

std::vector<Token> Tokenizer::tokenPartite(const std::string &text){
    std::vector<Token> result;

    std::string currentScope;
    std::string token;

    for (int i = 0; text[i] != '\0'; ++i) {
        if (text[i] == ' ' || isOperand(text[i]) || text[i] == ',' || isBracket(text[i])) {
            if (!token.empty()) {
                if (isFunc(token)) currentScope = "function";
                if (isCommand(token)) currentScope = "command";

                result.emplace_back(token, currentScope);
            }

            currentScope = "";
            token = "";
            if (isBracket(text[i])) result.emplace_back(text[i], "brackets");
            else if (text[i] != ' ') result.emplace_back(text[i], "operand");
            continue;
        }

        if (isNumber(text[i]) && currentScope != "string") {
            if (currentScope.empty()) currentScope = "number";
        } else {
            if (currentScope.empty()) currentScope = "string";
        }
        token += text[i];
    }

    if (!token.empty()) {
        result.emplace_back(token, currentScope);
    }

    return result;
}

