#include <set>
#include <string>
#include <map>

#include "grammar.h"
#include "follow.h"


std::map<std::string, std::set<enum Terminals>> getFirstSets() {
    return {
        {"E", {LP, ID, NUM}},
        {"T", {LP, ID, NUM}},
        {"F", {LP, ID, NUM}}
    };
}

std::map<std::string, std::set<Terminals>> getFollowSets() {
    std::map<std::string, std::set<Terminals>> follow;
    
    // S' -> E, поэтому $ ∈ FOLLOW(E)
    // E -> E + T, поэтому + ∈ FOLLOW(E)
    // E -> E - T, поэтому - ∈ FOLLOW(E)
    // F -> ( E ), поэтому ) ∈ FOLLOW(E)
    follow["E"] = {NL, PLUS, MINUS, RP};  // $, +, -, )
    
    // E -> T, поэтому FOLLOW(T) содержит всё из FOLLOW(E)
    // T -> T * F, поэтому * ∈ FOLLOW(T)
    // T -> T / F, поэтому / ∈ FOLLOW(T)
    follow["T"] = {NL, PLUS, MINUS, MUL, DIV, RP};  // $, +, -, *, /, )
    
    // T -> F, поэтому FOLLOW(F) содержит всё из FOLLOW(T)
    follow["F"] = {NL, PLUS, MINUS, MUL, DIV, RP};  // $, +, -, *, /, )
    
    // S' всегда содержит только $
    follow["S'"] = {NL};
    
    return follow;
}


#if 0
// функция first для нахождения множества терминалов, с которых могут начинаться цепочки, выводимые из нетерминала non_terminal
std::set<std::string> first(const std::vector<Production>& extendedGrammar, std::string non_terminal, std::set<std::string>& visited)
{
    std::set<std::string> result;

    if (visited.find(non_terminal) != visited.end())
    {
        return result;
    }
    visited.insert(non_terminal);

    ArithmeticGrammar full_grammar;
    std::set<std::string> terminals = full_grammar.getTerminals();

    for (const Production& prod : extendedGrammar)
    {
        if (prod.lhs == non_terminal)
        {
            if (terminals.find(prod.rhs[0]) != terminals.end())
            {
                result.insert(prod.rhs[0]);
            }

            else
            {
                std::set<std::string> first_of_nonterm = first(extendedGrammar, prod.rhs[0], visited);
                result.insert(first_of_nonterm.begin(), first_of_nonterm.end());
            }
        }
    }

    visited.erase(non_terminal);
    return result;
}

// обертка для вызова рекурсивной функции first
std::set<std::string> first(const std::vector<Production>& extendedGrammar, std::string non_terminal)
{
    std::set<std::string> visited;
    return first(extendedGrammar, non_terminal, visited);
}


// функция follow, которая возвращает множество терминалов, которые могут следовать непосредственно за non_terminal в процессе вывода 
std::set<std::string> follow(const std::vector<Production>& extendedGrammar, std::string non_terminal)
{
    std::set<std::string> result;

    if (non_terminal == extendedGrammar[0].lhs)
    {
        result = {"$"};
        return result;
    }

    else
    {
        ArithmeticGrammar full_grammar;
        std::set<std::string> non_terminals = full_grammar.getNonTerminals();

        bool changed = true;

        while(changed)
        {
            changed = false;

            for (const Production& prod : extendedGrammar)
            {
                if (prod.lhs == non_terminal)
                {
                    for (const std::string elem : prod.rhs)
                    {

                        if (non_terminals.find(elem) != non_terminals.end())
                        {

                        }
                    }
                }
            }
        }
    }
}
#endif