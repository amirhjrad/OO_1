#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

struct Gate {
    std::string type;
    std::string output;
    std::vector<std::string> inputs;
    std::string boolean_form;
};

void getIOs(const std::string& inputFile, std::vector<std::string>& inputs, std::vector<std::string>& outputs)
{
    std::ifstream input_file(inputFile);
    if (!input_file.is_open())  
    {
        std::cerr << "Failed to open input file!" << std::endl;
        return;
    }
    std::string line;
    while (std::getline(input_file, line)) 
    {
        if (line.find("output") != std::string::npos)   outputs.push_back(line);
        if (line.find("input")  != std::string::npos)    inputs.push_back(line); 
    }
    input_file.close();
}

void parseVerilogNetlist(const std::string& input_file, std::vector<Gate>& gates) {
    std::ifstream file(input_file);
    std::string line;
    while (std::getline(file, line)) {
        if (line.find("and") != std::string::npos || line.find("or") != std::string::npos || line.find("not") != std::string::npos) {
            std::istringstream iss(line);
            Gate gate;
            iss >> gate.type;
            std::string token;
            iss >> token; 
            token = token.substr(token.find("(")+1, (token.find(",")-token.find("("))-1 );
            gate.output = token;
            while (iss >> token ) {
                if(token.find(",") != std::string::npos)
                    token = token.substr(0, token.find(","));
                if(token.find(")") != std::string::npos)
                    token = token.substr(0, token.find(")"));
                gate.inputs.push_back(token);
            }
            gates.push_back(gate);
        }
    }
    file.close();
}

std::string generateBooleanExpression(std::vector<Gate>& gates, std::vector<std::string>& outputs) {
    std::string expression;

    for (int i = 0; i < gates.size(); i++)
    {
        std::string sign;
        if (gates[i].type == "and")         sign = " & ";
        else if (gates[i].type == "or")     sign = " | ";
        else if (gates[i].type == "not")    gates[i].boolean_form += "~";
        if (i != gates.size() - 1) gates[i].boolean_form += "(";
        for (int j = 0; j< gates[i].inputs.size(); j++){
            gates[i].boolean_form += gates[i].inputs[j];
            if (j+1 < gates[i].inputs.size())   gates[i].boolean_form += sign;
        }
        if (i != gates.size() - 1)  gates[i].boolean_form += ")";
    }

    for (auto& gate1 : gates)
    {
        bool is_found = false;
        for (auto& gate2 : gates)
        {
            if (std::find(gate2.inputs.begin(), gate2.inputs.end(), gate1.output) != gate2.inputs.end()){
                size_t pos = gate2.boolean_form.find(gate1.output);
                if (pos != std::string::npos) {
                    gate2.boolean_form.replace(pos, gate1.output.length(), gate1.boolean_form);
                    is_found = true;
                }
            }
        }
        if (!is_found)  expression = gate1.boolean_form;
    }
    
    return expression;
}

void generateFunctionalVerilog(std::vector<Gate>& gates, std::vector<std::string> inputs, std::vector<std::string> outputs) {
    std::ofstream output_file("output.v");
    if (!output_file.is_open()) {
        std::cerr << "Unable to create output file." << std::endl;
        return;
    }
    output_file << "module Behavioral(" << std::endl;
    for (auto input : inputs)
            output_file << input << std::endl;
    for (auto output : outputs)
            output_file << output << std::endl;
    output_file << ");" << std::endl << std::endl;
    output_file << "    assign W = " + generateBooleanExpression(gates, outputs);
    output_file << ";" << std::endl << std::endl;
    output_file << "endmodule" << std::endl;
    output_file.close();
}





int main() {
    std::vector<std::string> inputs;
    std::vector<std::string> outputs;
    std::vector<Gate> gates;
    getIOs("input.v", inputs, outputs);
    parseVerilogNetlist("input.v", gates);
    generateFunctionalVerilog(gates, inputs, outputs);
    std::cout << "Done" << std::endl;
    return 0;
}
