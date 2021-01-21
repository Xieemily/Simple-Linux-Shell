%{
#include <stdio.h>
#include "shell.hh"
#include "command.hh"

 void yyerror(char *s);
 int yylex(void);

%}

%token	<string_val> WORD
%token NOTOKEN GREAT NEWLINE GREATGREAT PIPE AMPERSAND GREATAMPERSAND GREATGREATAMPERSAND LESS
%union{
	char   *string_val;
    }
%locations

%%
goal: 
    command_list;

command_list:  
    command_line 
    | command_list command_line 
;/* command loop*/ 

command_line: 
    pipe_list io_modifier_list background_optional NEWLINE {
        // printf("execute\n");
        Shell::_shellCommand.execute();
    }
    | NEWLINE /*accept empty cmd line*/ 
    // {printf("bison empty line\n");}
    | error NEWLINE
    // {printf("error\n");} 
    /*error recovery*/ 
;
pipe_list: 
    pipe_list PIPE cmd_and_args 
    // {printf("pipe list\n");}
    | cmd_and_args 
    // { printf("no pipe\n");}
;
cmd_and_args: 
    WORD{
        // printf("cmd word\n");
        Command::_currentSimpleCommand = new SimpleCommand();
        Command::_currentSimpleCommand->insertArgument($1);
    } arg_list {
        // printf("cmd_simple\n");
        Shell::_shellCommand.insertSimpleCommand(Command::_currentSimpleCommand);
    }
;

arg_list: 
    arg_list WORD{
        // printf("arg word\n");
        Command::_currentSimpleCommand->insertArgument($2);
    }
    | /*empty*/ // { printf("empty arg\n");}
;

io_modifier: 
    GREATGREAT WORD{
        // append
        Shell::_shellCommand._outFile = $2;
        Shell::_shellCommand._append = 1;
    }
    | GREAT WORD{
        // redirect output
        Shell::_shellCommand._outFile = $2;
        Shell::_shellCommand._append = 0;
    }
    | GREATGREATAMPERSAND WORD{
        // outfile and error file
        Shell::_shellCommand._outFile = $2;
        Shell::_shellCommand._errFile = $2;
        Shell::_shellCommand._append = 1;
    }
    | GREATAMPERSAND WORD{
        Shell::_shellCommand._outFile = $2;
        Shell::_shellCommand._errFile = $2;
        Shell::_shellCommand._append = 0;
    }
    | LESS WORD{
        // redirect input
        Shell::_shellCommand._inputFile = $2;
    }
;
io_modifier_list: 
    io_modifier_list io_modifier 
    | /*empty*/ 
    // {printf("no io_modifier\n");}
;

background_optional:  
    AMPERSAND {
        Shell::_shellCommand._background = true;
    }
    | /*empty*/ 
    // {printf("no bg\n");}
;



%%

void yyerror(char *s)
{
  extern int yylineno;  // defined and maintained in lex
  extern char *yytext;  // defined and maintained in lex
  fprintf(stderr, "ERROR: %s at symbol '%s' on line %d\n", s,
          yytext, yylineno);
}
