#include <iostream>
#include "Classes/Tokenizer/Tokenizer.h"
#include "Classes/ShuntingYard/ShuntingYard.h"
#include "Classes/Calculator/Calculator.h"


int main() {
    auto vector = Tokenizer::tokenPartite("pow((5 + 3 * 2), 2) * abs(4 - 7)");
    auto vector1 = ShuntingYard::ConvertPolishNotation(vector);
    double result = Calculator::CalculatePolishNotation(vector1);

    for (auto &el : vector1) {
        std::cout << el.content << " ";
    }

    std::cout << std::endl << "Result: " << result << std::endl;

    return 0;
}