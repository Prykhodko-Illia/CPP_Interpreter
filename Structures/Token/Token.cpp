#include <iostream>

struct Token {
    std::string content;
    std::string type;

    Token() = default;

    Token(std::string content, std::string type)
        : content(std::move(content)), type(std::move(type)) {}

    Token(const char content, const char* type)
        : content(1, content), type(type) {}
};