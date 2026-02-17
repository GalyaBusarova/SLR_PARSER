#pragma once

#include <map>
#include <string>
#include <set>

// терминалы грамматики
enum Terminals
{
    NUM = 0,
    ID = 1,
    PLUS = 2,
    MINUS = 3,
    MUL = 4,
    DIV = 5,
    LP = 6,
    RP = 7,
    NL = 8,
    TERM_COUNT
};

inline std::map<std::string, enum Terminals> terminals_table
{
    {"NUMBER", NUM},
    {"ID", ID},
    {"+", PLUS},
    {"-", MINUS},
    {"*", MUL},
    {"/", DIV},
    {"(", LP},
    {")", RP},
    {"$", NL}
};

// функция first для нахождения множества терминалов, с которых могут начинаться цепочки, выводимые из нетерминала 
std::map<std::string, std::set<enum Terminals>> getFirstSets();

// функция follow, которая возвращает множество терминалов, которые могут следовать непосредственно за non_terminal в процессе вывода 
std::map<std::string, std::set<enum Terminals>> getFollowSets();
