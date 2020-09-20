## 知识点

内核内存地址布局

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1gi6j9lhm65j31820n24a8.jpg)

用户内存地址布局

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1gi6j11wjv7j317y0n415l.jpg)

## lab5

在该lab中，hello应用程序的位置和大小记录在全局变量_binary_obj___user_hello_out_start和_binary_obj___user_hello_out_size中，这个hello用户程序能够和ucore内核一起被 bootloader 加载到内存里中，通过这两个全局变量定位hello用户程序执行码的起始位置和大小（后面文件系统的lab则不用此方法）

### 练习0

>#### 练习0：填写已有实验
>
>本实验依赖实验1/2/3/4。请把你做的实验1/2/3/4的代码填入本实验中代码中有“LAB1”/“LAB2”/“LAB3”/“LAB4”的注释相应部分。注意：为了能够正确执行lab5的测试应用程序，可能需对已完成的实验1/2/3/4的代码进行进一步改进。

填写已有实验，需要对原来的代码进一步改进

trap_dispatch函数(trap/trap.c)

```c
static void
trap_dispatch(struct trapframe *tf) {
    char c;

    int ret=0;

    switch (tf->tf_trapno) {
    case T_PGFLT:  //page fault
        if ((ret = pgfault_handler(tf)) != 0) {
            print_trapframe(tf);
            if (current == NULL) {
                panic("handle pgfault failed. ret=%d\n", ret);
            }
            else {
                if (trap_in_kernel(tf)) {
                    panic("handle pgfault failed in kernel mode. ret=%d\n", ret);
                }
                cprintf("killed by kernel.\n");
                panic("handle user mode pgfault failed. ret=%d\n", ret); 
                do_exit(-E_KILLED);
            }
        }
        break;
    case T_SYSCALL:
        syscall();
        break;
    case IRQ_OFFSET + IRQ_TIMER:
#if 0
    LAB3 : If some page replacement algorithm(such as CLOCK PRA) need tick to change the priority of pages,
    then you can add code here. 
#endif
        /* LAB1 YOUR CODE : STEP 3 */
        /* handle the timer interrupt */
        /* (1) After a timer interrupt, you should record this event using a global variable (increase it), such as ticks in kern/driver/clock.c
         * (2) Every TICK_NUM cycle, you can print some info using a funciton, such as print_ticks().
         * (3) Too Simple? Yes, I think so!
         */
        /* LAB5 YOUR CODE */
        /* you should upate you lab1 code (just add ONE or TWO lines of code):
         *    Every TICK_NUM cycle, you should set current process's current->need_resched = 1
         */
      	if (ticks == TICK_NUM) {
            current->need_resched = 1;//更新，将时间片设置为需要调度，说明当前进程的时间片已经用完了
	    			ticks = 0;
        }
				ticks ++;
        break;
    case IRQ_OFFSET + IRQ_COM1:
        c = cons_getc();
        cprintf("serial [%03d] %c\n", c, c);
        break;
    case IRQ_OFFSET + IRQ_KBD:
        c = cons_getc();
        cprintf("kbd [%03d] %c\n", c, c);
        break;
    //LAB1 CHALLENGE 1 : YOUR CODE you should modify below codes.
    case T_SWITCH_TOU:
    case T_SWITCH_TOK:
        panic("T_SWITCH_** ??\n");
        break;
    case IRQ_OFFSET + IRQ_IDE1:
    case IRQ_OFFSET + IRQ_IDE2:
        /* do nothing */
        break;
    default:
        print_trapframe(tf);
        if (current != NULL) {
            cprintf("unhandled trap.\n");
            do_exit(-E_KILLED);
        }
        // in kernel, it must be a mistake
        panic("unexpected trap in kernel.\n");

    }
}
```



