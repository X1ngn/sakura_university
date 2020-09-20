## 知识点

### 进程

进程 = 程序 + 执行状态

进程是程序的执行

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1ghrrgcytbpj317u0myn80.jpg)

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1ghrrizd5u6j317y0nc177.jpg)

ucore中进程的生命划分为 进程创建、进程执行、进程等待、进程抢占、进程唤醒、进程结束

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1ghsb6zmtm9j30wu0msabn.jpg)

等待->等待挂起状态：没有进程处于就绪状态或就绪进程要求更多内存资源，**进程在外存并等待某个时间出现**

就绪->就绪挂起状态：当有高优先级等待进程和低优先级就绪进程时，**进程在外存，但只要进入内存就可运行**

运行->就绪挂起状态：对抢先式分时系统，当有高优先级等待挂起进程因事件出现而进入就绪挂起，而没有足够空间，则当前运行进程进入就绪挂起状态

由操作系统维护的一组队列，根据进程状态不同，将进程控制块（PCB）加入相应的状态队列中，以表示不同状态

### 线程

>在进程内部增加一类实体，满足以下特性：
>
>- 实体之间可以并发执行
>- 实体之间共享相同的地址空间
>
>这种实体就是线程

线程是进程中指令执行流的最小单元，是cpu调度的基本单位

每个线程有独立堆栈，pc指针和寄存器，而代码、数据和打开文件是一个进程中所有线程共享的

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1ghsovc9vqxj31820n0wzi.jpg)

#### 用户线程

由一组用户级的线程库函数来完成线程的管理，包括线程的创建、终止、同步和调度等

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1ghsp3m2e4uj31800n4ne0.jpg)

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1ghsp3qbwcsj317y0my167.jpg)

#### 内核线程

进程由内核通过系统调用实现线程机制，由内核完成线程的创建、终止和管理

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1ghsp8scplaj31820mw7hx.jpg)

用户线程与内核线程形成一对一的对应关系

### 进程切换（上下文切换）

暂停当前运行进程，从运行状态转变为其他状态

调度另一个进程从就绪状态变成运行状态

#### 进程切换要求

- 切换前保存进程上下文在进程控制块里

- 切换后恢复进程上下文从进程控制块里

- 快速切换（一般保存恢复都通过汇编实现）

#### 进程生命周期的信息——即需要保存的信息

- 寄存器

- cpu状态

- 内存地址空间

    （由于内存中信息保存的区域不同，所以大部分不用保存）

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1ghspp0u4wpj31800mwgx7.jpg)

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1ghsqohx4n7j31800n81a5.jpg)

如链表过长检索开销很大，所以加一级hash队列，队列中hash值相同再组成链表

ucore的进程控制块为结构体proc_struct(process/proc.h)

```c
struct proc_struct {
    enum proc_state state;                      // Process state
    int pid;                                    // Process ID
    int runs;                                   // the running times of Proces
    uintptr_t kstack;                           // Process kernel stack
    volatile bool need_resched;                 // bool value: need to be rescheduled to release CPU?
    struct proc_struct *parent;                 // the parent process
    struct mm_struct *mm;                       // Process's memory management field
    struct context context;                     // Switch here to run process
    struct trapframe *tf;                       // Trap frame for current interrupt
    uintptr_t cr3;                              // CR3 register: the base addr of Page Directroy Table(PDT)
    uint32_t flags;                             // Process flag
    char name[PROC_NAME_LEN + 1];               // Process name
    list_entry_t list_link;                     // Process link list 
    list_entry_t hash_link;                     // Process hash list
};
```



### 进程创建

Windows进程创建API：CreateProcess(filename)

Unix进程创建系统调用：fork()/exec()

> fork函数的工作
>
> 1. 分配进程控制块数据结构
> 2. 创建内核堆栈、设置地址空间
> 3. 修改子进程状态为运行状态
> 4. 设置进程标识

系统没有任务执行时，处于暂停状态，执行**空闲进程**的代码

空闲进程

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1ghsvz3e9t7j30hk0a8goi.jpg)

fork复制整个进程开销大——使用vfork函数创建进程时，不创建一个同样的内存映像；使用写时复制技术

### 进程加载

使用exec函数加载，加载成功时，还是原来的进程，但是运行了不同的程序（从main开始执行，代码段、堆栈和堆等完全重写

### 进程等待与退出

子进程通过exit函数向父进程返回一个值

父进程通过wait函数接收这个值

exit函数：

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1ghswmgc3i1j30ne0ean89.jpg)

### 其他进程控制系统调用

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1ghswu0gb6ej31800n4k5f.jpg)



## lab4

### 练习0

