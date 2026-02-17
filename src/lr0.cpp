#include <vector>
#include <string>
#include <queue>
#include <iostream>

#include "lr0.h"
#include "grammar.h"

// функция closure, необходимая для построения LR(0)-автомата
std::set<lr0_item> closure(const std::set<lr0_item>& items, const std::vector<Production>& grammar) 
{
    std::set<lr0_item> result = items;
    bool changed = true;
    
    while (changed) {
        changed = false;
        
        std::set<lr0_item> snapshot = result;  
        for (const lr0_item& item : snapshot) 
        {
            if (item.rule_id < 0 || item.rule_id >= grammar.size()) 
            {
                std::cerr << "ERROR in closure: item.rule_id = " << item.rule_id 
                          << " is out of bounds (grammar.size() = " << grammar.size() << ")\n";
                std::cerr << "Item: rule_id=" << item.rule_id << ", dot_position=" << item.dot_position << "\n";
                exit(1);
            }
            
            if (item.dot_position < grammar[item.rule_id].rhs.size()) 
            {
                std::string symbol = grammar[item.rule_id].rhs[item.dot_position];
                
                std::set<std::string> nonterminals = ArithmeticGrammar().getNonTerminals();
                
                if (nonterminals.find(symbol) != nonterminals.end()) 
                {
                    for (size_t i = 0; i < grammar.size(); i++) 
                    {
                        if (grammar[i].lhs == symbol) 
                        {
                            lr0_item new_item{static_cast<int>(i), 0};
                            
                            if (result.find(new_item) == result.end()) 
                            {
                                result.insert(new_item);
                                changed = true;
                            }
                        }
                    }
                }
            }
        }
    }
    
    return result;
}


// функция goto, необходимая для построения LR(0)-автомата
std::set<lr0_item> GoTo(const std::set<lr0_item>& situation_set, const std::string& symbol, const std::vector<Production>& grammar)
{
    std::set<lr0_item> goto_result;

    for (const lr0_item& situation : situation_set)
    {
        if (situation.dot_position < grammar[situation.rule_id].rhs.size() 
            && grammar[situation.rule_id].rhs[situation.dot_position] == symbol)
        {
            lr0_item GoTo_elem = {situation.rule_id, situation.dot_position + 1};
            goto_result.insert(GoTo_elem);
        }
    }

    return closure(goto_result, grammar);
}

// функция построения LR(0)-автомата
std::vector<lr0_state> buildLR0Automaton(const std::vector<Production>& ExtendedGrammar)
{
    std::vector<lr0_state> states;

    lr0_item start_item{0, 0};
    lr0_state start_state{0, closure({start_item}, ExtendedGrammar)};
    states.push_back(start_state);

    size_t i = 0;
    while (i < states.size())
    {
        std::set<std::string> symbols = GetSymbolsAfterDot(states[i], ExtendedGrammar);

        for (const std::string& symbol : symbols)
        {
            std::set<lr0_item> goto_set = GoTo(states[i].items, symbol, ExtendedGrammar);
            
            if (goto_set.empty())
            {
                continue;
            }
            
            bool found = false;
            for (size_t j = 0; j < states.size(); j++)
            {
                if (states[j].items == goto_set)
                {
                    found = true;
                    break;
                }
            }
            
            if (!found)
            {
                lr0_state new_state{static_cast<int>(states.size()), goto_set};
                states.push_back(new_state);
            }
        }
        
        i++;
    }

    return states;
}

// функцмя для получения символов после точки в текущем состоянии
std::set<std::string> GetSymbolsAfterDot(const lr0_state& cur_state, const std::vector<Production>& Grammar)
{
    std::set<std::string> symbols_after_dot;

    for (const lr0_item& item : cur_state.items)
    {
        if (item.rule_id >= 0 && item.rule_id < Grammar.size())
        {
            if (item.dot_position < Grammar[item.rule_id].rhs.size()) 
            {
                symbols_after_dot.insert(Grammar[item.rule_id].rhs[item.dot_position]);
            }
        }
    }

    return symbols_after_dot;
}