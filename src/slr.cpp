#include <vector>
#include <string>
#include <set>
#include <iostream>

#include "lr0.h"
#include "grammar.h"
#include "follow.h"
#include "table.h"

// функция создания action-таблицы
std::vector<std::vector<Action>> make_action_table(
    const std::vector<lr0_state>& lr0_automation, 
    const std::vector<Production>& ExtendedGrammar
) 
{
    std::set<std::string> terminals = ArithmeticGrammar().getTerminals();
    auto follow_sets = getFollowSets();

    std::vector<std::vector<Action>> action_table(
        lr0_automation.size(), 
        std::vector<Action>(TERM_COUNT, Action(ERROR, 0))
    );

    for (size_t i = 0; i < lr0_automation.size(); i++) 
    {
        for (const lr0_item& item : lr0_automation[i].items) 
        {
            if (item.rule_id < 0 || item.rule_id >= static_cast<int>(ExtendedGrammar.size())) 
            {
                continue;
            }
            
            const Production& prod = ExtendedGrammar[item.rule_id];
            
            if (item.dot_position == static_cast<int>(prod.rhs.size())) 
            {
                if (item.rule_id == 0) 
                {
                    action_table[i][NL] = Action(ACCEPT, 0);
                } 
                else 
                {
                    std::string lhs = prod.lhs;
                    if (follow_sets.find(lhs) != follow_sets.end()) 
                    {
                        for (Terminals terminal : follow_sets.at(lhs)) 
                        {
                            action_table[i][terminal] = Action(REDUCE, item.rule_id);
                        }
                    }
                }
            }
            else 
            {
                std::string symbol = prod.rhs[item.dot_position];
                
                if (terminals.find(symbol) != terminals.end()) 
                {
                    std::set<lr0_item> goto_items = GoTo(lr0_automation[i].items, symbol, ExtendedGrammar);
                    
                    int next_state = -1;
                    for (size_t idx = 0; idx < lr0_automation.size(); ++idx) 
                    {
                        if (lr0_automation[idx].items == goto_items) 
                        {
                            next_state = static_cast<int>(idx);
                            break;
                        }
                    }
                    
                    if (next_state != -1) 
                    {
                        if (terminals_table.find(symbol) != terminals_table.end()) 
                        {
                            Terminals tok = terminals_table.at(symbol);
                            action_table[i][tok] = Action(SHIFT, next_state);
                        }
                    }
                }
            }
        }
    }

    return action_table;
}

// функция для построения goto-таблицы, для каждого состояния и нетерминала: GOTO[i][B] = nextStateIndex
std::vector<std::map<std::string, int>> make_goto_table(
    const std::vector<lr0_state>& lr0_automation, 
    const std::vector<Production>& ExtendedGrammar
) 
{
    std::set<std::string> nonterminals = ArithmeticGrammar().getNonTerminals();
    nonterminals.insert(ExtendedGrammar[0].lhs);

    std::vector<std::map<std::string, int>> goto_table(lr0_automation.size());

    for (size_t i = 0; i < lr0_automation.size(); i++) 
    {
        for (const lr0_item& item : lr0_automation[i].items) 
        {
            if (item.rule_id < 0 || item.rule_id >= static_cast<int>(ExtendedGrammar.size())) 
            {
                continue;
            }
            
            if (item.dot_position < ExtendedGrammar[item.rule_id].rhs.size()) 
            {
                std::string symbol = ExtendedGrammar[item.rule_id].rhs[item.dot_position];
                
                if (nonterminals.find(symbol) != nonterminals.end()) 
                {
                    std::set<lr0_item> goto_items = GoTo(lr0_automation[i].items, symbol, ExtendedGrammar);
                    
                    int nextStateIndex = -1;
                    for (size_t idx = 0; idx < lr0_automation.size(); ++idx) 
                    {
                        if (lr0_automation[idx].items == goto_items) 
                        {
                            nextStateIndex = static_cast<int>(idx);
                            break;
                        }
                    }
                    
                    if (nextStateIndex != -1) 
                    {
                        goto_table[i][symbol] = nextStateIndex;
                    }
                }
            }
        }
    }

    return goto_table;
}