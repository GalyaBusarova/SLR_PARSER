cmake_minimum_required(VERSION 3.10)
project(SLR_Parser LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(FLEX REQUIRED)

# ════════════════════════════════════════════════════════════
# Добавляем директории с заголовками
# ════════════════════════════════════════════════════════════
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)

# ════════════════════════════════════════════════════════════
# Генерация лексера
# ════════════════════════════════════════════════════════════
FLEX_TARGET(Lexer src/lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.c)
set(LEXER_SOURCE ${FLEX_Lexer_OUTPUTS})

# ════════════════════════════════════════════════════════════
# ИСТОЧНИКИ БИБЛИОТЕКИ: все .cpp файлы + сгенерированный лексер
# ════════════════════════════════════════════════════════════
set(LIBRARY_SOURCES
    src/parser.cpp
    src/lr0.cpp
    src/follow.cpp
    src/token.cpp
    src/lexer.cpp
    src/slr.cpp
    ${LEXER_SOURCE}  # ← КРИТИЧЕСКИ ВАЖНО: добавить сгенерированный файл!
)

# ════════════════════════════════════════════════════════════
# Создаём статическую библиотеку
# ════════════════════════════════════════════════════════════
add_library(slr_lib STATIC ${LIBRARY_SOURCES})

# ════════════════════════════════════════════════════════════
# Исполняемые файлы
# ════════════════════════════════════════════════════════════
add_executable(slr_parser src/main.cpp)
target_link_libraries(slr_parser slr_lib)

add_executable(run_tests tests/test_parser.cpp)
target_link_libraries(run_tests slr_lib)

# ════════════════════════════════════════════════════════════
# Опции компилятора
# ════════════════════════════════════════════════════════════
if(APPLE)
    target_compile_options(slr_parser PRIVATE -Wno-unused-function -Wno-unused-parameter)
    target_compile_options(run_tests PRIVATE -Wno-unused-function -Wno-unused-parameter)
endif()