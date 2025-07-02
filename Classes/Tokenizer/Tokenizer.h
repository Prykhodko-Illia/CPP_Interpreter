#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <iostream>
#include <vector>
#include "../classes.h"

class Tokenizer {
public:
    static std::vector<Token> tokenPartite(const std::string &text);
};



#endif //TOKENIZER_H
