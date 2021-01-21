#ifndef command_hh
#define command_hh
#define MAXN 270

#include <iostream>
// Command Data Structure 
// Describes a simple command and arguments 

struct SimpleCommand { 
        // Available space for arguments currently preallocated 
        int _numberOfAvailableArguments; 
        // Number of arguments
        int _numberOfArguments;
        // Array of arguments 
        char * _arguments[MAXN];
        SimpleCommand();
        void insertArgument( char * argument );
};
// Describes a complete command with the multiple pipes if any 
// and input/output redirection if any. 
struct Command { 
        int _numberOfAvailableSimpleCommands;
        int _numSimpleCmds;
        SimpleCommand * _simpleCmds[MAXN];
        char * _outFile;
        char * _inputFile;
        char * _errFile;
        int _append;
        int _background;
        void prompt();
        void print();
        void execute();
        void clear();
        Command();
        void insertSimpleCommand( SimpleCommand * simpleCommand );
        static Command _shellCommand;
        static SimpleCommand *_currentSimpleCommand;
};

#endif