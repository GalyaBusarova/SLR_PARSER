#include <iostream>
#include <string>

#include "grammar.h"
#include "lexer.h"
#include "lr0.h"
#include "table.h"

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
        } else { // ERROR
            action_output = "Error";
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


int main() 
{
    try 
    {
        ArithmeticGrammar G;
        std::vector<Production> ExtendedGrammar;
        
        ExtendedGrammar.push_back({"S'", {"E"}, 0});
        auto baseGrammar = G.getProductions();
        
        for (size_t i = 0; i < baseGrammar.size(); i++) 
        {
            Production p = baseGrammar[i];
            p.id = static_cast<int>(i + 1);
            ExtendedGrammar.push_back(p);
        }

        auto lr0_automation = buildLR0Automaton(ExtendedGrammar);
        auto action_table = make_action_table(lr0_automation, ExtendedGrammar);
        auto goto_table = make_goto_table(lr0_automation, ExtendedGrammar);

        std::cout << "Enter arithmetic expression: ";
        std::string input_str;
        std::getline(std::cin, input_str);
        
        if (input_str.empty()) 
        {
            std::cout << "Empty input, exiting.\n";
            return 0;
        }

        std::vector<TokenId> input_tokens = tokenize(input_str);
        
        if (input_tokens.empty() || input_tokens.back() != TOK_END) 
        {
            std::cerr << "Error: Invalid token stream\n";
            return 1;
        }

        if (parser(input_tokens, action_table, goto_table, ExtendedGrammar)) 
        {
            std::cout << "Parsing successful!\n";
            return 0;
        } 
        else 
        {
            std::cout << "Syntax error\n";
            return 1;
        }
    } 
    catch (const std::exception& e) 
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}