## 知识点

### 处理机调度

在进程切换时

- 从就绪队列中挑选下一个占用cpu运行的进程
- 从多个可用的cpu中挑选就绪进程可使用的cpu

调度程序要考虑两个问题

**调度策略**：依据什么原则挑选进程/线程

- cpu使用率：cpu处于忙状态的时间百分比
- 吞吐量：单位时间内完成的进程数量
- 周转时间：进程从初始化到结束（包括等待）的总时间
- 等待时间：进程在就绪队列中的总时间
- 响应时间：从提交请求到产生响应花费的总时间

**调度时机**：什么时候进行调度

- 进程从运行状态切换到等待状态（等待条件）
- 进程退出
- 当前进程主动放弃cpu（非抢占系统）
- 中断请求（可抢占系统）
- 当前进程被抢占（可抢占系统）
    - 进程时间片用完
    - 进程从等待切换到就绪（等待条件满足）

#### 调度算法

**先来先服务算法**

**短进程优先算法**

- 有最优平均周转时间
- 会造成“饥饿”现象，即长进程一直处于等待状态

**最高响应比优先算法**（依据进程在就绪队列中的等待时间）

- R(响应比) = (w + s) / s

    w：等待时间   s：执行时间

- 不可抢占

- 等待时间越久，优先级越高

时间片轮转算法（轮流占用基本时间片，在就绪队列中依然使用先来先服务算法）

多级反馈队列（就绪队列分成多个子队列，不同子队列不同调度算法）

公平共享调度算法（按照进程占用资源调度，保证每个进程占用的资源公平）



## lab6

Round Robin调度算法（时间片轮转）

### 练习0

>#### 练习0：填写已有实验
>
>本实验依赖实验1/2/3/4/5。请把你做的实验2/3/4/5的代码填入本实验中代码中有“LAB1”/“LAB2”/“LAB3”/“LAB4”“LAB5”的注释相应部分。并确保编译通过。注意：为了能够正确执行lab6的测试应用程序，可能需对已完成的实验1/2/3/4/5的代码进行进一步改进。

填写已有实验，需要对原来的代码进一步改进

没有找到sched_class_proc_tick函数定义

猜测sched_class_proc_tick函数是RR_proc_tick函数的封装

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1gidj04f8vvj30tc0ig103.jpg)

每个进程初始化时都会设置time_slice，在产生时钟中断的时候会触发以上函数调用，产生一次时钟中断意味着时间流逝一小段，time_slice进行减一操作，一旦time_slice通过减操作变为0，就意味着这个进程时间片用完，则放弃cpu（将need_resched置1，表示该进程需要调度出去，在之后的中断处理历程中会判断need_resched是否为1）

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
		//if (ticks == TICK_NUM) {
    //    current->need_resched = 1;
	  //		ticks = 0;
    //}
		//ticks ++;
        /* LAB6 YOUR CODE */
        /* you should upate you lab5 code
         * IMPORTANT FUNCTIONS:
	     	 * sched_class_proc_tick
         */
        ticks ++;
        sched_class_proc_tick(current);//更新，将设置需要调度的标记放在sched_class_proc_tick函数中
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
    
    // 该进程是否需要调度，只对当前进程有效
    volatile bool need_resched;
    // 该进程的调度链表结构，该结构内部的连接组成了 运行队列 列表
    list_entry_t run_link;
   	// 该进程剩余的时间片，只对当前进程有效
    int time_slice;
  
    // round-robin 调度器并不会用到以下成员
    // 该进程在优先队列中的节点，仅在 LAB6 使用
    skew_heap_entry_t  lab6_run_pool;
    // 该进程的调度优先级，仅在 LAB6 使用
    uint32_t lab6_priority;
    // 该进程的调度步进值，仅在 LAB6 使用
    uint32_t lab6_stride;
};
```

更新初始化proc结构体的函数

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
      proc->wait_state = 0;//初始化进程为等待状态
      proc->cptr = proc->yptr = proc->optr = NULL;//初始化指针为空
     //LAB6 YOUR CODE : (update LAB5 steps)
    /*
     * below fields(add in LAB6) in proc_struct need to be initialized
     *     struct run_queue *rq;                       // running queue contains Process
     *     list_entry_t run_link;                      // the entry linked in run queue
     *     int time_slice;                             // time slice for occupying the CPU
     *     skew_heap_entry_t lab6_run_pool;            // FOR LAB6 ONLY: the entry in the run pool
     *     uint32_t lab6_stride;                       // FOR LAB6 ONLY: the current stride of the process
     *     uint32_t lab6_priority;                     // FOR LAB6 ONLY: the priority of process, set by lab6_set_priority(uint32_t)
     */
      //更新
      proc->rq = NULL;//初始化进程所属运行队列队列
      list_init(&(proc->run_link));//初始化该进程所属的运行队列
      proc->time_slice = 0;//初始化该进程剩余的时间片
      //Round Robin不会用到以下成员
      
      proc->lab6_run_pool.left = proc->lab6_run_pool.right = proc->lab6_run_pool.parent = NULL;
      //初始化该进程在优先队列中的节点，仅在 LAB6 使用
      proc->lab6_stride = 0;
      //初始化该进程的调度步进值，仅在 LAB6 使用
      proc->lab6_priority = 0;
      //初始化该进程的调度优先级，仅在 LAB6 使用
    }
    return proc;
}
```

