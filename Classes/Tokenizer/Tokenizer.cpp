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

std::vector<std::vector<std::string>> templates = {
    {"var", "*string", "=", "expression"},
    {"def", "*string", "(", "...", ")", "{", "expression", "}"},
    {"var", "*string", "(", "...", ")", "=", "{", "expression", "}"}
};

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

void operationsCheck(const std::vector<Token> &tokens) {
    std::string lastTokenType;
    std::string lastTokenContent;

    for (const auto &token : tokens) {
        if (token.type == "string") {
            std::cout << "Unresolved token name: " << token.content << std::endl;
            exit(-1);
        }

        if (token.type == lastTokenType && token.content != "(" && token.content != ")") {
            if (token.type == "number")
                std::cout << "Invalid expression: two consecutive numbers: ";
            else if (token.type == "operand")
                std::cout << "Invalid expression: two consecutive operands: ";
            std::cout << lastTokenContent << " " << token.content << std::endl;
            exit(-1);
        }

        lastTokenType = token.type;
        lastTokenContent = token.content;
    }
}

void bracketsCheck(const std::vector<Token> &tokens) {
    std::stack<std::string> stack;
    for (const auto &token : tokens) {
        if (token.type != "brackets") continue;

        if (token.content == "(") stack.push(token.content);
        else if (token.content == ")") {
            if (stack.empty()) {
                std::cout << "Brackets problem: ')' before the '('" << std::endl;
                exit(-1);
            }
            stack.pop();
        }
    }

    if (!stack.empty()) {
        std::cout << "Brackets problem: '(' is not closed" << std::endl;
        exit(-1);
    }
}

int checkTemplate(const std::vector<std::string> &t, const std::vector<Token> &tokens) {
    int j = -1;
    for (int i = 0; i < t.size(); ++i) {
        ++j;
        if (tokens.empty()) return 0;

        if (t[i][0] == '*') { if (tokens[j].type == t[i].substr(1)) continue; }
        else { if (tokens[j].content == t[i]) continue; }

        if (i == t.size() - 1) break;
        if (t[i] == "..." || t[i] == "expression") {
            bool flag = false;
            while (j < tokens.size()) {
                if (tokens.empty()) return 0;

                if (t[i + 1][0] == '*') {
                    if (tokens[j].type == t[i + 1].substr(1))
                    {flag = true; break;}
                }
                else {
                    if (tokens[j].content == t[i + 1])
                    {flag = true; break;}
                }
                ++j;
            }
            if (flag) ++i;
            else return 0;
        } else return 0;
    }

    return 1;
}

int templatesCheck(const std::vector<Token> &tokens, const std::vector<std::vector<std::string>>& templates) {
    int i = 0;
    for (const auto &t : templates) {
        if (checkTemplate(t, tokens)) {
            std::cout << i;
            return i;
        }
        ++i;
    }
    std::cout << -1;
    return -1;
}

std::vector<Token> Tokenizer::expressionCheck(const std::vector<Token> &tokens) {
    if (tokens.empty()) {
        std::cout << "There are no tokens to check" << std::endl;
        exit(-1);
    }

    templatesCheck(tokens, templates);
    operationsCheck(tokens);
    bracketsCheck(tokens);

    return tokens;
}