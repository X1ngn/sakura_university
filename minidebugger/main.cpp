//
//  main.cpp
//  debuger
//
//  Created by X1ng on 2020/10/5.
//  Copyright © 2020 X1ng. All rights reserved.
//


#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>


#include <unistd.h>
#include <stdio.h>

#include <utility>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/wait.h>

using namespace std;

class BreakPoint{
public:
    BreakPoint(pid_t pid, intptr_t addr)//intptr_t类型始终与地址位数相同
    : m_pid(pid), m_addr(addr)
    {}
    BreakPoint()
    {}

    void enable();
    void disable();
    inline bool is_able() {return m_abled;}
    inline uint64_t get_address() {return m_addr;}
private:
    pid_t m_pid;
    intptr_t m_addr;
    bool m_abled;
    uint64_t m_saved_data;
};

void BreakPoint::enable()
{
    m_saved_data = ptrace(PTRACE_PEEKDATA, m_pid, m_addr, nullptr);
    uint64_t data_with_int_3 = ((m_saved_data & ~0xff) | 0xcc);
    ptrace(PTRACE_POKEDATA, m_pid, m_addr, data_with_int_3);
    m_abled = true;
}

void BreakPoint::disable()
{
    ptrace(PTRACE_POKEDATA, m_pid, m_addr, m_saved_data);
    m_abled = false;
}



class Debugger{
public:
    Debugger (string prog_name, pid_t pid)
    : m_prog_name{move(prog_name)}, m_pid{pid}//move将一个左值强制转化为右值引用,等同于一个类型转换
    {}
    void run();
    void handle_command(const string &);
    void continue_execution();
    void set_breakpoint(intptr_t addr);
    //void show_register();
private:
    string m_prog_name;
    pid_t m_pid;
    unordered_map<intptr_t, BreakPoint> m_breakpoints;
};


void Debugger::continue_execution()
{
    ptrace(PTRACE_CONT, m_pid, nullptr, nullptr);
    //继续执行目标进程
    int wait_status;
    auto options = 0;
    waitpid(m_pid, &wait_status, options);//用waitpid回收子进程
}

void Debugger::set_breakpoint(intptr_t addr)
{
    std::cout << "Set BreakPoint at address 0x" << hex << addr << endl;
    BreakPoint bp(m_pid, addr);
    bp.enable();
    m_breakpoints[addr] = bp;
}


vector<string> split(const string &s, char delimiter)
{
    vector<string> out{};
    stringstream ss{s};
    string item;
    while (getline(ss, item, delimiter)) {
        out.push_back(item);
    }
    return out;
}

bool is_prefix(const string &s, const string &of)
{
    return s.size() > of.size() ? false : equal(s.begin(), s.end(), of.begin());
}

void Debugger::handle_command(const string &line)
{
    auto args = split(line, ' ');//将命令各部分分开
    auto command = args[0];
    if(is_prefix(command, "continue"))//
        continue_execution();
    else if(is_prefix(command, "break")){
        if(args[1][1]=='x'){//判断用户输入的地址是十六进制还是十进制
            string addr (args[1], 2);
            set_breakpoint(std::stol(addr, 0, 16));
        }
        else{
            string addr (args[1]);
            set_breakpoint(std::stol(addr, 0, 10));
        }
    }
    else if(is_prefix(command, "quit")){
        kill(m_pid, SIGKILL);
        fflush(stdout);
        exit(0);
    }
    else
        cerr << "Unknow command\n";
}

void Debugger::run()
{
    int wait_status;
    auto options = 0;
    waitpid(m_pid, &wait_status, options);
    string line;
    while (true) {
        cout << "mgdb > ";
        getline(cin, line);
        if(line.empty())
            continue;
        else{
            handle_command(line);
        }
    }
}


int main(int argc, const char * argv[]) {
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
