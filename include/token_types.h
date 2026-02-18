#pragma once

// Синхронизируем с enum Terminals (0..8)
enum TokenId {
    TOK_NUM = 0,    // NUMBER → 0
    TOK_ID = 1,     // ID → 1
    TOK_PLUS = 2,   // + → 2
    TOK_MINUS = 3,  // - → 3
    TOK_MUL = 4,    // * → 4
    TOK_DIV = 5,    // / → 5
    TOK_LP = 6,     // ( → 6
    TOK_RP = 7,     // ) → 7
    TOK_END = 8,    // $ → 8
    TOK_ERROR = 9
};