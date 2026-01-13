#include <iostream>
#include <cmath>
#include <array>
#include <algorithm>
#include <vector>
#include <iomanip>
#include <sstream>
#include <readline/readline.h>
#include <readline/history.h>


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
    if((!isdigit(equation.front()) && equation.front() != '(' && equation.front() != '-') || (!isdigit(equation.back()) && equation.back() != ')')){
        return true;
    }
    return false;
}


// Main logic of the entire program, deal with parentheses and deals with the main BODMAS logic.
double BODMAS(const std::string& equation){
    // Declaring all variables used in the function.
    std::array<char, 6> negative_number_checklist = {'(', '+', '-', '*', '/', '^'};
    std::string number_builder;
    std::vector<double> numbers;
    std::vector<char> operators;
    std::string nested_equation;
    int nested_depth = 0;

    // Main for loop designed to organize the equation in 2 vectors, a vector of numebers and a vector of operators.
    for(int i = 0; i < equation.size(); i++){
        bool is_negative_number = equation[i] == '-' && (i == 0 || std::find(negative_number_checklist.begin(), negative_number_checklist.end(), equation[i - 1]) != negative_number_checklist.end());

        if(isdigit(equation[i]) || equation[i] == '.' || is_negative_number){
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
            double nested_solution = BODMAS(nested_equation);
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
    double number;
    int i;
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


// Formatting the result to a more readable version, getting rid of scientific notation, adding commas every 3 numbers and dealing with decimal numbers properly by not having hanging 0s and to skip all decimal numbers when adding commas.
std::string format_number(const double& result){
    std::ostringstream string_stream;
    std::string formatted_result;

    // Makes the result more verbose.
    if(result == floor(result)){
        string_stream << std::setprecision(0) << std::fixed << result;
        formatted_result = string_stream.str();
    }
    else{
        string_stream << std::setprecision(6) << std::fixed << result;
        formatted_result = string_stream.str();
        // Gets rid of any trailing 0s for decimal numbers.
        while(formatted_result.back() == '0'){
            formatted_result.pop_back();
        }
    }

    // Sets up where to start the index to include commas with numbers greater than 1000.
    int i = 0;
    if(formatted_result.find('.') == std::string::npos){
        i = formatted_result.length() - 1;
    }
    else{
        i = formatted_result.find('.') - 1;
    }

    // Adds commas every 3 numbers.
    int counter = 0;
    while(i >= 0){
        counter++;
        if((i == 1 && formatted_result[0] == '-') || i == 0){
            break;
        }
        if(counter == 3){
            formatted_result.insert(i, ",");
            counter = 0;
        }
        i--;
    }

    return formatted_result;
}


// Main function dealing with the main loop so user can keep inputting equations and getting the results.
int main(){
    char* user_input = nullptr;
    std::string equation;
    double result;
    std::cout << "CLI Calculator\n";
    std::cout << "Type 'exit' or 'quit' to exit, 'help' for more info.\n";

    while(true){
        free(user_input);
        
        user_input = readline("> ");

        if(user_input == nullptr){
            std::cout << "Exiting..." << std::endl;
            break;
        }

        equation = user_input;
        

        if(equation.empty()){
            continue;
        }
        else{
            add_history(user_input);
        }
        
        if(equation == "quit" || equation == "exit"){
            std::cout << "Exiting..." << std::endl;
            break;
        }
        
        if(equation == "help"){
            std::cout << "\nOperators:\n";
            std::cout << "  +    Addition\n";
            std::cout << "  -    Subtraction\n";
            std::cout << "  *    Multiplication\n";
            std::cout << "  /    Division\n";
            std::cout << "  ^    Power\n";
            std::cout << "  ()   Brackets\n\n";
            std::cout << "Features:\n";
            std::cout << "  - Use LEFT arrow and RIGHT arrow keys to edit equations\n";
            std::cout << "  - Press UP arrow to recall previous equations\n";
            std::cout << "  - Type 'exit' or 'quit' to exit\n\n";
            continue;
        }
        
        equation = white_space_remover(equation);

        if(invalid_character_checker(equation)){
            std::cout << "Error: Syntax Error\n";
            continue;
        }

        result = BODMAS(equation);

        if(std::isnan(result)){
            continue;
        }

        std::cout << " = " << format_number(result) << std::endl;
    }
    return 0;
}
