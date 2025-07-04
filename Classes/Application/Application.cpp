#include "Application.h"

std::vector<std::vector<std::string>> templates = {
    {"var", "*string", "=", "expression"},
    {"def", "*string", "(", "...", ")", "{", "expression", "}"},
    {"var", "*string", "(", "...", ")", "=", "{", "expression", "}"}
};

void operationsCheck(const std::vector<Token> &tokens, const std::unordered_set<std::string> &params = {}) {
    std::string lastTokenType;
    std::string lastTokenContent;

    for (const auto &token : tokens) {
        if (token.type == "string") {
            if (params.empty() || !params.contains(token.content)) {
                std::cout << "Unresolved token name: " << token.content << std::endl;
                exit(-1);
            }
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
            return i;
        }
        ++i;
    }
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

int variableInFile(const std::string &name, const std::string &filepath) {
    FILE *file = fopen(filepath.c_str(), "r");
    if (!file) {
        std::cout << "Error opening file" << std::endl;
        exit(-1);
    }

    char buffer[40];
    int i = 0;
    while (fgets(buffer, 40, file)) {
        std::string toCheck;
        for (int j = 0; j < name.size(); ++j) toCheck += buffer[j];

        if (toCheck == name) return i;
        ++i;
    }

    return -1;
}

void Application::proceedInput(const std::string &input) {
    const std::vector<Token> &tokens = Tokenizer::tokenPartite(input);

    if (tokens.empty()) {
        std::cout << "There are no tokens to check" << std::endl;
        exit(-1);
    }

    bracketsCheck(tokens);

    const int templateNumber = templatesCheck(tokens, templates);
    if (templateNumber == -1) std::cout << Calculator::CalculatePolishNotation(
            ShuntingYard::ConvertPolishNotation(tokens)) << std::endl;

    if (templateNumber == 0) {
        const std::string variableName = tokens[1].content;
        const std::vector<Token> expression = subVector(tokens, 3);
        expressionCheck(expression);

        const double exprResult = Calculator::CalculatePolishNotation(
            ShuntingYard::ConvertPolishNotation(expression));

        if (variableInFile(variableName,"..//variables.txt") != -1) {
            std::cout << "Variable " << variableName << " already exist" << std::endl;
            exit(-1);
        }

        FILE *file = fopen("..//variables.txt", "a");
        if (!file) {
            std::cout << "Error opening file" << std::endl;
            exit(-1);
        }

        const std::string textToWrite = variableName + " : " + std::to_string(exprResult) + '\n';
        fwrite(textToWrite.c_str(), sizeof(char), textToWrite.size(), file);

        fclose(file);

        std::cout << "Variable " << variableName << " was written to file as " << exprResult << std::endl;
    }

    if (templateNumber == 1) {
        const std::string functionName = tokens[1].content;
        const std::vector<Token> parameters = subVector(tokens,
        getIndexBySymbol(tokens, "(") + 1, getIndexBySymbol(tokens, ")") - 1);

        std::unordered_set<std::string> params_checked = parametersCheck(parameters);

        const std::vector<Token> expression = subVector(tokens,
                getIndexBySymbol(tokens, "{") + 1, getIndexBySymbol(tokens, "}") - 1);
        operationsCheck(expression, params_checked);

        if (variableInFile(functionName,"..//functions.txt") != -1) {
            std::cout << "Function " << functionName << " already exist" << std::endl;
            exit(-1);
        }

        FILE *file = fopen("..//functions.txt", "a");
        if (!file) {
            std::cout << "Error opening file" << std::endl;
            exit(-1);
        }

        {
            constexpr char delimiter = ':';
            const std::string textToWrite = functionName + delimiter;
            fwrite(textToWrite.c_str(), sizeof(char), textToWrite.size(), file);
        }

        for (const auto &parameter : params_checked) {
            fwrite(parameter.c_str(), sizeof(char), parameter.size(), file);
            constexpr char delimiter = ',';
            fwrite(&delimiter, sizeof(char), 1, file);
        }

        {
            constexpr char delimiter = ':';
            fwrite(&delimiter, sizeof(char), 1, file);
        }

        for (const auto &token : expression) {
            fwrite(token.content.c_str(), sizeof(char), token.content.size(), file);
        }

        {
            constexpr char endLine = '\n';
            fwrite(&endLine, sizeof(char), 1, file);
        }

        fclose(file);

        std::cout << "Function " << functionName << " was written to file" << std::endl;
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
}