proc结构体发生改变

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
    int exit_code;                              // exit code (be sent to parent proc)
    uint32_t wait_state;                        // waiting state
    struct proc_struct *cptr, *yptr, *optr;     // relations between processes
};
```

>parent:           proc->parent  (proc is children)
>children:         proc->cptr    (proc is parent)
>older sibling:    proc->optr    (proc is younger sibling)
>younger sibling:  proc->yptr    (proc is older sibling)

alloc_proc函数(process/proc.c)

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
     //LAB5 YOUR CODE : (update LAB4 steps)
    /*
     * below fields(add in LAB5) in proc_struct need to be initialized	
     *       uint32_t wait_state;                        // waiting state
     *       struct proc_struct *cptr, *yptr, *optr;     // relations between processes
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
      proc->wait_state = 0;//更新，初始化进程为等待状态
      proc->cptr = proc->yptr = proc->optr = NULL;//更新，初始化指针为空
    }
    return proc;
}
```

do_fork函数(process/proc.c)

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

	//LAB5 YOUR CODE : (update LAB4 steps)
   /* Some Functions
    *    set_links:  set the relation links of process.  ALSO SEE: remove_links:  lean the relation links of process 
    *    -------------------
	*    update step 1: set child proc's parent to current process, make sure current process's wait_state is 0
	*    update step 5: insert proc_struct into hash_list && proc_list, set the relation links of process
    */
	if ((proc = alloc_proc()) == NULL) {
        goto fork_out;
    }
    proc->parent = current;//更新，设置父进程，并确保其在等待状态
    assert(current->wait_state == 0);

    if (setup_kstack(proc) != 0){//分配内核栈地址（由于内核空间使用同一个内核栈，所以实际上不需要这一步）
        goto bad_fork_cleanup_proc;
    }
    if (copy_mm(clone_flags, proc) != 0){//复制父进程的内存
        goto bad_fork_cleanup_kstack;
    }

    copy_thread(proc, stack, tf);//复制中断帧和上下文信息

    
    bool intr_flag;//按照参考答案 以下操作需要屏蔽中断进行
    local_intr_save(intr_flag); //屏蔽中断
    proc->pid = get_pid();//获取pid，在下面建立hash映射和插入进程链表需要用到
    hash_proc(proc);
    //list_add(&proc_list, &(proc->list_link)); //set_link函数中调用了list_add函数
    set_links(proc);//更新，设置进程的关系进程
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



### 练习1

>#### 练习1: 加载应用程序并执行（需要编码）
>
>**do_execv**函数调用load_icode（位于kern/process/proc.c中）来加载并解析一个处于内存中的ELF执行文件格式的应用程序，建立相应的用户内存空间来放置应用程序的代码段、数据段等，且要设置好proc_struct结构中的成员变量trapframe中的内容，确保在执行此进程后，能够从应用程序设定的起始执行地址开始执行。需设置正确的trapframe内容。
>
>请在实验报告中简要说明你的设计实现过程。
>
>请在实验报告中描述当创建一个用户态进程并加载了应用程序后，CPU是如何让这个应用程序最终在用户态执行起来的。即这个用户态进程被ucore选择占用CPU执行（RUNNING态）到具体执行应用程序第一条指令的整个经过。

load_icode函数给用户进程建立一个能够让用户进程正常运行的用户环境，完成了如下重要工作：

>1. 调用mm_create函数来申请进程的内存管理数据结构mm所需内存空间，并对mm进行初始化；
>2. 调用setup_pgdir来申请一个页目录表所需的一个页大小的内存空间，并把描述ucore内核虚空间映射的内核页表（boot_pgdir所指）的内容拷贝到此新目录表中，最后让mm->pgdir指向此页目录表，这就是进程新的页目录表了，且能够正确映射内核虚空间；
>3. 根据应用程序执行码的起始位置来解析此ELF格式的执行程序，并调用mm_map函数根据ELF格式的执行程序说明的各个段（代码段、数据段、BSS段等）的起始位置和大小建立对应的vma结构，并把vma插入到mm结构中，从而表明了用户进程的合法用户态虚拟地址空间；
>4. 调用根据执行程序各个段的大小分配物理内存空间，并根据执行程序各个段的起始位置确定虚拟地址，并在页表中建立好物理地址和虚拟地址的映射关系，然后把执行程序各个段的内容拷贝到相应的内核虚拟地址中，至此应用程序执行码和数据已经根据编译时设定地址放置到虚拟内存中了；
>5. 需要给用户进程设置用户栈，为此调用mm_mmap函数建立用户栈的vma结构，明确用户栈的位置在用户虚空间的顶端，大小为256个页，即1MB，并分配一定数量的物理内存且建立好栈的虚地址<-->物理地址映射关系；
>6. 至此,进程内的内存管理vma和mm数据结构已经建立完成，于是把mm->pgdir赋值到cr3寄存器中，即更新了用户进程的虚拟内存空间，此时的initproc已经被hello的代码和数据覆盖，成为了第一个用户进程，但此时这个用户进程的执行现场还没建立好；
>7. 先清空进程的中断帧，再重新设置进程的中断帧，使得在执行中断返回指令“iret”后，能够让CPU转到用户态特权级，并回到用户态内存空间，使用用户态的代码段、数据段和堆栈，且能够跳转到用户进程的第一条指令执行，并确保在用户态能够响应中断；

