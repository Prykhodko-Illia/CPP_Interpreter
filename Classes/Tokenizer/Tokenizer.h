#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <iostream>
#include <vector>
#include <stack>
#include <limits>
#include <unordered_set>
#include "../classes.h"
#include "../Calculator/Calculator.h"
#include "../ShuntingYard/ShuntingYard.h"

class Tokenizer {
public:
    static std::vector<Token> tokenPartite(const std::string &text);
};



#endif //TOKENIZER_H
