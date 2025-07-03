#include "Application.h"

double Application::Evaluate(const std::string &expression) {
    const auto tokens = Tokenizer::tokenPartite(expression);
    const auto rpnTokens = ShuntingYard::ConvertPolishNotation(tokens);
    return Calculator::CalculatePolishNotation(rpnTokens);
}