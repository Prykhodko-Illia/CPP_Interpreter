#include "Application.h"

double Application::Evaluate(const std::string &expression) {
    return Calculator::CalculatePolishNotation(
        ShuntingYard::ConvertPolishNotation(
            Tokenizer::expressionCheck(
                Tokenizer::tokenPartite(
                        expression
                        ))));
}