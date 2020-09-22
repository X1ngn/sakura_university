## 知识点

### 死锁

由于竞争资源或者通信关系，两个或更多线程在执行中出现，永远相互等待的现象

#### 出现死锁的必要条件

- 互斥
- 持有资源并等待
- 非抢占（资源只能在进程使用后自愿释放）
- 循环等待



### 银行家算法

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1gikj15jet5j31820mwaq7.jpg)

银行家算法需要维护的信息：

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1gikj25ecw1j31820mw7kv.jpg)

安全判断：

对于所有的数据进行演算

（**当前剩余资源(Available)**如果可以满足已经分配过资源的线程**未来需要的资源(Need)**，则表示可以在该线程用完后把**已经分配给该线程的资源(Allocation)**回收）

寻找**未来需要资源(Need)**比**当前剩余资源(Available)**少，并且没运行完的线程，如果可以找到，则该进程标记为运行结束，并回收该线程**已经分配的资源(Allocation)**，然后继续寻找下一个这样的线程

到最后如果所有线程都结束，则代表当前状态是安全的，如果有线程不能成功结束，则不安全

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1gikjv8u2opj317y0mynds.jpg)

在线程向操作系统申请资源的时候，判断：

1. 申请的资源量未超过允许申请的最大量，否则拒绝申请
2. 申请的资源量未超过当前剩余的资源，否则线程进入等待
3. 假定已经将资源分配给该线程，进行安全状态判断，如果安全，将资源分配给线程，否则拒绝资源请求

结合例子比较容易理解

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1gikk3rtbcij31800n0qgg.jpg)

（Ti代表线程，Ri代表每种资源的量）

当前可用资源能满足T2线程未来需要的资源，于是回收T2已经分配的资源

当前可用资源变为

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1gikkaq2zk0j307o046mxq.jpg)

以此类推，这种状态是安全的



### 死锁检测

- 允许系统进入死锁状态
- 维护系统的资源分配图
- 定期调用死锁检测算法，搜索图中是否存在死锁
- 出现死锁时，用死锁恢复机制进行恢复

死锁检测算法与银行家算法类似，只是检测时已经完成了资源分配

死锁恢复

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1giknsrzjz8j31820mkwrw.jpg)

选择被抢占进程后，进程回退，返回一些安全状态重新启动进程执行（可能出现饥饿，同一进程一直被选作被抢占者）



### 进程通信(IPC)

IPC提供两个基本操作：发送和接收

#### 进程通信流程

- 在通信进程间建立通信链路
- 同个两个操作交换信息

#### 通信链路特征

- 物理（如共享内存、硬件总线）
- 逻辑（如逻辑属性）

#### 通信方式

- 直接通信：在两个进程间建立通讯信道（共享信道），直接向共享信道发送或从共享信道读取数据

    ![](https://tva1.sinaimg.cn/large/007S8ZIlgy1gikpnkxd92j317y0m4gys.jpg)

    

- 间接通信：建立消息队列，进程A将信息发送到内核的消息队列上，另一个进程从其中读取（生命周期可以不一样）

    通信流程

    - 创建一个新的消息队列
    - 通过消息队列发送和接收消息
    - 销毁消息队列

    通信操作时所描述的对象是消息队列，发方并不关心收方是谁，收方也不关心发方是谁

#### 阻塞与非阻塞通信

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1gikq0yvb5ej317y0mgtk7.jpg)

#### 通信链路缓冲

- 0容量——发送方必须等待接收方接收才能继续发送
- 有限容量——通信链路缓冲区满时发送方必须等待接收方接收才能继续发送
- 无限容量——发送方不需要等待



### 进程间通讯机制的具体实现

#### 信号

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1gikq9z923wj31820mewua.jpg)

#### 管道

- 进程间基于内存文件的通讯机制
    - 子进程从父进程继承文件描述符
    - 缺省的文件描述符：0、1、2
- 进程并不关心管道另一段是谁

（linux命令中用“|”隔开命令即在两个命令间建立管道，如`ls | more`）