ucore中trapframe结构体如下

```c
struct trapframe {
    struct pushregs tf_regs;
    uint16_t tf_gs;
    uint16_t tf_padding0;
    uint16_t tf_fs;
    uint16_t tf_padding1;
    uint16_t tf_es;
    uint16_t tf_padding2;
    uint16_t tf_ds;
    uint16_t tf_padding3;
    uint32_t tf_trapno;
    /* below here defined by x86 hardware */
    uint32_t tf_err;
    uintptr_t tf_eip;
    uint16_t tf_cs;
    uint16_t tf_padding4;
    uint32_t tf_eflags;
    /* below here only when crossing rings, such as from user to kernel */
    uintptr_t tf_esp;
    uint16_t tf_ss;
    uint16_t tf_padding5;
} ;
```

补充load_icode函数实现其第7部分的工作

```c
/* LAB5:EXERCISE1 YOUR CODE
     * should set tf_cs,tf_ds,tf_es,tf_ss,tf_esp,tf_eip,tf_eflags
     * NOTICE: If we set trapframe correctly, then the user level process can return to USER MODE from kernel. So
     *          tf_cs should be USER_CS segment (see memlayout.h)
     *          tf_ds=tf_es=tf_ss should be USER_DS segment
     *          tf_esp should be the top addr of user stack (USTACKTOP)
     *          tf_eip should be the entry point of this binary program (elf->e_entry)
     *          tf_eflags should be set to enable computer to produce Interrupt
     */
    tf->tf_cs = USER_CS;//设置CS段寄存器
    tf->tf_ds = tf->tf_es = tf->tf_ss = USER_DS;//设置DS、ES、SS段寄存器
    tf->tf_esp = USTACKTOP;//esp指向栈顶
    tf->tf_eip = elf->e_entry;//eip指向entry
    tf->tf_eflags = FL_IF;		// Interrupt Flag(mm/mmu.h)
    ret = 0;
```

- 请在实验报告中描述当创建一个用户态进程并加载了应用程序后，CPU是如何让这个应用程序最终在用户态执行起来的。即这个用户态进程被ucore选择占用CPU执行（RUNNING态）到具体执行应用程序第一条指令的整个经过。

    通过do_execve函数来完成指定应用程序的加载，包括使用load_icode函数完成对整个用户线程内存空间的初始化，包括堆栈的设置以及将ELF可执行文件的加载、通过tf指针修改了当前系统调用的trapframe，使得最终中断返回的时候能够切换到用户态且可以正确地将控制权转移到应用程序的入口处

    之后就是iret进行中断返回，将堆栈切换到用户的栈，并且完成特权级的切换，并且跳转到要求的应用程序的入口处

### 练习2

