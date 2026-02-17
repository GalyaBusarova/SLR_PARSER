#include <string>

#include "token.h"

std::string tokenToString(TokenId token) {
    switch (token) {
        case TOK_NUM:   return "NUMBER";  
        case TOK_ID:    return "ID";
        case TOK_PLUS:  return "+";
        case TOK_MINUS: return "-";
        case TOK_MUL:   return "*";
        case TOK_DIV:   return "/";
        case TOK_LP:    return "(";
        case TOK_RP:    return ")";
        case TOK_END:   return "$";
        case TOK_ERROR: return "ERROR";
        default:        return "?";
    }
}