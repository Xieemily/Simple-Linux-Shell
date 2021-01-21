#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <pwd.h>
#include <libgen.h>
#include <sys/types.h>
#include <cstring>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "command.hh"
#include "shell.hh"

Command::Command(){
    _numSimpleCmds = 0;
    _outFile = NULL;
    _inputFile = NULL;
    _errFile = NULL;
    _background = false;
    _append = 0;
}

void Command::insertSimpleCommand( SimpleCommand * simpleCmd ){
    _simpleCmds[_numSimpleCmds++] = simpleCmd;
    Command::_currentSimpleCommand = simpleCmd;
    // printf("insert cmd finish\n");
}

void Command::execute() 
{ 
    // input/output file discriptor
    int tmpin = dup(0);
    int tmpout = dup(1);
    int tmperr = dup(2);

    // open input file
    int fdin, fdout, fderr;
    if(_inputFile != NULL){
        fdin = open(_inputFile, O_RDONLY);
        if(fdin < 0){perror("input file:"); return;}
    }
    else fdin = dup(tmpin);
    // error file
    if(_errFile != NULL){
        fderr = open(_errFile, O_CREAT|O_RDWR);
        if(fdin < 0){perror("error file:"); return;}
    }
    else fderr = dup(tmperr);
    

    // iterate through commands
    int ret;
    for( int i = 0; i < _numSimpleCmds; i++ ) { 
        
        // builtin commands
        if(!strcmp(_simpleCmds[i]->_arguments[0], "exit")){
            exit(0);
        }
        if(!strcmp(_simpleCmds[i]->_arguments[0], "cd")){
            chdir(_simpleCmds[i]->_arguments[1]);
            continue;
        }

        // redirect input/error
        dup2(fdin, 0);
        close(fdin);
        dup2(fderr, 2);
        close(fderr);

        // redirect
        if(i == _numSimpleCmds-1){
            // last command
            // output file
            if(_outFile != NULL){
                if(_append) fdout = open(_outFile, O_APPEND|O_RDWR);
                else fdout = open(_outFile, O_CREAT | O_RDWR);
                if(fdin < 0){perror("output file:"); return;}
            }
            else{
                // default output
                fdout = dup(tmpout);
            }

            
        }
        else{
            // not last command, create pipe
            int fdpipe[2];
            pipe(fdpipe);
            fdin = fdpipe[0];
            fdout = fdpipe[1];
        }

        dup2(fdout,1);
        close(fdout);

        // create process for commands
        ret = fork();
        if (ret == 0) { 
            //child 
            execvp(_simpleCmds[i]->_arguments[0], _simpleCmds[i]->_arguments); 
            perror("execvp"); 
            _exit(1); 
        } 
        else if (ret < 0) { 
            perror("fork"); 
            return; 
        } 
    // Parent shell continue 
    } // for 

    dup2(tmpin, 0);
    dup2(tmpout, 1);
    dup2(tmperr, 2);
    close(tmpin);
    close(tmpout);
    close(tmperr);

    if(!_background) { 
        // wait for last process 
        waitpid(ret, NULL, 0); 
    } 

    // Command::print();
    Command::clear();
    Shell::prompt();
    
}// execute 

void Command::clear(){
    for( int i = 0; i < _numSimpleCmds; i++ ) { 
        _simpleCmds[i]->_numberOfArguments = 0;
        delete _simpleCmds[i];
    } 
    _numSimpleCmds = 0;
    _outFile = NULL;
    _inputFile = NULL;
    _errFile = NULL;
    _append = 0;
}

void Command::print(){
    printf("\n");
    int cnt = 0;
    for(int i = 0; i < _numSimpleCmds; i++){
        while(_simpleCmds[i]->_arguments[cnt]!=NULL){
            printf("%s\t", _simpleCmds[i]->_arguments[cnt++]);
        }
        printf("\n");
    }
    
}

SimpleCommand::SimpleCommand(){
    int _numberOfArguments = 0;
}


void SimpleCommand::insertArgument( char * argument ){
    _numberOfArguments++;
    // printf("num:%d\n", _numberOfArguments);
    _arguments[_numberOfArguments-1] = argument;
    _arguments[_numberOfArguments] = NULL;
    // printf("finish insert arg %s\n", argument);
}