>#### 练习0：填写已有实验
>
>本实验依赖实验1/2/3。请把你做的实验1/2/3的代码填入本实验中代码中有“LAB1”,“LAB2”,“LAB3”的注释相应部分。

### 练习1

>#### 练习1：分配并初始化一个进程控制块（需要编码）
>
>alloc_proc函数（位于kern/process/proc.c中）负责分配并返回一个新的struct proc_struct结构，用于存储新建立的内核线程的管理信息。ucore需要对这个结构进行最基本的初始化，你需要完成这个初始化过程。
>
>> 【提示】在alloc_proc函数的实现中，需要初始化的proc_struct结构中的成员变量至少包括：state/pid/runs/kstack/need_resched/parent/mm/context/tf/cr3/flags/name。
>
>请在实验报告中简要说明你的设计实现过程。请回答如下问题：
>
>- 请说明proc_struct中`struct context context`和`struct trapframe *tf`成员变量含义和在本实验中的作用是啥？（提示通过看代码和编程调试可以判断出来）

alloc_proc用于创建一个TCB内存块并初始化

>首先把proc进行初步初始化（即把proc_struct中的各个成员变量清零）。但有些成员变量设置了特殊的值，比如：
>
>```
>proc->state = PROC_UNINIT;  设置进程为“初始”态
>proc->pid = -1;             设置进程pid的未初始化值
>proc->cr3 = boot_cr3;       使用内核页目录表的基址
>...
>```
>
>上述三条语句中,第一条设置了进程的状态为“初始”态，这表示进程已经 “出生”了，正在获取资源茁壮成长中；第二条语句设置了进程的pid为-1，这表示进程的“身份证号”还没有办好；第三条语句表明由于该内核线程在内核中运行，故采用为uCore内核已经建立的页表，即设置为在uCore内核页表的起始地址boot_cr3。后续实验中可进一步看出所有内核线程的内核虚地址空间（也包括物理地址空间）是相同的。既然内核线程共用一个映射内核空间的页表，这表示内核空间对所有内核线程都是“可见”的，所以更精确地说，这些内核线程都应该是从属于同一个唯一的“大内核进程”—uCore内核。

补充alloc_proc函数

```c
// alloc_proc - alloc a proc_struct and init all fields of proc_struct
static struct proc_struct *
alloc_proc(void) {
    struct proc_struct *proc = kmalloc(sizeof(struct proc_struct));
    if (proc != NULL) {
    //LAB4:EXERCISE1 YOUR CODE
    /*
     * below fields in proc_struct need to be initialized
     *       enum proc_state state;                      // Process state
     *       int pid;                                    // Process ID
     *       int runs;                                   // the running times of Proces
     *       uintptr_t kstack;                           // Process kernel stack
     *       volatile bool need_resched;                 // bool value: need to be rescheduled to release CPU?
     *       struct proc_struct *parent;                 // the parent process
     *       struct mm_struct *mm;                       // Process's memory management field
     *       struct context context;                     // Switch here to run process
     *       struct trapframe *tf;                       // Trap frame for current interrupt
     *       uintptr_t cr3;                              // CR3 register: the base addr of Page Directroy Table(PDT)
     *       uint32_t flags;                             // Process flag
     *       char name[PROC_NAME_LEN + 1];               // Process name
     */
      proc->state = PROC_UNINIT;//状态设置为初始化状态
      proc->pid = -1;//进程pid初始化为-1
      proc->runs = 0;//初始化时间片
      proc->kstack = 0;//内核栈地址初始化为0
      proc->need_resched = 0;//初始化为0
      proc->parent = NULL;//初始化为空
      proc->mm = NULL;//初始化为空
      memset(&(proc->context), 0, sizeof(struct context)); //初始化为0
      proc->tf = NULL;//初始化为空
      proc->cr3 = boot_cr3;//初始化为内核页表的起始地址
      proc->flags = 0;//初始化为0
      memset(proc->name, 0, PROC_NAME_LEN);//初始化为0
    }
    return proc;
}
```

