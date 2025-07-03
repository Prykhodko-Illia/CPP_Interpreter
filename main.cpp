#include "Classes/Application/Application.h"


int main() {
    std::cout << "Write input here: " << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::cout << Application::Evaluate(input) << std::endl;

    return 0;
}