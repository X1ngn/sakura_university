//
//  main.cpp
//  debuger
//
//  Created by X1ng on 2020/10/5.
//  Copyright © 2020 X1ng. All rights reserved.
//


#include "main.h"
#include "breakpoint.h"
#include "debugger.h"

using namespace std;


int main(int argc, const char * argv[]){
    if(argc < 2){
        cerr << "Input fine name!\n";
        return -1;
    }
    string prog = argv[1];
    auto pid = fork();
    if(pid == 0){
        //在子进程中
        ptrace(PTRACE_TRACEME, 0, nullptr, nullptr);//在子进程中表示允许父进程跟踪自己
        execl(prog.c_str(), prog.c_str(), nullptr);
    }
    else if(pid > 0){
        //在父进程中
        Debugger dbg{prog, pid};
        dbg.run();
    }
    return 0;
}
