#include "main.h"

using namespace std;

#ifndef BREAKPOINT
#define BREAKPOINT

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

#endif
