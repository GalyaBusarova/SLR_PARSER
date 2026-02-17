
#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>

#include "lexer.h"

// эти функции сгенерирует Flex
extern "C" {
    int yylex();
    void yy_scan_string(const char*);
    void yy_delete_buffer(void*);
}

// функция, которая нашу входную строку a + b * c переводит в поток токенов TOK_ID, TOK_PLUS, TOK_ID, TOK_MUL, TOK_ID, TOK_END
std::vector<TokenId> tokenize(const std::string& input) 
{
    std::vector<TokenId> tokens;

    yy_scan_string(input.c_str());
    
    int token;
    while ((token = yylex()) != 0) 
    { 
        if (token == TOK_ERROR) 
        {
            throw std::runtime_error("Lexical error: invalid character in input");
        }

        tokens.push_back(static_cast<TokenId>(token));
    }
    
    tokens.push_back(TOK_END);
    
    return tokens;
}


// проверка работы функции
#if 0
int main() 
{
    try {
        auto tokens = tokenize("a + 42 * (b - 1)");
        std::cout << "Tokens:\n";
        for (auto t : tokens) {
            // Добавьте функцию tokenToString() для отладки
            std::cout << tokenToString(t) << " ";
        }
        // Ожидаемо: ID PLUS NUM MUL LP ID MINUS NUM RP END
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}
#endif
