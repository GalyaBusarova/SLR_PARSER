#pragma once

#include <set>
#include "grammar.h"

// структура элемента
struct lr0_item
{
    int rule_id;
    int dot_position; // позиция точки в правиле

    // для использования std::set
    bool operator<(const lr0_item& other) const 
    {
        if (rule_id != other.rule_id) 
            return rule_id < other.rule_id;
        return dot_position < other.dot_position;
    }

    bool operator==(const lr0_item& other) const {
        return rule_id == other.rule_id && dot_position == other.dot_position;
    }
};

// структуры состояния 
struct lr0_state
{
    int id;
    std::set<lr0_item> items; 
};

// функция closure, необходимая для построения LR(0)-автомата
std::set<lr0_item> closure(const std::set<lr0_item>& items, const std::vector<Production>& grammar);

// функция goto, необходимая для построения LR(0)-автомата
std::set<lr0_item> GoTo(const std::set<lr0_item>& situation_set, const std::string& symbol, const std::vector<Production>& grammar);

// функция построения LR(0)-автомата
std::vector<lr0_state> buildLR0Automaton(const std::vector<Production>& ExtendedGrammar);

// функция для получения всех символов после точек в состоянии cur_state
std::set<std::string> GetSymbolsAfterDot(const lr0_state& cur_state, const std::vector<Production>& Grammar);