（run queue——运行队列，就是就绪队列的意思）



### 练习1

>#### 练习1: 使用 Round Robin 调度算法（不需要编码）
>
>完成练习0后，建议大家比较一下（可用kdiff3等文件比较软件）个人完成的lab5和练习0完成后的刚修改的lab6之间的区别，分析了解lab6采用RR调度算法后的执行过程。执行make grade，大部分测试用例应该通过。但执行priority.c应该过不去。
>
>请在实验报告中完成：
>
>- 请理解并分析sched_class中各个函数指针的用法，并结合Round Robin 调度算法描ucore的调度执行过程
>- 请在实验报告中简要说明如何设计实现”多级反馈队列调度算法“，给出概要设计，鼓励给出详细设计

Round Robin 调度算法的基本原理是 每个进程都有一个时间片，时间片用完则等到下一轮再执行

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1gidl5qtcshj30sa0fggu9.jpg)

这些函数的具体实现，体现时间片轮转的特性

运行进程时会设定time_slice，每个时钟中断都会通过proc_tick进行time_slice减一操作并判断是否为0，如果time_slice为0则设置为需要调度状态，在中断处理历程中会进行判断，如果判断需要将进程换出运行队列，则执行schedule进行切换。切换时，进程在队列中进行入队通过enqueue函数实现，出队通过 pick_next函数——选择 和 dequeue函数——出队 实现

- 请理解并分析sched_class中各个函数指针的用法，并结合Round Robin 调度算法描ucore的调度执行过程

    run_queue结构体定义如下：

    ```c
    struct run_queue {
    	list_entry_t run_list;//运行队列链表入口
    	unsigned int proc_num;//在运行队列中进程的个数
    	int max_time_slice;
    };
    ```

    init函数

    ```c
    static void RR_init(struct run_queue *rq) {
        list_init(&(rq->run_list));//初始化运行队列
        rq->proc_num = 0;//初始化进程个数
    }
    ```

    enqueue函数

    ```c
    static void RR_enqueue(struct run_queue *rq, struct proc_struct *proc) {
        assert(list_empty(&(proc->run_link)));
        list_add_before(&(rq->run_list), &(proc->run_link));//将当前进程插入运行队列，FIFO
        if (proc->time_slice == 0 || proc->time_slice > rq->max_time_slice) {
            proc->time_slice = rq->max_time_slice;
        }
        proc->rq = rq;
        rq->proc_num ++;
    }
    ```

    pick_next函数

    ```c
    static struct proc_struct * RR_pick_next(struct run_queue *rq) {
        list_entry_t *le = list_next(&(rq->run_list));//从运行队列选择下一个占用cpu的进程，FIFO
        if (le != &(rq->run_list)) {
            return le2proc(le, run_link);
        }
        return NULL;
    }
    ```

    dequeue函数

    ```c
    static void RR_dequeue(struct run_queue *rq, struct proc_struct *proc) {
        assert(!list_empty(&(proc->run_link)) && proc->rq == rq);
        list_del_init(&(proc->run_link));//将所选择的进程取出运行队列
        rq->proc_num --;
    }
    ```

    还有练习0说过的

    proc_tick函数

    ```c
    static void RR_proc_tick(struct run_queue *rq, struct proc_struct *proc) {
        if (proc->time_slice > 0) {
            proc->time_slice --;
        }
        if (proc->time_slice == 0) {
            proc->need_resched = 1;
        }
    }
    ```

