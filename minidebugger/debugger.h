#include "main.h"


#ifndef DEBUGGER
#define DEBUGGER

class Debugger{
public:
    Debugger (string prog_name, pid_t pid)
    : m_prog_name{move(prog_name)}, m_pid{pid}//move将一个左值强制转化为右值引用,等同于一个类型转换
    {}
    void run();
    void show_register();
    void handle_command(const string &);
    void continue_execution();
    void set_breakpoint(intptr_t addr);
    void show_memory(intptr_t addr);
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
    cout << "Set BreakPoint at address 0x" << hex << addr << endl;
    BreakPoint bp(m_pid, addr);
    bp.enable();
    m_breakpoints[addr] = bp;
}

void Debugger::show_memory(intptr_t addr)
{
    
    uint64_t data;
    
    for(intptr_t i = 0; i < 20; i++){
        data = ptrace(PTRACE_PEEKDATA, m_pid, addr + i*8, nullptr);
        cout << "0x" << hex << addr + i*8 << ':';
        cout << "0x" << hex << data << '\n';
    }

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

bool flag_show = true;

void Debugger::handle_command(const string &line)
{
    auto args = split(line, ' ');//将命令各部分分开
    auto command = args[0];
    if(is_prefix(command, "continue"))//
        continue_execution();
    else if(is_prefix(command, "break")){
        if(args[1][1]=='x'){//判断用户输入的地址是十六进制还是十进制
            string addr (args[1], 2);
            set_breakpoint(stol(addr, 0, 16));
        }
        else{
            string addr (args[1]);
            set_breakpoint(stol(addr, 0, 10));
        }
    }
    else if(is_prefix(command, "x")){
        flag_show = false;
        if(args[1][1]=='x'){//判断用户输入的地址是十六进制还是十进制
            string addr (args[1], 2);
            show_memory(stol(addr, 0, 16));
        }
        else{
            string addr (args[1]);
            show_memory(stol(addr, 0, 10));
        }       
    }
    else if(is_prefix(command, "quit")){
        kill(m_pid, SIGKILL);
        fflush(stdout);
        exit(0);
    }
    else{
        flag_show = false;
        cerr << "Unknow command\n";
    }
}


void Debugger::show_register()
{
    struct user_regs_struct regs;
    ptrace(PTRACE_GETREGS, m_pid, nullptr, & regs);
    //通过PTRACE_GETREGS参数获取了所有的寄存器值。结构体user_regs_struct定义在sys/user.h中
    cout << "------------------------------------\n";
    cout << "rax:" << regs.rax << '\n';
    cout << "rbi:" << regs.rbx << '\n';
    cout << "rcx:" << regs.rcx << '\n';
    cout << "rdx:" << regs.rdx << '\n';
    cout << "rdi:" << regs.rdi << '\n';
    cout << "rsi:" << regs.rsi << '\n';
    cout << "r8:" << regs.r8 << '\n';
    cout << "r9:" << regs.r9 << '\n';
    cout << "r10:" << regs.r10 << '\n';
    cout << "r11:" << regs.r11 << '\n';
    cout << "r12:" << regs.r12 << '\n';
    cout << "r13:" << regs.r13 << '\n';
    cout << "r14:" << regs.r14 << '\n';
    cout << "r15:" << regs.r15 << '\n';
    cout << "rbp:" << regs.rbp << '\n';
    cout << "rsp:" << regs.rsp << '\n';
    cout << "rip:" << regs.rip << '\n';
    cout << "------------------------------------\n";
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
            if(flag_show == true)
                show_register();
            else
                flag_show = true;
        }
    }
}


#endif