- 请说明proc_struct中`struct context context`和`struct trapframe *tf`成员变量含义和在本实验中的作用是啥？（提示通过看代码和编程调试可以判断出来）

    ```c
    struct context {
        uint32_t eip;
        uint32_t esp;
        uint32_t ebx;
        uint32_t ecx;
        uint32_t edx;
        uint32_t esi;
        uint32_t edi;
        uint32_t ebp;
    };
    ```

    可知context用于保存进程上下文信息

    在copy_thread函数中

    ```c
    static void
    copy_thread(struct proc_struct *proc, uintptr_t esp, struct trapframe *tf) {
        proc->tf = (struct trapframe *)(proc->kstack + KSTACKSIZE) - 1;
        *(proc->tf) = *tf;
        proc->tf->tf_regs.reg_eax = 0;
        proc->tf->tf_esp = esp;
        proc->tf->tf_eflags |= FL_IF;
    
        proc->context.eip = (uintptr_t)forkret;
        proc->context.esp = (uintptr_t)(proc->tf);
    }
    ```

    设置了context变量的esp为tf；eip为forkret

    tf为中断帧的指针，当进程从用户空间跳到内核空间时，中断帧记录了进程在被中断前的状态。当内核需要跳回用户空间时，需要调整中断帧以恢复让进程继续执行的各寄存器值。

    在嵌套中断的时候，为了保证嵌套中断发生时tf 总是能够指向当前的trapframe，ucore 在内核栈上维护了 tf 的链

### 练习2

>#### 练习2：为新创建的内核线程分配资源（需要编码）
>
>创建一个内核线程需要分配和设置好很多资源。kernel_thread函数通过调用**do_fork**函数完成具体内核线程的创建工作。do_kernel函数会调用alloc_proc函数来分配并初始化一个进程控制块，但alloc_proc只是找到了一小块内存用以记录进程的必要信息，并没有实际分配这些资源。ucore一般通过do_fork实际创建新的内核线程。do_fork的作用是，创建当前内核线程的一个副本，它们的执行上下文、代码、数据都一样，但是存储位置不同。在这个过程中，需要给新内核线程分配资源，并且复制原进程的状态。你需要完成在kern/process/proc.c中的do_fork函数中的处理过程。它的大致执行步骤包括：
>
>- 调用alloc_proc，首先获得一块用户信息块。
>- 为进程分配一个内核栈。
>- 复制原进程的内存管理信息到新进程（但内核线程不必做此事）
>- 复制原进程上下文到新进程
>- 将新进程添加到进程列表
>- 唤醒新进程
>- 返回新进程号
>
>请在实验报告中简要说明你的设计实现过程。请回答如下问题：
>
>- 请说明ucore是否做到给每个新fork的线程一个唯一的id？请说明你的分析和理由。

```c
/* do_fork -     parent process for a new child process
 * @clone_flags: used to guide how to clone the child process
 * @stack:       the parent's user stack pointer. if stack==0, It means to fork a kernel thread.
 * @tf:          the trapframe info, which will be copied to child process's proc->tf
 */
int
do_fork(uint32_t clone_flags, uintptr_t stack, struct trapframe *tf) {
    int ret = -E_NO_FREE_PROC;
    struct proc_struct *proc;
    if (nr_process >= MAX_PROCESS) {
        goto fork_out;
    }
    ret = -E_NO_MEM;
    //LAB4:EXERCISE2 YOUR CODE
    /*
     * Some Useful MACROs, Functions and DEFINEs, you can use them in below implementation.
     * MACROs or Functions:
     *   alloc_proc:   create a proc struct and init fields (lab4:exercise1)
     *   setup_kstack: alloc pages with size KSTACKPAGE as process kernel stack
     *   copy_mm:      process "proc" duplicate OR share process "current"'s mm according clone_flags
     *                 if clone_flags & CLONE_VM, then "share" ; else "duplicate"
     *   copy_thread:  setup the trapframe on the  process's kernel stack top and
     *                 setup the kernel entry point and stack of process
     *   hash_proc:    add proc into proc hash_list
     *   get_pid:      alloc a unique pid for process
     *   wakeup_proc:  set proc->state = PROC_RUNNABLE
     * VARIABLES:
     *   proc_list:    the process set's list
     *   nr_process:   the number of process set
     */

    //    1. call alloc_proc to allocate a proc_struct
    //    2. call setup_kstack to allocate a kernel stack for child process
    //    3. call copy_mm to dup OR share mm according clone_flag
    //    4. call copy_thread to setup tf & context in proc_struct
    //    5. insert proc_struct into hash_list && proc_list
    //    6. call wakeup_proc to make the new child process RUNNABLE
    //    7. set ret vaule using child proc's pid
    if ((proc = alloc_proc()) == NULL) {//申请一个proc结构体
        goto fork_out;
    }

    if (setup_kstack(proc) != 0) {//分配内核栈地址（由于内核空间使用同一个内核栈，所以实际上不需要这一步）
        goto bad_fork_cleanup_proc;
    }
    if (copy_mm(clone_flags, proc) != 0) {//复制父进程的内存
        goto bad_fork_cleanup_kstack;
    }

    copy_thread(proc, stack, tf);//复制中断帧和上下文信息
		
  	bool intr_flag;//按照参考答案 以下操作需要屏蔽中断进行
  	local_intr_save(intr_flag); //屏蔽中断
    proc->pid = get_pid();//获取pid，在下面建立hash映射和插入进程链表需要用到
    hash_proc(proc);
    list_add(&proc_list, &(proc->list_link));
  	local_intr_restore(intr_flag);//恢复中断
  
    wakeup_proc(proc);//唤醒进程，将其放在就绪队列
    
    ret = proc->pid;//返回进程pid

fork_out:
    return ret;

bad_fork_cleanup_kstack:
    put_kstack(proc);
bad_fork_cleanup_proc:
    kfree(proc);
    goto fork_out;
}
```

