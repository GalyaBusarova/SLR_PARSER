#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cassert>

#include "grammar.h"
#include "lr0.h"
#include "lexer.h"
#include "parser.h"

int total_tests = 0;
int passed_tests = 0;
int failed_tests = 0;


// Функция для запуска одного теста
bool run_test(const std::string& expr, bool expected, const std::string& test_name = "") 
{
    total_tests++;
    
    std::cout << "Test " << total_tests << ": ";
    if (!test_name.empty()) {
        std::cout << test_name << " ";
    }
    std::cout << "\"" << expr << "\" ... \n";
    
    try {
        ArithmeticGrammar G;
        std::vector<Production> ExtendedGrammar;
        
        ExtendedGrammar.push_back({"S'", {"E"}, 0});
        auto baseGrammar = G.getProductions();

        for (size_t i = 0; i < baseGrammar.size(); i++) 
        {
            Production p = baseGrammar[i];
            p.id = static_cast<int>(i + 1);
            ExtendedGrammar.push_back(p);
        }
        
        auto lr0_automation = buildLR0Automaton(ExtendedGrammar);
        auto action_table = make_action_table(lr0_automation, ExtendedGrammar);
        auto goto_table = make_goto_table(lr0_automation, ExtendedGrammar);
        
        std::vector<TokenId> tokens = tokenize(expr);

        // 🔑 ОТЛАДКА: вывод токенов
        std::cout << "\n  Tokens (" << tokens.size() << "): ";
        for (TokenId t : tokens) {
            std::cout << tokenToString(t) << " ";
        }
        std::cout << "\n  ";
        
        bool result = parser(tokens, action_table, goto_table, ExtendedGrammar);
        
        if (result == expected) {
            std::cout << "\033[32m✓ PASS\033[0m\n";  
            passed_tests++;
            return true;
        } else {
            std::cout << "\033[31m✗ FAIL\033[0m (expected " 
                      << (expected ? "success" : "error") 
                      << ", got " << (result ? "success" : "error") << ")\n";  
            failed_tests++;
            return false;
        }
    } catch (const std::exception& e) {
        std::cout << "\033[31m✗ EXCEPTION\033[0m: " << e.what() << "\n";
        failed_tests++;
        return false;
    }
}


void test_valid_expressions() {
    std::cout << "\n=== Valid Expressions ===\n\n";
    
    run_test("a", true, "Simple variable");
    run_test("a+b", true, "Addition");
    run_test("a+b*c", true, "Operator precedence");
    run_test("(a+b)*c", true, "Parentheses");
    run_test("a+b-c*d/e", true, "Mixed operators");
    run_test("((a+b)*c)", true, "Nested parentheses");
    run_test("a*b+c*d", true, "Multiple multiplications");
    run_test("a+b+c", true, "Left associativity +");
    run_test("a*b*c", true, "Left associativity *");
    run_test("(a+b)*(c+d)", true, "Multiple parentheses");
    run_test("a+(b*c)", true, "Parentheses with precedence");
    run_test("123", true, "Number literal");
    run_test("x1+y2*z3", true, "Multiple variables");
    run_test("a-b+c", true, "Mixed + and -");
    run_test("a*b/c", true, "Mixed * and /");
}


void test_invalid_expressions() {
    std::cout << "\n=== Invalid Expressions ===\n\n";
    
    run_test("a+", false, "Incomplete expression");
    run_test("(a+b", false, "Unclosed parenthesis");
    run_test("+", false, "Single operator");
    run_test("a b", false, "Missing operator");
    run_test("a++b", false, "Double operator");
    run_test("a*b+", false, "Operator at end");
    run_test("(a+b))", false, "Extra closing parenthesis");
    run_test(")", false, "Single closing parenthesis");
    run_test("(", false, "Single opening parenthesis");
    run_test("+a", false, "Operator at start");
    run_test("a+", false, "Operator at end");
    run_test("a b c", false, "Multiple missing operators");
}



int main() {
    std::cout << "\033[1;36m================================\033[0m\n";
    std::cout << "\033[1;36m=== SLR Parser Test Suite ===\033[0m\n";
    std::cout << "\033[1;36m================================\033[0m\n\n";
    
    test_valid_expressions();
    test_invalid_expressions();
    
    std::cout << "\n\033[1;36m================================\033[0m\n";
    std::cout << "\033[1;36m=== Test Results ===\033[0m\n";
    std::cout << "\033[1;36m================================\033[0m\n";
    std::cout << "Total tests: " << total_tests << "\n";
    std::cout << "\033[32mPassed: " << passed_tests << "\033[0m\n";
    std::cout << "\033[31mFailed: " << failed_tests << "\033[0m\n";
    std::cout << "\033[1;36m================================\033[0m\n\n";
    
    if (failed_tests == 0) {
        std::cout << "\033[1;32m✓ All tests passed!\033[0m\n\n";
        return 0;
    } else {
        std::cout << "\033[1;31m✗ Some tests failed!\033[0m\n\n";
        return 1;
    }
}