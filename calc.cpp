#include <iostream>
#include <cmath>
#include <array>
#include <algorithm>
#include <vector>


// Removing all white space from equation.
std::string white_space_remover(const std::string& equation){
    std::string clean_equation;
    for(char character : equation){
        if(character != ' '){
            clean_equation += character;
        }
    }
    return clean_equation;
}


// Checks to see if user inputed a valid characters and did not start or end the equation with arithmatic operators.
bool invalid_character_checker(const std::string& equation){
    std::array<char, 8> valid_operators = {'.', '(', ')', '+', '-', '*', '/', '^'};
    // Checks to see if user included any characters not valid for the calculator within their equation.
    for(char character : equation){
        if(!isdigit(character) && std::find(valid_operators.begin(), valid_operators.end(), character) == valid_operators.end())
        return true;
    }
    // Checks to see if user starts or end their equation with an arithmatic operator.
    if((!isdigit(equation.front()) && equation.front() != '(') || (!isdigit(equation.back()) && equation.back() != ')')){
        return true;
    }
    return false;
}


// Main logic of the entire program, deal with parentheses and deals with the main BODMAS logic.
double arithmatic_logic(const std::string& equation){
    // Declaring all variables used in the function.
    std::string number_builder;
    std::vector<double> numbers;
    std::vector<char> operators;
    int i;
    double number;
    std::string nested_equation;
    int nested_depth = 0;

    // Main for loop designed to organize the equation in 2 vectors, a vector of numebers and a vector of operators.
    for(int i = 0; i < equation.size(); i++){
        if(isdigit(equation[i]) || equation[i] == '.'){
            number_builder += equation[i];
        }
        else if(equation[i] == '('){
            // Nested for loop for dealing with any parentheses, after the for loop is done this else if statement will recall the arithmatic_logic() funciton as a nested call to solve the equation within the parentheses and therefore slot the solution for the parentheses within the numbers vector.
            if(!number_builder.empty()){
                std::cout << "Error: Syntax Error\n";
                return NAN;
            }
            nested_equation = "";
            nested_depth++;
            for(int j = i + 1; j < equation.size(); j++){
                if(equation[j] == '('){
                    nested_depth++;
                    nested_equation += equation[j];
                }
                else if(equation[j] == ')'){
                    nested_depth--;
                    if(nested_depth == 0){
                        i = j;
                        break;
                    }
                    else{
                        nested_equation += equation[j];
                    }
                }
                else{
                    nested_equation += equation[j];
                }
            }
            if(nested_equation.empty()){
                std::cout << "Error: Empty Parentheses!\n";
                return NAN;
            }
            else if(nested_depth != 0){
                std::cout << "Error: Unmatched Parentheses!\n";
                return NAN;
            }
            double nested_solution = arithmatic_logic(nested_equation);
            numbers.push_back(nested_solution);
        }
        else if(equation[i] == ')'){
            std::cout << "Error: Unmatched Parentheses!\n";
            return NAN;
        }
        else{
            if(number_builder.empty() && numbers.size() == operators.size()){
                std::cout << "Error: Invalid Operator Placement.\n";
                return NAN;
            }
            else if(!number_builder.empty()){
                numbers.push_back(std::stod(number_builder));
                number_builder.clear();
            }
            operators.push_back(equation[i]);
        }
    }
    
    // Puts last number in the numbers list.
    if(!number_builder.empty()){
        numbers.push_back(std::stod(number_builder));
    }

    // This while loop deals with all power of operators in the equation.
    i = operators.size() - 1;
    while(i >= 0){
        switch(operators[i]){
            case '^':
                number = pow(numbers[i], numbers[i + 1]);
                numbers[i] = number;
                numbers.erase(numbers.begin() + i + 1);
                operators.erase(operators.begin() + i);
                break;
        }
        i--;
    }
    
    // This while loop deals with all multiplication and division in the equation.
    i = 0;
    while(i < operators.size()){
        switch(operators[i]){
            case '*':
                number = numbers[i] * numbers[i + 1];
                numbers[i] = number;
                numbers.erase(numbers.begin() + i + 1);
                operators.erase(operators.begin() + i);
                break;
            case '/':
                if(numbers[i + 1] == 0){
                    std::cout << "Error: Division by zero!\n";
                    return NAN;
                }
                number = numbers[i] / numbers[i + 1];
                numbers[i] = number;
                numbers.erase(numbers.begin() + i + 1);
                operators.erase(operators.begin() + i);
                break;
            default: i++;
        }
    }

    // This while loop deals with all addition and subtraction in the equation.
    i = 0;
    while(i < operators.size()){
        switch(operators[i]){
            case '+':
                number = numbers[i] + numbers[i + 1];
                numbers[i] = number;
                numbers.erase(numbers.begin() + i + 1);
                operators.erase(operators.begin() + i);
                break;
            case '-':
                number = numbers[i] - numbers[i + 1];
                numbers[i] = number;
                numbers.erase(numbers.begin() + i + 1);
                operators.erase(operators.begin() + i);
                break;
        }
    }

    return numbers[0];
}


// Main function dealing with the main loop so user can keep inputting equations and getting the results.
int main(){
    std::string equation;
    double result;
    std::cout << "Calculator Program\n";

    while(true){
        std::cout << "> ";
        std::getline(std::cin, equation);

        if(equation.empty()){
            continue;
        }

        if(equation == "quit" || equation == "exit"){
            std::cout << "Exiting..." << std::endl;
            break;
        }

        equation = white_space_remover(equation);

        if(invalid_character_checker(equation)){
            std::cout << "Error: Syntax Error\n";
            continue;
        }

        result = arithmatic_logic(equation);

        if(std::isnan(result)){
            continue;
        }
        
        std::cout << " = " << result << '\n';
    }
    return 0;
}