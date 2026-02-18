%option noyywrap

%%

[ \t\n\r]+   { /* игнорируем пробелы */ }

[0-9]+       { return 1; }  // NUMBER → 1 (НЕ 0!)

[a-zA-Z_][a-zA-Z0-9_]*  { return 2; }  // ID → 2

"+"          { return 3; }  // PLUS → 3
"-"          { return 4; }  // MINUS → 4
"*"          { return 5; }  // MUL → 5
"/"          { return 6; }  // DIV → 6
"("          { return 7; }  // LP → 7
")"          { return 8; }  // RP → 8

.            { return 9; }  // ERROR → 9

%%