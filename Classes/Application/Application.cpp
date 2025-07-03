#include "Application.h"

double Application::Evaluate(const std::string &expression) {
    return Calculator::CalculatePolishNotation(
        ShuntingYard::ConvertPolishNotation(
            Tokenizer::inputCheck(
                Tokenizer::tokenPartite(
                        expression
                        ))));
}