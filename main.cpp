#include <iostream>
#include "Classes/Tokenizer/Tokenizer.h"
#include "Classes/ShuntingYard/ShuntingYard.h"


int main() {
    auto vector = Tokenizer::tokenPartite("(5 + 3 * 2) * (7 - 4)");
    auto vector1 = ShuntingYard::ConvertPolishNotation(vector);

    for (auto &el : vector1) {
        std::cout << el.content << " ";
    }

    return 0;
}