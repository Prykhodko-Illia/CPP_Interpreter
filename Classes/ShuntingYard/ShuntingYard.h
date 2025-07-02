#ifndef SHUNTINGYARD_H
#define SHUNTINGYARD_H

#include <vector>
#include <unordered_map>
#include <stack>
#include "../classes.h"

class ShuntingYard {
public:
    static std::vector<Token> ConvertPolishNotation(const std::vector<Token> &tokens);
};



#endif //SHUNTINGYARD_H
