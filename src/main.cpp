#include <iostream>
#include <string>

#include "grammar.h"
#include "lexer.h"
#include "lr0.h"
#include "parser.h"

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