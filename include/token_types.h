// token_types.h
#pragma once

// Только перечисление — без зависимостей от C++
enum TokenId {
    TOK_NUM = 0,
    TOK_ID = 1,
    TOK_PLUS = 2,
    TOK_MINUS = 3,
    TOK_MUL = 4,
    TOK_DIV = 5,
    TOK_LP = 6,
    TOK_RP = 7,
    TOK_END = 8,
    TOK_ERROR = 9
};