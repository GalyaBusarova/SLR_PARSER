%option noyywrap

%{
#include "token_types.h"  
%}

%%

[ \t\n\r]    { /* игнорируем пробелы */ }

[a-zA-Z_][a-zA-Z0-9_]*  { return TOK_ID; }

[0-9]+       { return TOK_NUM; }

"+"          { return TOK_PLUS; }
"-"          { return TOK_MINUS; }
"*"          { return TOK_MUL; }
"/"          { return TOK_DIV; }
"("          { return TOK_LP; }
")"          { return TOK_RP; }

.            { return TOK_ERROR; }

%%