>#### 练习2: 父进程复制自己的内存空间给子进程（需要编码）
>
>创建子进程的函数do_fork在执行中将拷贝当前进程（即父进程）的用户内存地址空间中的合法内容到新进程中（子进程），完成内存资源的复制。具体是通过copy_range函数（位于kern/mm/pmm.c中）实现的，请补充copy_range的实现，确保能够正确执行。
>
>请在实验报告中简要说明如何设计实现”Copy on Write 机制“，给出概要设计，鼓励给出详细设计。
>
>> Copy-on-write（简称COW）的基本概念是指如果有多个使用者对一个资源A（比如内存块）进行读操作，则每个使用者只需获得一个指向同一个资源A的指针，就可以该资源了。若某使用者需要对这个资源A进行写操作，系统会对该资源进行拷贝操作，从而使得该“写操作”使用者获得一个该资源A的“私有”拷贝—资源B，可对资源B进行写操作。该“写操作”使用者对资源B的改变对于其他的使用者而言是不可见的，因为其他使用者看到的还是资源A。

copy_range函数功能遍历父进程的虚拟页，为子进程申请物理页，从而对应父进程存在的页，但由于页目录表不一样，他们不是一个内存空间。然后将父进程物理页中的所有内容复制到子进程物理页中去，然后为子进程的这个物理页和对应的虚拟地址（事实上是线性地址）建立映射关系

补充copy_range函数

```c
/* copy_range - copy content of memory (start, end) of one process A to another process B
 * @to:    the addr of process B's Page Directory
 * @from:  the addr of process A's Page Directory
 * @share: flags to indicate to dup OR share. We just use dup method, so it didn't be used.
 *
 * CALL GRAPH: copy_mm-->dup_mmap-->copy_range
 */
int
copy_range(pde_t *to, pde_t *from, uintptr_t start, uintptr_t end, bool share) {
    assert(start % PGSIZE == 0 && end % PGSIZE == 0);
    assert(USER_ACCESS(start, end));
    // copy content by page unit.
    do {
        //call get_pte to find process A's pte according to the addr start
        pte_t *ptep = get_pte(from, start, 0), *nptep;
        if (ptep == NULL) {
            start = ROUNDDOWN(start + PTSIZE, PTSIZE);
            continue ;
        }
        //call get_pte to find process B's pte according to the addr start. If pte is NULL, just alloc a PT
        if (*ptep & PTE_P) {
            if ((nptep = get_pte(to, start, 1)) == NULL) {
                return -E_NO_MEM;
            }
        uint32_t perm = (*ptep & PTE_USER);
        //get page from ptep
        struct Page *page = pte2page(*ptep);
        // alloc a page for process B
        struct Page *npage=alloc_page();
        assert(page!=NULL);
        assert(npage!=NULL);
        int ret=0;
        /* LAB5:EXERCISE2 YOUR CODE
         * replicate content of page to npage, build the map of phy addr of nage with the linear addr start
         *
         * Some Useful MACROs and DEFINEs, you can use them in below implementation.
         * MACROs or Functions:
         *    page2kva(struct Page *page): return the kernel vritual addr of memory which page managed (SEE pmm.h)
         *    page_insert: build the map of phy addr of an Page with the linear addr la
         *    memcpy: typical memory copy function
         *
         * (1) find src_kvaddr: the kernel virtual address of page
         * (2) find dst_kvaddr: the kernel virtual address of npage
         * (3) memory copy from src_kvaddr to dst_kvaddr, size is PGSIZE
         * (4) build the map of phy addr of  nage with the linear addr start
         */
        void * src_kvaddr = page2kva(page);//获取父进程物理页的虚拟地址
        void * dst_kvaddr = page2kva(npage);//获取子进程物理页的虚拟的值
        memcpy(dst_kvaddr, src_kvaddr, PGSIZE);//复制
        ret = page_insert(to, npage, start, perm);//建立子进程物理页和虚拟地址的映射关系
        assert(ret == 0);
        }
        start += PGSIZE;
    } while (start != 0 && start < end);
    return 0;
}
```

