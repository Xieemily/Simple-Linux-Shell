/* 
 * shell.l: simple lexical analyzer for the shell. 
 */

%{ 
#include <string.h> 
#include <stdlib.h>
#include <stdio.h>
#include "y.tab.h"
%} 

%option yylineno

%% 

\n      { 
                // printf("lex: newline\n");
                return NEWLINE;
} 
[ \t]   { 
                // printf("lex: tab\n");
                /* Discard spaces and tabs */ 
        } 
">"     { 
                // printf("lex: >\n");
                return GREAT;
        } 
"<"     { 
                // printf("lex: <\n");
                return LESS;
        } 
">>"     { 
                // printf("lex: >>\n");
                return GREATGREAT;
        } 
">&"     { 
                // printf("lex: >&\n");
                return GREATAMPERSAND;
        } 
"|"    { 
                // printf("lex: pipe!\n");
                return PIPE;
        } 
"&"    { 
                return AMPERSAND;
        } 
[a-zA-Z_\-\.][a-zA-Z0-9_\.]*  { 
                /* Pass to parser */ 
                // printf("word:%s\n", yytext);
                yylval.string_val = strdup(yytext);
                return WORD;
        } 
.       {
                // printf("not matched\n");
        }

%%


