#pragma once

#include <vector>
#include <string>
#include <set>

// структура продукции языка
struct Production {
    std::string lhs;               // нетерминал слева 
    std::vector<std::string> rhs;  // символы справа 
    int id;                        // номер правила 
};

// класс грамматики языка
class ArithmeticGrammar 
{
public:
    // возвращает все правила грамматики
    std::vector<Production> getProductions() const
    {
        return grammar;
    }

    // возвращает множество терминалов
    std::set<std::string> getTerminals() const
    {
        std::set<std::string> terminals;
        std::set<std::string> nonterminals = getNonTerminals();

        for (size_t i = 0; i < grammar.size(); i++)
        {
            for (size_t j = 0; j < grammar[i].rhs.size(); j++)
            {
                if(nonterminals.find(grammar[i].rhs[j]) == nonterminals.end())
                {
                    terminals.insert(grammar[i].rhs[j]);
                }
            }
        }

        terminals.insert("$");

        return terminals;
    }

    // возвращает множество нетерминалов
    std::set<std::string> getNonTerminals() const
    {
        std::set<std::string> non_terminals;

        for (size_t i = 0; i < grammar.size(); i++)
        {
            non_terminals.insert(grammar[i].lhs);
        }

        return non_terminals;
    }

    // возвращает стартовый символ
    std::string getStartSymbol() const
    {
        return grammar[0].lhs;
    }

private:
    // грамматика языка
    std::vector<Production> grammar {
        {"E", {"E", "+", "T"}, 0},
        {"E", {"E", "-", "T"}, 1},
        {"E", {"T"},           2},
        {"T", {"T", "*", "F"}, 3},
        {"T", {"T", "/", "F"}, 4},
        {"T", {"F"},           5},
        {"F", {"(", "E", ")"}, 6},
        {"F", {"ID"},          7},
        {"F", {"NUMBER"},      8}
    };

};



