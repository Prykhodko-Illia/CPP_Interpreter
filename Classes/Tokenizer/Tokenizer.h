#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <iostream>
#include <vector>
#include <stack>
#include "../classes.h"

class Tokenizer {
public:
    static std::vector<Token> tokenPartite(const std::string &text);
    static std::vector<Token> expressionCheck(const std::vector<Token> &tokens);
    // std::vector<Token> Tokenizer::templatesCheck(const std::vector<Token> &tokens, std::string templates[][]);
};



#endif //TOKENIZER_H
