#pragma once

#include <string>
#include <vector>
#include <map>

#include "grammar.h"
#include "table.h"
#include "token.h"

// функция, переводящая правило в строку
std::string RuleToString(const Production& prod);

// функция, переводящая стек строк в строку
std::string StackToString(const std::vector<std::string>& str_stack);

// функция, переводящая входную последовательность токенов в строку
std::string InputToString(const std::vector<enum TokenId>& input_tokens);

// основная функция парсинга
bool parser(
    const std::vector<TokenId>& tokens,
    const std::vector<std::vector<Action>>& action_table,
    const std::vector<std::map<std::string, int>>& goto_table,
    const std::vector<Production>& ExtendedGrammar);