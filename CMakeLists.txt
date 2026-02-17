cmake_minimum_required(VERSION 3.10)
project(SLR_Parser LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(FLEX REQUIRED)

# Добавляем директории с заголовками
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)  # для лексера

# Список всех .cpp файлов
set(SOURCE_FILES
    src/main.cpp
    src/lr0.cpp
    src/follow.cpp
    src/token.cpp
    src/lexer.cpp
    src/slr.cpp
)

# Генерация лексера
FLEX_TARGET(Lexer src/lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.c)
set(LEXER_SOURCE ${FLEX_Lexer_OUTPUTS})

# Создание исполняемого файла
add_executable(slr_parser ${SOURCE_FILES} ${LEXER_SOURCE})

if(APPLE)
    target_compile_options(slr_parser PRIVATE -Wno-unused-function -Wno-unused-parameter)
endif()