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

        if (token.content == "(" || token.content == "{") stack.push(token.content);
        else if (token.content == ")" || token.content == "}") {
            if (stack.empty()) {
                std::cout << "Brackets problem: closed bracket " << token.content << " before opened"<< std::endl;
                exit(-1);
            }
            stack.pop();
        }
    }

    if (!stack.empty()) {
        std::cout << "Brackets problem: Some bracket is not closed" << std::endl;
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

std::vector<Token> subVector(const std::vector<Token> &vector, const int start, const int end = std::numeric_limits<int>::max()) {
    std::vector<Token> result;
    if (end < start) {
        std::cout << "Cannot do sub vectoring: end index bigger than start" << std::endl;
        exit(-1);
    }

    if (start < 0) {
        std::cout << "Cannot do sub vectoring: start index less than 0" << std::endl;
        exit(-1);
    }

    for (int i = 0; i < vector.size(); ++i) {
        if (i >= start && i <= end) result.push_back(vector[i]);
    }

    return result;
}

void expressionCheck(const std::vector<Token> &tokens) {
    operationsCheck(tokens);
}

std::unordered_set<std::string> parametersCheck(const std::vector<Token> &tokens) {
    if (tokens.size() % 2 == 0) {
        std::cout << "Wrong functions parameters" << std::endl;
        exit(-1);
    }

    std::unordered_set<std::string> parameters;

    int i = 0;
    for (auto &token : tokens) {
        if (token.type == "string" && i % 2 == 0) {
            if (parameters.contains(token.content)) {
                std::cout << "Wrong parameters: " << token.content << " repeats twice" << std::endl;
                exit(-1);
            }
            parameters.insert(token.content);
        } else if (token.content == "," && i % 2 == 1) {}
        else {
            std::cout << "Wrong functions parameters" << tokens[i - 1].content << " " << token.content << std::endl;
            exit(-1);
        }

        ++i;
    }

    return parameters;
}

int getIndexBySymbol(const std::vector<Token> &vector, const std::string &symbol) {
    for (int i = 0; i < vector.size(); ++i) {
        if (vector[i].content == symbol) return i;
    }

    std::cout << "There are no such symbol in the vector" << std::endl;
    return -1;
}

std::vector<Token> Tokenizer::inputCheck(const std::vector<Token> &tokens) {
    if (tokens.empty()) {
        std::cout << "There are no tokens to check" << std::endl;
        exit(-1);
    }

    bracketsCheck(tokens);

    const int templateNumber = templatesCheck(tokens, templates);
    if (templateNumber == -1) return tokens;

    if (templateNumber == 0) {
        std::string variableName = tokens[1].content;
        std::vector<Token> expression = subVector(tokens, 3);
        expressionCheck(expression);
    }

    if (templateNumber == 1) {
        std::string functionName = tokens[1].content;
        std::vector<Token> parameters = subVector(tokens,
        getIndexBySymbol(tokens, "(") + 1, getIndexBySymbol(tokens, ")") - 1);
        parametersCheck(parameters);

        std::vector<Token> expression = subVector(tokens,
                getIndexBySymbol(tokens, "{") + 1, getIndexBySymbol(tokens, "}") - 1);
        expressionCheck(expression);
    }

    if (templateNumber == 2) {
        std::string functionName = tokens[1].content;
        std::vector<Token> parameters = subVector(tokens,
        getIndexBySymbol(tokens, "(") + 1, getIndexBySymbol(tokens, ")") - 1);
        parametersCheck(parameters);

        std::vector<Token> expression = subVector(tokens,
                getIndexBySymbol(tokens, "{") + 1, getIndexBySymbol(tokens, "}") - 1);
        expressionCheck(expression);
    }

    return tokens;
}