- 请在实验报告中简要说明如何设计实现”多级反馈队列调度算法“，给出概要设计，鼓励给出详细设计

    1. 使用多个就绪队列，每个队列都有着各自的优先级，编号从小到大，则优先级从高到低低

    2. 在PCB中记录当前进程所处的队列的优先级

    3. 队列使用时间片轮转调度算法，编号从小到大，则时间片从小到大

    4. 如果进程在当前的时间片没有完成，则降到下一个优先级

        >5. 在选择下一个执行的进程的时候，有限考虑高优先级的队列中是否存在任务，如果不存在才转而寻找较低优先级的队列；（有可能导致饥饿）
        >6. 从就绪进程集合中删除某一个进程就只需要在对应队列中删除即可；
        >7. 处理时间中断的函数不需要改变
        >
        >
        >
        >作者：AmadeusChan
        >链接：https://www.jianshu.com/p/fd1a1a7d4892
        >来源：简书
        >著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。



### 练习2

>#### 练习2: 实现 Stride Scheduling 调度算法（需要编码）
>
>首先需要换掉RR调度器的实现，即用default_sched_stride_c覆盖default_sched.c。然后根据此文件和后续文档对Stride度器的相关描述，完成Stride调度算法的实现。
>
>后面的实验文档部分给出了Stride调度算法的大体描述。这里给出Stride调度算法的一些相关的资料（目前网上中文的资料比较欠缺）。
>
>- [strid-shed paper location1](http://wwwagss.informatik.uni-kl.de/Projekte/Squirrel/stride/node3.html)
>- [strid-shed paper location2](http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.138.3502&rank=1)
>- 也可GOOGLE “Stride Scheduling” 来查找相关资料
>
>执行：make grade。如果所显示的应用程序检测都输出ok，则基本正确。如果只是priority.c过不去，可执行 make run-priority 命令来单独调试它。大致执行结果可看附录。（ 使用的是 qemu-1.0.1 ）。
>
>请在实验报告中简要说明你的设计实现过程。

Stride调度算法

>该算法的基本思想可以考虑如下：
>
>1. 为每个runnable的进程设置一个当前状态stride，表示该进程当前的调度权。另外定义其对应的pass值，表示对应进程在调度后，stride 需要进行的累加值。
>2. 每次需要调度时，从当前 runnable 态的进程中选择 stride最小的进程调度。
>3. 对于获得调度的进程P，将对应的stride加上其对应的步长pass（只与进程的优先权有关系）。
>4. 在一段固定的时间之后，回到 2.步骤，重新调度当前stride最小的进程。
>    可以证明，如果令 P.pass =BigStride / P.priority ，其中 P.priority 表示进程的优先权（大于 1），而 BigStride 表示一个预先定义的大常数，则该调度方案为每个进程分配的时间将与其优先级成正比。证明过程我们在这里略去，有兴趣的同学可以在网上查找相关资料。将该调度器应用到 ucore 的调度器框架中来，则需要将调度器接口实现如下：
>
>- init:
>    – 初始化调度器类的信息（如果有的话）。
>    – 初始化当前的运行队列为一个空的容器结构。（比如和RR调度算法一样，初始化为一个有序列表）
>- enqueue
>    – 初始化刚进入运行队列的进程 proc的stride属性。
>    – 将 proc插入放入运行队列中去（注意：这里并不要求放置在队列头部）。
>- dequeue
>    – 从运行队列中删除相应的元素。
>- pick next
>    – 扫描整个运行队列，返回其中stride值最小的对应进程。
>    – 更新对应进程的stride值，即pass = BIG_STRIDE / P->priority; P->stride += pass。
>- proc tick:
>    – 检测当前进程是否已用完分配的时间片。如果时间片用完，应该正确设置进程结构的相关标记来引起进程切换。
>    – 一个 process 最多可以连续运行 rq.max_time_slice个时间片。

ucore中采用斜堆的结构来表示Stride调度算法的优先队列，这是一种左偏树

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1gidw2cm9e8j31ah0ff75n.jpg)

