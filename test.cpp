#include <iostream>
#include <vector>
#include <string>

struct Gate {
    std::string type;
    std::string output;
    std::vector<std::string> inputs;
    std::string boolean_form; // Added member to hold boolean form
};

void updateBooleanForms(std::vector<Gate>& gates, size_t startIdx) {
    for (size_t i = startIdx + 1; i < gates.size(); ++i) {
        for (auto& input : gates[i].inputs) {
            if (input == gates[startIdx].output) {
                input = gates[startIdx].boolean_form;
            }
        }
    }
}

std::string generateBooleanExpression(const std::vector<Gate>& gates) {
    std::string expression;
    for (const auto& gate : gates) {
        expression += gate.boolean_form;
    }
    return expression;
}

int main() {
    std::vector<Gate> gates = {
        {"and", "w1", {"A", "B"}, "(A & B)"},
        {"and", "w2", {"C", "D"}, "(C & D)"},
        {"or", "w3", {"w1", "w2"}, "(w1 | w2)"},
        {"not", "W", {"w3"}, "~(w3)"}
    };

    // Update boolean forms by replacing inputs with boolean forms of previous gates' outputs
    for (size_t i = 0; i < gates.size(); ++i) {
        updateBooleanForms(gates, i);
    }

    std::string boolean_expression = generateBooleanExpression(gates);
    std::cout << "Boolean expression: " << boolean_expression << std::endl;

    return 0;
}