- 请在实验报告中简要说明如何设计实现”Copy on Write 机制“，给出概要设计，鼓励给出详细设计。

    >Copy-on-write（简称COW）的基本概念是指如果有多个使用者对一个资源A（比如内存块）进行读操作，则每个使用者只需获得一个指向同一个资源A的指针，就可以该资源了。若某使用者需要对这个资源A进行写操作，系统会对该资源进行拷贝操作，从而使得该“写操作”使用者获得一个该资源A的“私有”拷贝—资源B，可对资源B进行写操作。该“写操作”使用者对资源B的改变对于其他的使用者而言是不可见的，因为其他使用者看到的还是资源A。

    

    1. 在原来应该进行内存复制的时候，比如copy_range函数内部，将子进程和父进程的虚拟页映射上同一个物理页面，而不是进行复制，然后在这两个进程的虚拟页对应的PTE部分将这个页设置成不可写，同时利用PTE中的保留位将这个页设置成共享的页面，如果有应用程序试图写共享页，就会产生页访问异常
    2. 在处理页访问异常的时候需要判断是否是由于应用程序试图写共享页引起的，处理这种页访问异常需要额外申请一个物理页面，复制当前的共享页中的内容，建立出错的线性地址与新创建的物理页的映射关系，将PTE设置设置成非共享的
    3. 如果此时原先的物理页已经没有其他进程共享了，也就是只有一个进程在使用，则需要将对应的虚拟地址的PTE中的共享标记删去，并恢复可写标记

### 练习3

>#### 练习3: 阅读分析源代码，理解进程执行 fork/exec/wait/exit 的实现，以及系统调用的实现（不需要编码）
>
>请在实验报告中简要说明你对 fork/exec/wait/exit函数的分析。并回答如下问题：
>
>- 请分析fork/exec/wait/exit在实现中是如何影响进程的执行状态的？
>- 请给出ucore中一个用户态进程的执行状态生命周期图（包执行状态，执行状态之间的变换关系，以及产生变换的事件或函数调用）。（字符方式画即可）

- fork：进程执行fork函数，会执行中断处理流程，根据系统调用号执行sys_fork函数，进一步执行了上文中的do_fork函数，完成新的进程的进程控制块的初始化、设置、以及将父进程内存中的内容到子进程的内存的复制工作，然后将新创建的进程放入可执行队列（runnable）

- exec：进程执行exec函数，会执行中断处理流程，根据系统调用号执行sys_exec函数，进一步执行了上文中的do_execve函数，在该函数中，会对内存空间进行清空，然后将新的要执行的程序加载到内存中，然后设置好中断帧，使得最终中断返回之后可以跳转到指定的应用程序的入口处

- wait：进程执行wait函数，会执行中断处理流程，根据系统调用号执行sys_wait函数，进一步执行了的do_wait函数

    如果当前进程有已经处于ZOMBIE态的子进程，则直接将其占用的资源释放；否则当前进程进入SLEEPING态，并且标记为 等待ZOMBIE态的子进程 的状态，然后调用schedule函数将当前线程从CPU占用中切换出去，直到有子进程结束信号来唤醒该进程

- exit：进程执行exit函数，会执行中断处理流程，根据系统调用号执行sys_exit函数，进一步执行了的do_exit函数，首先将释放当前进程的大多数资源，然后将其标记为ZOMBIE态（如果其父进程处于SLEEPING态，则调用wakeup_proc函数将其父进程唤醒），然后调用schedule函数，让出CPU资源，等待父进程进一步完成其所有资源的回收




- 请分析fork/exec/wait/exit在实现中是如何影响进程的执行状态的？

    - fork不会影响当前进程的执行状态，但会将子进程的状态标记为RUNNALB
    - exec不会影响当前进程的执行状态
    - wait调用时，如果存在需要释放的 ZOMBIE态的子进程，则释放子进程，当前进程执行状态不变；否则变为SLEEPING态，等待出现ZOMBIE态的子进程将其唤醒
    - exit调用时，释放当前进程的大多数资源，然后标记为ZOMBIE态

- 请给出ucore中一个用户态进程的执行状态生命周期图（包执行状态，执行状态之间的变换关系，以及产生变换的事件或函数调用）。（字符方式画即可）

    ![](https://tva1.sinaimg.cn/large/007S8ZIlgy1gi8u3rurfcj30yu08cjuv.jpg)

    图片取自[](https://www.jianshu.com/p/8c852af5b403)

