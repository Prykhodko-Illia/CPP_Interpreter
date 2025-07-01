#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <iostream>
#include <utility>
#include <vector>

struct Token {
    std::string content;
    std::string type;

    Token() = default;

    Token(std::string content, std::string type)
        : content(std::move(content)), type(std::move(type)) {}

    Token(const char content, const char* type)
        : content(1, content), type(type) {}
};

class Tokenizer {
public:
    static std::vector<Token> tokenPartite(const std::string &text);
};



#endif //TOKENIZER_H
