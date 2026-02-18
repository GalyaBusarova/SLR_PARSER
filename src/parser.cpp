#include <iostream>

#include "parser.h"
#include "token.h"

// функция, переводящая правило в строку
std::string RuleToString(const Production& prod)
{
    std::string result;

    result += prod.lhs;
    result += "->";
    for (const std::string& symbol : prod.rhs)
    {
        result += symbol;
    }

    return result;
}

// функция, переводящая стек строк в строку
std::string StackToString(const std::vector<std::string>& str_stack)
{
    std::string result;

    for (const std::string& str : str_stack)
    {
        result += str;
    }

    return result;
}

// функция, переводящая входную последовательность токенов в строку
std::string InputToString(const std::vector<enum TokenId>& input_tokens)
{
    std::string result;

    std::string elem;

    for (size_t i = 0; i < input_tokens.size(); i++)
    {
        elem = tokenToString(input_tokens[i]);
        result += elem;
    }

    return result;
}

// основная функция парсинга
bool parser(
    const std::vector<TokenId>& tokens,
    const std::vector<std::vector<Action>>& action_table,
    const std::vector<std::map<std::string, int>>& goto_table,
    const std::vector<Production>& ExtendedGrammar)
{
    std::vector<int> state_stack;      // стек состояний 
    std::vector<std::string> symbol_stack;  // стек символов 

    state_stack.push_back(0);
    symbol_stack.push_back("$");

    std::vector<TokenId> input = tokens;

    # if 0
    // ОТЛАДКА: проверка входа
    std::cout << "Parser: input.size() = " << input.size() << "\n";
    if (input.empty()) {
        std::cout << "Parser: ERROR - input is empty!\n";
        return false;
    }
    std::cout << "Parser: first token = " << tokenToString(input[0]) << "\n";
    std::cout << "Parser: action_table[0][" << input[0] << "] = ";
    
    Action first_action = action_table[0][input[0]];
    switch (first_action.type) {
        case SHIFT: std::cout << "SHIFT " << first_action.value; break;
        case REDUCE: std::cout << "REDUCE " << first_action.value; break;
        case ACCEPT: std::cout << "ACCEPT"; break;
        case ERROR: std::cout << "ERROR"; break;
    }
    std::cout << "\n";
    // конец отладки
    #endif

    std::cout << "STACK\tINPUT\tACTION\n";

    while (true) 
    {
        int cur_state = state_stack.back();
    
        if (input.empty()) {
            std::cout << StackToString(symbol_stack) << "\t\tError\n";
            return false;
        }
    
        TokenId cur_token = input[0];
    
        std::string stack_output = StackToString(symbol_stack);
        std::string inp_output = InputToString(input);
    
        Action action = action_table[cur_state][cur_token];
        std::string action_output;
    
        if (action.type == SHIFT) {
            action_output = "Shift " + std::to_string(action.value);
        } else if (action.type == REDUCE) {
            action_output = "Reduce " + RuleToString(ExtendedGrammar[action.value]);
        } else if (action.type == ACCEPT) {
            action_output = "Accept";
            return true;
        } else { // ERROR
            action_output = "Error";
            return false;
        }
    
    
        std::cout << stack_output << "\t" << inp_output << "\t" << action_output << "\n";
    
    
        if (action.type == SHIFT) {
            symbol_stack.push_back(tokenToString(cur_token));
            state_stack.push_back(action.value);
            input.erase(input.begin());
        } else if (action.type == REDUCE) {
            const Production& cur_prod = ExtendedGrammar[action.value];
            size_t erase_count = cur_prod.rhs.size();
            for (size_t i = 0; i < erase_count; i++) {
                state_stack.pop_back();
                symbol_stack.pop_back();
            }
            symbol_stack.push_back(cur_prod.lhs);
            int new_state = goto_table[state_stack.back()].at(cur_prod.lhs);
            state_stack.push_back(new_state);
        } else if (action.type == ACCEPT) {
            return true;  
        } else { 
            return false; 
        }
    }
}