将stride作为斜堆的健值，则两个斜堆的合并 对应 将进程插入优先队列，根永远都是最小的stride，即下一个调度的进程



用default_sched_stride_c覆盖default_sched.c

补充各函数

```c
#include <defs.h>
#include <list.h>
#include <proc.h>
#include <assert.h>
#include <default_sched.h>

#define USE_SKEW_HEAP 1

/* You should define the BigStride constant here*/
/* LAB6: YOUR CODE */
#define BIG_STRIDE (((unsigned int)-1)/2)   /* you should give a value, and is ??? */
//将32位整型中最大的数作为BIG_STRIDE

/* The compare function for two skew_heap_node_t's and the
 * corresponding procs*/
static int
proc_stride_comp_f(void *a, void *b)
{
     struct proc_struct *p = le2proc(a, lab6_run_pool);
     struct proc_struct *q = le2proc(b, lab6_run_pool);
     int32_t c = p->lab6_stride - q->lab6_stride;
     if (c > 0) return 1;
     else if (c == 0) return 0;
     else return -1;
}

/*
 * stride_init initializes the run-queue rq with correct assignment for
 * member variables, including:
 *
 *   - run_list: should be a empty list after initialization.
 *   - lab6_run_pool: NULL
 *   - proc_num: 0
 *   - max_time_slice: no need here, the variable would be assigned by the caller.
 *
 * hint: see libs/list.h for routines of the list structures.
 */
static void
stride_init(struct run_queue *rq) {
     /* LAB6: YOUR CODE 
      * (1) init the ready process list: rq->run_list
      * (2) init the run pool: rq->lab6_run_pool
      * (3) set number of process: rq->proc_num to 0       
      */
    list_init(&(rq->run_list));//初始化进程列表
    rq->lab6_run_pool = NULL;//初始化运行队列
    rq->proc_num = 0;//初始化队列中的进程数
}

/*
 * stride_enqueue inserts the process ``proc'' into the run-queue
 * ``rq''. The procedure should verify/initialize the relevant members
 * of ``proc'', and then put the ``lab6_run_pool'' node into the
 * queue(since we use priority queue here). The procedure should also
 * update the meta date in ``rq'' structure.
 *
 * proc->time_slice denotes the time slices allocation for the
 * process, which should set to rq->max_time_slice.
 * 
 * hint: see libs/skew_heap.h for routines of the priority
 * queue structures.
 */
static void
stride_enqueue(struct run_queue *rq, struct proc_struct *proc) {
     /* LAB6: YOUR CODE 
      * (1) insert the proc into rq correctly
      * NOTICE: you can use skew_heap or list. Important functions
      *         skew_heap_insert: insert a entry into skew_heap
      *         list_add_before: insert  a entry into the last of list   
      * (2) recalculate proc->time_slice
      * (3) set proc->rq pointer to rq
      * (4) increase rq->proc_num
      */
    rq->lab6_run_pool = skew_heap_insert(rq->lab6_run_pool, &(proc->lab6_run_pool), proc_stride_comp_f);
  //将proc插入优先队列中
  //skew_heap_insert函数返回斜堆根节点，即stride最小的进程
    if (proc->time_slice == 0 || proc->time_slice > rq->max_time_slice) {
          proc->time_slice = rq->max_time_slice;
    }
  //如果时间片为0（即刚初始化）或者大于最大时间片，则将其设为最大时间片
    proc->rq = rq;
    rq->proc_num ++;
}

/*
 * stride_dequeue removes the process ``proc'' from the run-queue
 * ``rq'', the operation would be finished by the skew_heap_remove
 * operations. Remember to update the ``rq'' structure.
 *
 * hint: see libs/skew_heap.h for routines of the priority
 * queue structures.
 */
static void
stride_dequeue(struct run_queue *rq, struct proc_struct *proc) {
     /* LAB6: YOUR CODE 
      * (1) remove the proc from rq correctly
      * NOTICE: you can use skew_heap or list. Important functions
      *         skew_heap_remove: remove a entry from skew_heap
      *         list_del_init: remove a entry from the  list
      */
    rq->lab6_run_pool = skew_heap_remove(rq->lab6_run_pool, &(proc->lab6_run_pool), proc_stride_comp_f);
  //从优先队列中删去所选进程
  //skew_heap_remove函数返回斜堆根节点，即stride最小的进程
    rq->proc_num --;
}
/*
 * stride_pick_next pick the element from the ``run-queue'', with the
 * minimum value of stride, and returns the corresponding process
 * pointer. The process pointer would be calculated by macro le2proc,
 * see kern/process/proc.h for definition. Return NULL if
 * there is no process in the queue.
 *
 * When one proc structure is selected, remember to update the stride
 * property of the proc. (stride += BIG_STRIDE / priority)
 *
 * hint: see libs/skew_heap.h for routines of the priority
 * queue structures.
 */
static struct proc_struct *
stride_pick_next(struct run_queue *rq) {
     /* LAB6: YOUR CODE 
      * (1) get a  proc_struct pointer p  with the minimum value of stride
             (1.1) If using skew_heap, we can use le2proc get the p from rq->lab6_run_poll
             (1.2) If using list, we have to search list to find the p with minimum stride value
      * (2) update p;s stride value: p->lab6_stride
      * (3) return p
      */
    if(rq->lab6_run_pool == NULL){
        return NULL;
    }
  //若优先队列为空则返回NULL
  //否则选择一个进程，作为调度去占用cpu的进程（这个进程就是斜堆的根节点）
    else{
        struct proc_struct *p = le2proc(rq->lab6_run_pool, lab6_run_pool);
        
        if (p->lab6_priority == 0)
            p->lab6_stride += BIG_STRIDE;
        else 
            p->lab6_stride += BIG_STRIDE / p->lab6_priority;
  //将选择的进程的stride加上其对应的步长pass
  //步长pass与优先级priority成反比，故用优先级priority表示步长pass
  //考虑到优先级可能为0的情况，即优先级最低，则步长pass为最大stride——BIG_STRIDE
        return p;
    }
}

/*
 * stride_proc_tick works with the tick event of current process. You
 * should check whether the time slices for current process is
 * exhausted and update the proc struct ``proc''. proc->time_slice
 * denotes the time slices left for current
 * process. proc->need_resched is the flag variable for process
 * switching.
 */
static void
stride_proc_tick(struct run_queue *rq, struct proc_struct *proc) {
     /* LAB6: YOUR CODE */
  //时间中断部分与RR算法无区别
    if (proc->time_slice > 0) {
        proc->time_slice --;
    }
    if (proc->time_slice == 0) {
        proc->need_resched = 1;
    }
}

struct sched_class default_sched_class = {
     .name = "stride_scheduler",
     .init = stride_init,
     .enqueue = stride_enqueue,
     .dequeue = stride_dequeue,
     .pick_next = stride_pick_next,
     .proc_tick = stride_proc_tick,
};
```

