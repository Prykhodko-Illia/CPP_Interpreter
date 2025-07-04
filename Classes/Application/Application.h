#ifndef APPLICATION_H
#define APPLICATION_H

#include "../Calculator/Calculator.h"
#include "../Tokenizer/Tokenizer.h"
#include <unordered_map>
#include "../ShuntingYard/ShuntingYard.h"


class Application {
public:
    static void proceedInput(const std::string &input);
};



#endif //APPLICATION_H
