#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "../classes.h"
#include <vector>
#include <stack>
#include <cmath>
#include <unordered_map>
#include <algorithm>


class Calculator {
public:
    static double CalculatePolishNotation(const std::vector<Token> &tokens, const std::unordered_map<std::string, double> &map = {});
};



#endif //CALCULATOR_H