#### 消息队列

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1gikqmo29nxj317q0nih06.jpg)

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1gikqmsiwjwj31820mitv1.jpg)

#### 共享内存

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1gikqzd7jztj31800n24ea.jpg)

共享内存的实现：通过各自的页表项，将一个物理内存区域映射到两个进程



### 文件系统和文件

文件系统是操作系统中管理持久性数据的子系统，提供数据存储和访问功能

文件是具有符号名，由字节序列构成的数据项集合

### 文件描述符

打开的文件在内存中所维护的信息，是打开文件的标识

操作系统为每个进程维护一个打开文件表，对应着文件描述符

打开文件表表项内容：

- 文件指针——最近一次读写的位置，每个进程分别维护自己的文件指针
- 文件打开计数——当前打开文件的次数，最后一个进程关闭时将该文件从打开文件表中删除
- 文件的磁盘位置——缓存数据访问信息
- 访问权限

### 文件的用户视图和系统视图

#### 文件的用户视图

- 持久的数据结构

#### 系统访问接口

- 字节序列的集合（UNIX）
- 系统不关心存储在磁盘上的数据结构

#### 文件的系统视图

- 数据块的集合
- 数据块是逻辑存储单元，而扇区是物理存储单元
- 块大小不等于扇区大小

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1gikevyghojj317w0n27jb.jpg)

#### 访问模式

- 顺序访问
- 随机访问
- 索引访问

多用户系统中的文件共享

每个权限对应的每个用户，存放在文件访问控制列表(ACL)中

（UNIX中标识每个**用户**所允许的权限，并且允许用户组成**组**，并制定组权限）



### 语义一致性

#### UNIX文件系统语义

- 对打开文件的写入内容立即对其他打开同一文件的其他用户可见
- 共享文件指针允许多用户同时读取和写入文件

#### 会话语义

- 写入的内容只有文件关闭时其他用户才可见

#### 读写锁

- 操作系统提供几种基本的互斥访问锁，由用户进程选择需要什么样的同步互斥来保证内容的一致性



### 目录

目录的内容是文件索引表，<文件名，指向文件的指针>

操作系统应该只允许内核修改目录，应用程序通过系统调用修改目录

#### 目录的实现

- 文件名的线性列表，包含了指向数据块的指针
- 哈希表

#### 文件别名

- 硬链接：多个文件项指向一个文件
- 软链接：以“快捷方式”指向其他文件——通过存储真实文件的逻辑名称（完整路径）来实现

#### 名字解析

![](https://tva1.sinaimg.cn/large/007S8ZIlly1giki2qs70zj317w0n2h5d.jpg)

#### 文件系统挂载

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1giki5dmvprj31840msn8u.jpg)

将文件系统对应到根文件系统中的某一个目录

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1giki6bz0hhj317u0mytk1.jpg)

#### 文件系统种类

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1giki9891y5j31800mok6b.jpg)



### 虚拟文件系统

多种不同的文件系统可以通过虚拟文件系统对用户提供统一接口

相关数据结构

- 文件卷数据块(UNIX:"superblock")
    - 每个文件系统一个
    - 文件系统的详细信息
    - 块大小、已分配的块、空余块、各种计数/指针
- 文件控制块(UNIX:"inode")
    - 每个文件一个
    - 文件详细信息
    - 访问权限、拥有者、大小、数据块位置
- 目录项(UNIX:"dentry")
    - 每个目录项一个（目录和文件）
    - 将目录项数据结构及树型布局编码成树型数据结构
    - 指向文件控制块、父目录、子目录等



### 文件缓存和打开文件管理

#### 文件缓存

