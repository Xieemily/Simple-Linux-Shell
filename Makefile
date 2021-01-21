
#Use GNU compiler
cc= gcc
CC= g++
ccFLAGS= -g -std=c11
CCFLAGS= -g -std=c++17
WARNFLAGS= -Wall -Wextra -pedantic

LEX=lex -l
YACC=yacc -y -d -t --debug

EDIT_MODE_ON=

ifdef EDIT_MODE_ON
	EDIT_MODE_OBJECTS=tty-raw-mode.o read-line.o
endif

all: shell

lex.yy.o: shell.l 
	$(LEX) -o lex.yy.cc shell.l
	$(CC) $(CCFLAGS) -c lex.yy.cc

y.tab.o: shell.y
	$(YACC) -o y.tab.cc shell.y
	$(CC) $(CCFLAGS) -c y.tab.cc

command.o: command.cc command.hh
	$(CC) $(CCFLAGS) $(WARNFLAGS) -c command.cc

shell.o: shell.cc shell.hh
	$(CC) $(CCFLAGS) $(WARNFLAGS) -c shell.cc

shell: y.tab.o lex.yy.o shell.o command.o $(EDIT_MODE_OBJECTS)
		$(CC) $(CCFLAGS) $(WARNFLAGS) -o shell lex.yy.o y.tab.o shell.o command.o $(EDIT_MODE_OBJECTS) -static -lfl


.PHONY: clean
clean:
	rm -f lex.yy.cc y.tab.cc y.tab.hh shell *.o

