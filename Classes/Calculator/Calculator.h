#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "../classes.h"
#include <vector>
#include <stack>
#include <cmath>
#include <algorithm>


class Calculator {
public:
    static double CalculatePolishNotation(const std::vector<Token> &tokens);
};



#endif //CALCULATOR_H
