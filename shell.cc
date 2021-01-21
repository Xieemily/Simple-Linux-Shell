#include <cstdio>
#include <unistd.h>
#include "shell.hh"

int yyparse(void);

void Shell::prompt() {
  char pwd [1024], user[1024];
  size_t siz = 1024;
  getcwd(pwd, siz);
  getlogin_r(user, siz);
  printf("%s@myshell:%s$ ", user, pwd); 
  fflush(stdout);
}

int main() {
  Shell::prompt();
  yyparse();
}

Command Shell::_shellCommand;
SimpleCommand* Command::_currentSimpleCommand;