- 请说明ucore是否做到给每个新fork的线程一个唯一的id？请说明你的分析和理由。

    ucore中使用get_pid函数获取进程id

    ```c
    // get_pid - alloc a unique pid for process
    static int
    get_pid(void) {
        static_assert(MAX_PID > MAX_PROCESS);
        struct proc_struct *proc;
        list_entry_t *list = &proc_list, *le;
        static int next_safe = MAX_PID, last_pid = MAX_PID;
        if (++ last_pid >= MAX_PID) {
            last_pid = 1;
            goto inside;
        }
        if (last_pid >= next_safe) {
        inside:
            next_safe = MAX_PID;
        repeat:
            le = list;
            while ((le = list_next(le)) != list) {
                proc = le2proc(le, list_link);
                if (proc->pid == last_pid) {
                    if (++ last_pid >= next_safe) {
                        if (last_pid >= MAX_PID) {
                            last_pid = 1;
                        }
                        next_safe = MAX_PID;
                        goto repeat;
                    }
                }
                else if (proc->pid > last_pid && next_safe > proc->pid) {
                    next_safe = proc->pid;
                }
            }
        }
        return last_pid;
    }
    ```

    last_pid和next_safe之间为 没有被分配的pid

    首先判断last_pid是否超出MAX_PID，超出则变成1，然后进入循环

    如果满足`next_safe > last_pid + 1`，那么就可以直接更新last_pid为`last_pid + 1`并返回作为新的pid；如果不满足前面的条件，那么进入循环

    循环中两个条件判断分支，`proc->pid == last_pid`保证不会分配到proc_list中进程的pid，`proc->pid > last_pid && next_safe > proc->pid`保证last_pid和next_safe之间为 没有被分配的pid

    最后返回last_pid

    ucore给每个新fork的线程分配了一个唯一的id

### 练习3

>#### 练习3：阅读代码，理解 proc_run 函数和它调用的函数如何完成进程切换的。（无编码工作）
>
>请在实验报告中简要说明你对proc_run函数的分析。并回答如下问题：
>
>- 在本实验的执行过程中，创建且运行了几个内核线程？
>- 语句`local_intr_save(intr_flag);....local_intr_restore(intr_flag);`在这里有何作用?请说明理由
>
>完成代码编写后，编译并运行代码：make qemu
>
>如果可以得到如 附录A所示的显示内容（仅供参考，不是标准答案输出），则基本正确。

proc_run函数

```c
// proc_run - make process "proc" running on cpu
// NOTE: before call switch_to, should load  base addr of "proc"'s new PDT
void
proc_run(struct proc_struct *proc) {
    if (proc != current) {//判断目前进程是不是需要运行的进程
        bool intr_flag;
        struct proc_struct *prev = current, *next = proc;
        local_intr_save(intr_flag);//屏蔽中断
        {
            current = proc;//设置当前进程为新进程
            load_esp0(next->kstack + KSTACKSIZE);//设置tss
            lcr3(next->cr3);//加载新进程的页目录表到cr3中
            switch_to(&(prev->context), &(next->context));//切换进程
        }
        local_intr_restore(intr_flag);//恢复中断
    }
}
```

调用swtich_to

>在调用该函数之后，首先会恢复要运行的线程的上下文，然后由于恢复的上下文中已经将返回地址（copy_thread函数中完成）修改成了forkret函数的地址(如果这个线程是第一运行的话，否则就是切换到这个线程被切换出来的地址)，也就是会跳转到这个函数，最后进一步跳转到了__trapsret函数，调用iret最终将控制权切换到新的线程；
>
>作者：AmadeusChan
>链接：https://www.jianshu.com/p/50dd281a82f0
>来源：简书
>著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

- 在本实验的执行过程中，创建且运行了几个内核线程？

    创建了idleproc(最初的内核线程)和initproc(用于打印"hello world"的线程)

- 语句`local_intr_save(intr_flag);....local_intr_restore(intr_flag);`在这里有何作用?请说明理由

    分别为屏蔽中断和恢复中断，让两语句中间的操作成为原子操作（即不可被中断）

