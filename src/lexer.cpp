
#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>

#include "lexer.h"
#include "token_types.h"

// эти функции сгенерирует Flex
extern "C" {
    int yylex();
    void yy_scan_string(const char*);
    void yy_delete_buffer(void*);
}



std::vector<TokenId> tokenize(const std::string& input) {
    std::vector<TokenId> tokens;
    yy_scan_string(input.c_str());
    
    int token;
    while ((token = yylex()) != 0) {  
        if (token == 9) {  // TOK_ERROR
            throw std::runtime_error("Lexical error: invalid character");
        }

        // Лексер: 1→8, Парсер: 0→7
        TokenId tok_id = static_cast<TokenId>(token - 1);
        tokens.push_back(tok_id);
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
