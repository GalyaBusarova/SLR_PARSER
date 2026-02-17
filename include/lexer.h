#pragma once

#include <vector>
#include <string>

#include "token.h"

// функция, которая нашу входную строку a + b * c переводит в поток токенов TOK_ID, TOK_PLUS, TOK_ID, TOK_MUL, TOK_ID, TOK_END
std::vector<TokenId> tokenize(const std::string& input);