#ifndef APPLICATION_H
#define APPLICATION_H

#include "../Calculator/Calculator.h"
#include "../Tokenizer/Tokenizer.h"
#include "../ShuntingYard/ShuntingYard.h"


class Application {
public:
    static double Evaluate(const std::string &expression);
};



#endif //APPLICATION_H