- 磁盘块缓存（数据块缓存）

    页缓存和文件读写都可能有磁盘块缓存，访问时先访问磁盘块缓存

    ![](https://tva1.sinaimg.cn/large/007S8ZIlgy1gilku9plqwj317o0n2thm.jpg)

- 页缓存

    ![](https://tva1.sinaimg.cn/large/007S8ZIlgy1gill1cmozkj317y0n84eh.jpg)

    ![](https://tva1.sinaimg.cn/large/007S8ZIlgy1gill1e0k5aj30n60dmjy9.jpg)

#### 打开文件的数据结构

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1ginu9503bij31820my4di.jpg)

#### 打开文件锁

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1ginubveyt1j31800n0k2p.jpg)



### 文件分配

再分配数据块的时候，需要对小文件和大文件都有很好的支持

如何表示分配给一个文件数据块的位置和顺序？

- 连续分配

    ![](https://tva1.sinaimg.cn/large/007S8ZIlly1gitcvfprrpj31800n6qel.jpg)

- 链式分配

    ![](https://tva1.sinaimg.cn/large/007S8ZIlly1gitcvrz0v1j317w0myn9u.jpg)

- 索引分配（分配一个块存储序号，记录保存数据的数据块及其顺序）

    ![](https://tva1.sinaimg.cn/large/007S8ZIlly1gitcvwqy1qj31840mgayz.jpg)

UFS多级索引分配

前十个数据块采用直接索引的方式

当十个数据块不足以存储文件的时候，第十一个数据块作为一级索引，用于保存其他数据块的地址

当一级索引不足以存储文件的时候，第十二个数据块作为二级索引，使用二级索引时不使用一级索引，以此类推

![](https://tva1.sinaimg.cn/large/007S8ZIlly1giy2z66xs5j31800n2dso.jpg)



### 空闲空间管理一般采用位图、链式索引等方式组织



## lab8

### 练习0

>#### 练习0：填写已有实验
>
>本实验依赖实验1/2/3/4/5/6/7。请把你做的实验1/2/3/4/5/6/7的代码填入本实验中代码中有“LAB1”/“LAB2”/“LAB3”/“LAB4”/“LAB5”/“LAB6” /“LAB7”的注释相应部分。并确保编译通过。注意：为了能够正确执行lab8的测试应用程序，可能需对已完成的实验1/2/3/4/5/6/7的代码进行进一步改进。



### 练习1

>#### 练习1: 完成读文件操作的实现（需要编码）
>
>首先了解打开文件的处理流程，然后参考本实验后续的文件读写操作的过程分析，编写在sfs_inode.c中sfs_io_nolock读文件中数据的实现代码。
>
>请在实验报告中给出设计实现”UNIX的PIPE机制“的概要设方案，鼓励给出详细设计方案

补充sfs_io_nolock函数

写不来了抄了答案的

在需要补充部分：blkno可以理解为目前要读取的文件首个数据块的index，endpos是读取结尾的地址，则nblks可以理解为还需要读取的数据块数目

```c
/*  
 * sfs_io_nolock - Rd/Wr a file contentfrom offset position to offset+ length  disk blocks<-->buffer (in memroy)
 * @sfs:      sfs file system
 * @sin:      sfs inode in memory
 * @buf:      the buffer Rd/Wr
 * @offset:   the offset of file
 * @alenp:    the length need to read (is a pointer). and will RETURN the really Rd/Wr lenght
 * @write:    BOOL, 0 read, 1 write
 */
static int
sfs_io_nolock(struct sfs_fs *sfs, struct sfs_inode *sin, void *buf, off_t offset, size_t *alenp, bool write) {
    struct sfs_disk_inode *din = sin->din;
    assert(din->type != SFS_TYPE_DIR);
    off_t endpos = offset + *alenp, blkoff;
    *alenp = 0;
	// calculate the Rd/Wr end position
    if (offset < 0 || offset >= SFS_MAX_FILE_SIZE || offset > endpos) {
        return -E_INVAL;
    }
    if (offset == endpos) {
        return 0;
    }
    if (endpos > SFS_MAX_FILE_SIZE) {
        endpos = SFS_MAX_FILE_SIZE;
    }
    if (!write) {
        if (offset >= din->size) {
            return 0;
        }
        if (endpos > din->size) {
            endpos = din->size;
        }
    }

    int (*sfs_buf_op)(struct sfs_fs *sfs, void *buf, size_t len, uint32_t blkno, off_t offset);
    int (*sfs_block_op)(struct sfs_fs *sfs, void *buf, uint32_t blkno, uint32_t nblks);
    if (write) {
        sfs_buf_op = sfs_wbuf, sfs_block_op = sfs_wblock;
    }
    else {
        sfs_buf_op = sfs_rbuf, sfs_block_op = sfs_rblock;
    }

    int ret = 0;
    size_t size, alen = 0;
    uint32_t ino;
    uint32_t blkno = offset / SFS_BLKSIZE;          // The NO. of Rd/Wr begin block
    uint32_t nblks = endpos / SFS_BLKSIZE - blkno;  // The size of Rd/Wr blocks

  //LAB8:EXERCISE1 YOUR CODE HINT: call sfs_bmap_load_nolock, sfs_rbuf, sfs_rblock,etc. read different kind of blocks in file
	/*
	 * (1) If offset isn't aligned with the first block, Rd/Wr some content from offset to the end of the first block
	 *       NOTICE: useful function: sfs_bmap_load_nolock, sfs_buf_op
	 *               Rd/Wr size = (nblks != 0) ? (SFS_BLKSIZE - blkoff) : (endpos - offset)
	 * (2) Rd/Wr aligned blocks 
	 *       NOTICE: useful function: sfs_bmap_load_nolock, sfs_block_op
     * (3) If end position isn't aligned with the last block, Rd/Wr some content from begin to the (endpos % SFS_BLKSIZE) of the last block
	 *       NOTICE: useful function: sfs_bmap_load_nolock, sfs_buf_op	
	*/
  //sfs_io_nolock函数“可能”分三部分读取文件，第一部分数据块，中间部分的数据块，最后部分数据块
    if ((blkoff = offset % SFS_BLKSIZE) != 0) {
      //blkoff是数据块内偏移，如果不是0，则说明需要读取的部分不是与数据块对齐的
      //所以将从开始offset处到下一个数据块对齐的部分作为第一部分读取
        size = (nblks != 0) ? (SFS_BLKSIZE - blkoff) : (endpos - offset);//确定读取的大小
      //1.如果读取完该数据块后还需要读取其他数据块，则将前面不对齐的部分作为第一部分
      //2.如果读取完该数据块后不需要读取其他数据块，则所有需要读取的数据块就只是这部分
        if ((ret = sfs_bmap_load_nolock(sfs, sin, blkno, &ino)) != 0) {
            goto out;
        }//获取当前需要读取的数据块index
        if ((ret = sfs_buf_op(sfs, buf, size, ino, blkoff)) != 0) {
            goto out;
        }//完成实际的读写操作
        alen += size;
        if (nblks == 0) {//如果是第二种情况，则读取完毕，直接返回
            goto out;
        }
        buf += size, blkno ++, nblks --;
    }

    //读取中间部分的数据，将其分为size大小的块，然后一次读一块直至读完
    size = SFS_BLKSIZE;
    while (nblks != 0) {
        if ((ret = sfs_bmap_load_nolock(sfs, sin, blkno, &ino)) != 0) {
            goto out;
        }
        if ((ret = sfs_block_op(sfs, buf, ino, 1)) != 0) {
            goto out;
        }
        alen += size, buf += size, blkno ++, nblks --;
    } 
    //如果结尾的地址不与数据块大小对齐，则作为最后部分的数据块读取
    if ((size = endpos % SFS_BLKSIZE) != 0) {
        if ((ret = sfs_bmap_load_nolock(sfs, sin, blkno, &ino)) != 0) {
            goto out;
        }
        if ((ret = sfs_buf_op(sfs, buf, size, ino, 0)) != 0) {
            goto out;
        }
        alen += size;
    }
    
out:
    *alenp = alen;
    if (offset + alen > sin->din->size) {
        sin->din->size = offset + alen;
        sin->dirty = 1;
    }
    return ret;
}
```

- 请在实验报告中给出设计实现”UNIX的PIPE机制“的概要设方案，鼓励给出详细设计方案



### 练习2

>#### 练习2: 完成基于文件系统的执行程序机制的实现（需要编码）
>
>改写proc.c中的load_icode函数和其他相关函数，实现基于文件系统的执行程序机制。执行：make qemu。如果能看看到sh用户程序的执行界面，则基本成功了。如果在sh用户界面上可以执行”ls”,”hello”等其他放置在sfs文件系统中的其他执行程序，则可以认为本实验基本成功。
>
>请在实验报告中给出设计实现基于”UNIX的硬链接和软链接机制“的概要设方案，鼓励给出详细设计方案

首先根据注释在alloc_proc函数中补充PCB中对fs的初始化

加一句`proc->filesp = NULL;`即可

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
    //LAB8:EXERCISE2 YOUR CODE HINT:need add some code to init fs in proc_struct, ...
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
      proc->rq = NULL;
      list_init(&(proc->run_link));
      proc->time_slice = 0;
      proc->lab6_run_pool.left = proc->lab6_run_pool.right = proc->lab6_run_pool.parent = NULL;
      proc->lab6_stride = 0;
      proc->lab6_priority = 0;

      proc->filesp = NULL;//更新，初始化fs中的进程控制结构
    }
    return proc;
}
```

然后根据注释在do_fork函数中补充使用copy_fs函数复制父进程的fs到子进程中

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
    //LAB8:EXERCISE2 YOUR CODE  HINT:how to copy the fs in parent's proc_struct?
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
    proc->parent = current;//设置父进程
    assert(current->wait_state == 0);

    if (setup_kstack(proc) != 0){//分配内核栈地址（由于内核空间使用同一个内核栈，所以实际上不需要这一步）
        goto bad_fork_cleanup_proc;
    }
    
    if (copy_fs(clone_flags, proc) != 0){//更新，使用copy_fs复制父进程的fs到子进程中
        goto bad_fork_cleanup_kstack;
    }
    if (copy_mm(clone_flags, proc) != 0){//复制父进程的内存
        goto bad_fork_cleanup_kstack;
    }

    copy_thread(proc, stack, tf);//复制中断帧和上下文信息

    
    bool intr_flag;//按照参考答案 以下操作需要屏蔽中断进行
    local_intr_save(intr_flag); //屏蔽中断
    proc->pid = get_pid();//获取pid，在下面建立hash映射和插入进程链表需要用到
    hash_proc(proc);
    //list_add(&proc_list, &(proc->list_link));
    set_links(proc);
    local_intr_restore(intr_flag);//恢复中断

    wakeup_proc(proc);//唤醒进程，将其放在就绪队列
    
    ret = proc->pid;//返回进程pid
fork_out:
    return ret;

bad_fork_cleanup_fs:  //for LAB8
    put_files(proc);
bad_fork_cleanup_kstack:
    put_kstack(proc);
bad_fork_cleanup_proc:
    kfree(proc);
    goto fork_out;
}
```

补充load_icode函数

写不来了抄了答案的

```c
static int
load_icode(int fd, int argc, char **kargv) {
    /* LAB8:EXERCISE2 YOUR CODE  HINT:how to load the file with handler fd  in to process's memory? how to setup argc/argv?
     * MACROs or Functions:
     *  mm_create        - create a mm
     *  setup_pgdir      - setup pgdir in mm
     *  load_icode_read  - read raw data content of program file
     *  mm_map           - build new vma
     *  pgdir_alloc_page - allocate new memory for  TEXT/DATA/BSS/stack parts
     *  lcr3             - update Page Directory Addr Register -- CR3
     */
	/* (1) create a new mm for current process
     * (2) create a new PDT, and mm->pgdir= kernel virtual addr of PDT
     * (3) copy TEXT/DATA/BSS parts in binary to memory space of process
     *    (3.1) read raw data content in file and resolve elfhdr
     *    (3.2) read raw data content in file and resolve proghdr based on info in elfhdr
     *    (3.3) call mm_map to build vma related to TEXT/DATA
     *    (3.4) callpgdir_alloc_page to allocate page for TEXT/DATA, read contents in file
     *          and copy them into the new allocated pages
     *    (3.5) callpgdir_alloc_page to allocate pages for BSS, memset zero in these pages
     * (4) call mm_map to setup user stack, and put parameters into user stack
     * (5) setup current process's mm, cr3, reset pgidr (using lcr3 MARCO)
     * (6) setup uargc and uargv in user stacks
     * (7) setup trapframe for user environment
     * (8) if up steps failed, you should cleanup the env.
     */
    assert(argc >= 0 && argc <= EXEC_MAX_ARG_NUM);

    if (current->mm != NULL) {
        panic("load_icode: current->mm must be empty.\n");
    }

    int ret = -E_NO_MEM;
    struct mm_struct *mm;

    //(1) create a new mm for current process
    if ((mm = mm_create()) == NULL) {
        goto bad_mm;
    }
    //(2) create a new PDT, and mm->pgdir= kernel virtual addr of PDT
    if (setup_pgdir(mm) != 0) {
        goto bad_pgdir_cleanup_mm;
    }

    //(3) copy TEXT/DATA/BSS parts in binary to memory space of process
    struct Page *page;
    
    struct elfhdr __elf, *elf = &__elf;
    if ((ret = load_icode_read(fd, elf, sizeof(struct elfhdr), 0)) != 0) {
        goto bad_elf_cleanup_pgdir;
    }//读取elf文件头
    
    if (elf->e_magic != ELF_MAGIC) {
        ret = -E_INVAL_ELF;
        goto bad_elf_cleanup_pgdir;
    }
    
    struct proghdr __ph, *ph = &__ph;
    uint32_t vm_flags, perm, phnum;
    for (phnum = 0; phnum < elf->e_phnum; phnum ++) {
      //e_phnum是段入口地址数目
        off_t phoff = elf->e_phoff + sizeof(struct proghdr) * phnum;//循环读取程序的每个段的头部
        if ((ret = load_icode_read(fd, ph, sizeof(struct proghdr), phoff)) != 0) {
            goto bad_cleanup_mmap;
        }
        if (ph->p_type != ELF_PT_LOAD) {
            continue ;
        }
        if (ph->p_filesz > ph->p_memsz) {
            ret = -E_INVAL_ELF;
            goto bad_cleanup_mmap;
        }
        if (ph->p_filesz == 0) {
            continue ;
        }

        //call mm_map建立TEXT/DATA虚拟地址与物理地址之间的映射
        vm_flags = 0, perm = PTE_U;
        if (ph->p_flags & ELF_PF_X) vm_flags |= VM_EXEC;
        if (ph->p_flags & ELF_PF_W) vm_flags |= VM_WRITE;
        if (ph->p_flags & ELF_PF_R) vm_flags |= VM_READ;
        if (vm_flags & VM_WRITE) perm |= PTE_W;
        if ((ret = mm_map(mm, ph->p_va, ph->p_memsz, vm_flags, NULL)) != 0) {
            goto bad_cleanup_mmap;
        }
        off_t offset = ph->p_offset;
        size_t off, size;
        uintptr_t start = ph->p_va, end, la = ROUNDDOWN(start, PGSIZE);

        ret = -E_NO_MEM;

        ////复制数据段和代码段
        end = ph->p_va + ph->p_filesz;//计算数据段和代码段终止地址
        while (start < end) {
            if ((page = pgdir_alloc_page(mm->pgdir, la, perm)) == NULL) {
                ret = -E_NO_MEM;
                goto bad_cleanup_mmap;
            }//申请页用于数据段和代码段
            off = start - la, size = PGSIZE - off, la += PGSIZE;
            if (end < la) {
                size -= la - end;
            }
            //每次读取size大小的块，直至全部读完
            if ((ret = load_icode_read(fd, page2kva(page) + off, size, offset)) != 0) {
                goto bad_cleanup_mmap;
            }
            start += size, offset += size;
        }

        //建立BSS段
        end = ph->p_va + ph->p_memsz;
        if (start < la) {
            /* ph->p_memsz == ph->p_filesz */
            if (start == end) {
                continue ;
            }
            off = start + PGSIZE - la, size = PGSIZE - off;
            if (end < la) {
                size -= la - end;
            }
            memset(page2kva(page) + off, 0, size);
            start += size;
            assert((end < la && start == end) || (end >= la && start == la));
        }
        while (start < end) {
            if ((page = pgdir_alloc_page(mm->pgdir, la, perm)) == NULL) {
                ret = -E_NO_MEM;
                goto bad_cleanup_mmap;
            }
            off = start - la, size = PGSIZE - off, la += PGSIZE;
            if (end < la) {
                size -= la - end;
            }
            memset(page2kva(page) + off, 0, size);
            start += size;
        }
    }
    sysfile_close(fd);//关闭文件，加载程序结束

    //(4) call mm_map to setup user stack, and put parameters into user stack
    vm_flags = VM_READ | VM_WRITE | VM_STACK;
    if ((ret = mm_map(mm, USTACKTOP - USTACKSIZE, USTACKSIZE, vm_flags, NULL)) != 0) {
        goto bad_cleanup_mmap;
    }
    assert(pgdir_alloc_page(mm->pgdir, USTACKTOP-PGSIZE , PTE_USER) != NULL);
    assert(pgdir_alloc_page(mm->pgdir, USTACKTOP-2*PGSIZE , PTE_USER) != NULL);
    assert(pgdir_alloc_page(mm->pgdir, USTACKTOP-3*PGSIZE , PTE_USER) != NULL);
    assert(pgdir_alloc_page(mm->pgdir, USTACKTOP-4*PGSIZE , PTE_USER) != NULL);
    
    //(5) setup current process's mm, cr3, reset pgidr (using lcr3 MARCO)
    mm_count_inc(mm);
    current->mm = mm;
    current->cr3 = PADDR(mm->pgdir);
    lcr3(PADDR(mm->pgdir));

    //(6)处理用户栈中传入的参数，其中argc对应参数个数，uargv[]对应参数的具体内容的地址
    uint32_t argv_size=0, i;
    for (i = 0; i < argc; i ++) {
        argv_size += strnlen(kargv[i],EXEC_MAX_ARG_LEN + 1)+1;
    }

    uintptr_t stacktop = USTACKTOP - (argv_size/sizeof(long)+1)*sizeof(long);
    char** uargv=(char **)(stacktop  - argc * sizeof(char *));
    
    argv_size = 0;
    for (i = 0; i < argc; i ++) {//将所有参数取出来放置uargv
        uargv[i] = strcpy((char *)(stacktop + argv_size ), kargv[i]);
        argv_size +=  strnlen(kargv[i],EXEC_MAX_ARG_LEN + 1)+1;
    }
    
    stacktop = (uintptr_t)uargv - sizeof(int);//计算当前用户栈顶
    *(int *)stacktop = argc;
    
    //(7) setup trapframe for user environment
    struct trapframe *tf = current->tf;
    memset(tf, 0, sizeof(struct trapframe));
    tf->tf_cs = USER_CS;
    tf->tf_ds = tf->tf_es = tf->tf_ss = USER_DS;
    tf->tf_esp = stacktop;
    tf->tf_eip = elf->e_entry;
    tf->tf_eflags = FL_IF;
    ret = 0;

    //(8)返回
out:
    return ret;
bad_cleanup_mmap:
    exit_mmap(mm);
bad_elf_cleanup_pgdir:
    put_pgdir(mm);
bad_pgdir_cleanup_mm:
    mm_destroy(mm);
bad_mm:
    goto out;
}
```

大部分实现还是与lab7一样，在(3)、(6)的地方增加了对文件的操作

但是不知道为啥还是不能正常读取文件，等有时间再研究研究