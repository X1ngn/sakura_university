## 知识点

### 覆盖和交换

覆盖：将一个应用程序划分成若干功能相对独立的模块，将不会同时执行的模块分为一个区域，它们共享一块内存

下图中B、C共享一块内存，D、E、F共享一块内存，只要给每个区域分配最大所需的内存即可

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1ghmrw9q8i9j317q0n00vh.jpg)

交换：将暂时不能运行的程序放到外存（以进程为单位）

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1ghlv28c5s9j31800mutin.jpg)

比较

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1ghmrwjb09fj31820mc168.jpg)

### 虚拟内存技术（利用局部性原理）

- 只把部分程序放到内存中，从而运行比物理内存大的程序（操作系统完成，无需程序员干涉）
- 实现进程在内存外存间的交换，从而获得更多的空闲内存空间

虚拟页式存储中的页表项中需要有

驻留位：表示该页是否在内存

修改位：表示在内存中的该页是否被修改过

访问位：表示该页面是否被访问过（读或写）

保护位：表示该页的允许访问方式

### 缺页异常

- 在内存中有空闲物理页面时，分配一物理页帧f，将需要访问的页面p装入到页帧f

- 若内存中没有空闲物理页面，依据页面置换算法选择将被替换的物理页帧f——对应逻辑页q；

    如果逻辑页q被修改过，则把它写回外存，将q对应的页表项中保留位置0，否则直接将q对应的页表项中保留位置0；

    将需要访问的页p装入到物理页帧f，并将保留位置1；

    重新执行产生缺页的指令

### 外存管理

设置一个交换空间（磁盘或者文件）

代码段——二进制可执行文件

动态加载的共享库程序段——动态调用的库文件

其他段——交换空间

### 置换算法

#### 局部置换算法

- 最优置换算法OPT：理想情况，无法实现，作为其他算法性能评价依据

- 先进先出算法FIFO：即选择在内存中驻留时间最长的页面进行置换

- 最近最久未使用算法LRU：选择最长时间没有被引用的页面进行置换（通过维护链表或栈实现）

- 时钟页面置换算法Clock：

    ![](https://tva1.sinaimg.cn/large/007S8ZIlgy1ghmw11ivvzj317u0mswus.jpg)

    页面装入内存时，访问位初始化为0；

    访问页面（读/写）时，访问位置1；

    缺页时，检查链表，访问位为0，则置换该页，置换后访问位置1，指针指向下一个节点；访问位为1，则访问位置0，继续检查

- 改进时钟页面置换算法：

    处理修改过的页面速度提高

    ![](https://tva1.sinaimg.cn/large/007S8ZIlgy1ghmwt0vf4hj317g0mwna5.jpg)

- 最不常用算法LFU：每个页面设置计数，访问时加一，缺页时置换计数最小的页面

belady现象：分配的物理页面数增加，缺页次数反而增加（如FIFO）

#### 全局置换算法

考虑不同进程对内存需求量的差异，为进程分配可变数目的物理页面

- 工作集置换算法

    工作集：一个进程当前正在使用的逻辑页面集合,表示为W(t,&)（时刻t前&时间中所有访问页面组成的集合，加绝对值是指工作集大小，即页面数目）

    例如，让&为10

    ![](https://tva1.sinaimg.cn/large/007S8ZIlgy1ghog7spf3gj310o0aq77r.jpg)

    W(t1,&) = {1, 2, 5, 6, 7}

    W(t2,&) = {3, 4}

    常驻集：当前时刻驻留在内存中的页面集合，取决于系统分配给进程的物理页面数和页面置换算法

    工作集是进程运行过程中的一种性质，页面置换算法操作的是常驻集

    ![](https://tva1.sinaimg.cn/large/007S8ZIlgy1ghogjzux4nj311s0fegup.jpg)



## lab3

### 练习0

>#### 练习0：填写已有实验
>
>本实验依赖实验1/2。请把你做的实验1/2的代码填入本实验中代码中有“LAB1”,“LAB2”的注释相应部分。

填写已有实验

### 练习1

>#### 练习1：给未被映射的地址映射上物理页（需要编程）
>
>完成do_pgfault（mm/vmm.c）函数，给未被映射的地址映射上物理页。设置访问权限 的时候需要参考页面所在 VMA 的权限，同时需要注意映射物理页时需要操作内存控制 结构所指定的页表，而不是内核的页表。注意：在LAB3 EXERCISE 1处填写代码。执行
>
>```
>make　qemu
>```
>
>后，如果通过check_pgfault函数的测试后，会有“check_pgfault() succeeded!”的输出，表示练习1基本正确。
>
>请在实验报告中简要说明你的设计实现过程。请回答如下问题：
>
>- 请描述页目录项（Page Directory Entry）和页表项（Page Table Entry）中组成部分对ucore实现页替换算法的潜在用处。
>- 如果ucore的缺页服务例程在执行过程中访问内存，出现了页访问异常，请问硬件要做哪些事情？

产生页访问异常的原因主要有：

- 目标页帧不存在（页表项全为0，即该线性地址与物理地址尚未建立映射或者已经撤销)；
- 相应的物理页帧不在内存中（页表项非空，但Present标志位=0，比如在swap分区或磁盘文件上)，这在本次实验中会出现，我们将在下面介绍换页机制实现时进一步讲解如何处理；
- 不满足访问权限(此时页表项P标志=1，但低权限的程序试图访问高权限的地址空间，或者有程序试图写只读页面).

当出现上面情况之一，那么就会产生页面page fault（#PF）异常。CPU会把产生异常的线性地址存储在CR2中，并且把表示页访问异常类型的值（简称页访问异常错误码，errorCode）保存在中断栈中。

根据注释，先获取当前发生缺页的虚拟页对应的PTE，可以通过get_pte函数实现

如果这个物理地址没有分配，即该页目录表项全为0，则申请一个页并将物理地址映射到虚拟地址，可以通过pgdir_alloc_page函数实现

两个函数都定义在pmm.c中

```c
int
do_pgfault(struct mm_struct *mm, uint32_t error_code, uintptr_t addr) {
    int ret = -E_INVAL;
    //try to find a vma which include addr
    struct vma_struct *vma = find_vma(mm, addr);

    pgfault_num++;
    //If the addr is in the range of a mm's vma?
    if (vma == NULL || vma->vm_start > addr) {
        cprintf("not valid addr %x, and  can not find it in vma\n", addr);
        goto failed;
    }
    //check the error_code
    switch (error_code & 3) {
    default:
            /* error code flag : default is 3 ( W/R=1, P=1): write, present */
    case 2: /* error code flag : (W/R=1, P=0): write, not present */
        if (!(vma->vm_flags & VM_WRITE)) {
            cprintf("do_pgfault failed: error code flag = write AND not present, but the addr's vma cannot write\n");
            goto failed;
        }
        break;
    case 1: /* error code flag : (W/R=0, P=1): read, present */
        cprintf("do_pgfault failed: error code flag = read AND present\n");
        goto failed;
    case 0: /* error code flag : (W/R=0, P=0): read, not present */
        if (!(vma->vm_flags & (VM_READ | VM_EXEC))) {
            cprintf("do_pgfault failed: error code flag = read AND not present, but the addr's vma cannot read or exec\n");
            goto failed;
        }
    }
    /* IF (write an existed addr ) OR
     *    (write an non_existed addr && addr is writable) OR
     *    (read  an non_existed addr && addr is readable)
     * THEN
     *    continue process
     */
    uint32_t perm = PTE_U;
    if (vma->vm_flags & VM_WRITE) {
        perm |= PTE_W;
    }
    addr = ROUNDDOWN(addr, PGSIZE);

    ret = -E_NO_MEM;

    pte_t *ptep=NULL;
    /*LAB3 EXERCISE 1: YOUR CODE
    * Maybe you want help comment, BELOW comments can help you finish the code
    *
    * Some Useful MACROs and DEFINEs, you can use them in below implementation.
    * MACROs or Functions:
    *   get_pte : get an pte and return the kernel virtual address of this pte for la
    *             if the PT contians this pte didn't exist, alloc a page for PT (notice the 3th parameter '1')
    *   pgdir_alloc_page : call alloc_page & page_insert functions to allocate a page size memory & setup
    *             an addr map pa<--->la with linear address la and the PDT pgdir
    * DEFINES:
    *   VM_WRITE  : If vma->vm_flags & VM_WRITE == 1/0, then the vma is writable/non writable
    *   PTE_W           0x002                   // page table/directory entry flags bit : Writeable
    *   PTE_U           0x004                   // page table/directory entry flags bit : User can access
    * VARIABLES:
    *   mm->pgdir : the PDT of these vma
    *
    */
  #if 0
    /*LAB3 EXERCISE 1: YOUR CODE*/
    ptep = ???              //(1) try to find a pte, if pte's PT(Page Table) isn't existed, then create a PT.
    if (*ptep == 0) {
                            //(2) if the phy addr isn't exist, then alloc a page & 

    }
    else {
    }
  #endif
    ptep = get_pte(mm->pgdir, addr, 1);
    if(*ptep == 0){
        pgdir_alloc_page(mm->pgdir, addr, perm);
    }
    else{
        //练习2
    }
    ret = 0;


failed:
    return ret;
}
```

- 请描述页目录项（Page Directory Entry）和页表项（Page Table Entry）中组成部分对ucore实现页替换算法的潜在用处。

    页目录项高20位表示下一级页表（或物理页表）的基址（4K对齐），页表项高20位表示页基址

    其低12位为标志位：

    ```c
    /* page table/directory entry flags */
    #define PTE_P           0x001                   // Present
    #define PTE_W           0x002                   // Writeable
    #define PTE_U           0x004                   // User
    #define PTE_PWT         0x008                   // Write-Through
    #define PTE_PCD         0x010                   // Cache-Disable
    #define PTE_A           0x020                   // Accessed
    #define PTE_D           0x040                   // Dirty
    #define PTE_PS          0x080                   // Page Size
    #define PTE_MBZ         0x180                   // Bits must be zero
    #define PTE_AVAIL       0xE00                   // Available for software use
                                                    // The PTE_AVAIL bits aren't used by the kernel or interpreted by the
                                                    // hardware, so user processes are allowed to set them arbitrarily.
    ```

    对于PDE，第7位PTE_PS位为0时，页的大小是4KB；为1时，页的大小是4MB

    对于PTE，第7、8位强制为0

    第9到11位供内核以外的软件使用，ucore可以利用这些位来完成一些其他的内存管理相关的算法（比LRU），也就是说这些保留位有利于OS进行功能的拓展

- 如果ucore的缺页服务例程在执行过程中访问内存，出现了页访问异常，请问硬件要做哪些事情？

    与正常情况发生页访问异常一致

    1. 将发生错误的线性地址保存在cr2寄存器中;

    2. 在中断栈中依次压入EFLAGS，CS, EIP，以及页访问异常码error code（ISR运行在内核态下，不需要压入ss和esp和进行栈的切换）；

    3. 根据中断描述符表查询到对应页访问异常的ISR，跳转到对应的ISR处执行，接下来将由软件进行处理；

    （一般不应该将缺页的ISR以及OS中的其他一些关键代码或者数据换出到外存中，以确保操作系统的正常运行）



### 练习2

>#### 练习2：补充完成基于FIFO的页面替换算法（需要编程）
>
>完成vmm.c中的do_pgfault函数，并且在实现FIFO算法的swap_fifo.c中完成map_swappable和swap_out_victim函数。通过对swap的测试。注意：在LAB3 EXERCISE 2处填写代码。执行
>
>```
>make　qemu
>```
>
>后，如果通过check_swap函数的测试后，会有“check_swap() succeeded!”的输出，表示练习2基本正确。
>
>请在实验报告中简要说明你的设计实现过程。
>
>请在实验报告中回答如下问题：
>
>- 如果要在ucore上实现"extended clock页替换算法"请给你的设计方案，现有的swap_manager框架是否足以支持在ucore中实现此算法？如果是，请给你的设计方案。如果不是，请给出你的新的扩展和基此扩展的设计方案。并需要回答如下问题
>    - 需要被换出的页的特征是什么？
>    - 在ucore中如何判断具有这样特征的页？
>    - 何时进行换入和换出操作？

#### 换入部分

当访问内存产生page fault异常时，可获得访问的内存的方式（读或写）以及具体的虚拟内存地址，这样ucore就可以查询此地址，看是否属于vma_struct数据结构中描述的合法地址范围中，如果在，则可根据具体情况进行请求调页/页换入换出处理（这就是练习2涉及的部分）；如果不在，则报错。

实际操作系统中，只有映射到用户空间且被用户程序直接访问的页面才能被交换，而被内核直接使用的内核空间的页面不能被换出。而在实验三中仅仅通过执行check_swap函数在内核中分配一些页，模拟对这些页的访问，然后通过do_pgfault来调用swap_map_swappable函数来查询这些页的访问情况并间接调用相关函数，换出“不常用”的页到磁盘上。

基于数据结构Page的全局变量pages数组在目前lab可以表示内存中物理页使用情况，为了表示物理页可被换出或已被换出的情况，可对Page数据结构进行扩展：

```c
struct Page {  
……   
list_entry_t pra_page_link;   
uintptr_t pra_vaddr;   
};
```

pra_page_link可用来构造按页的第一次访问时间进行排序的一个链表，这个链表的开始表示第一次访问时间最近的页，链表结尾表示第一次访问时间最远的页。当然链表头可以就可设置为pra_list_head（定义在swap_fifo.c中），构造的时机是在page fault发生后，进行do_pgfault函数时。pra_vaddr可以用来记录此物理页对应的虚拟页起始地址。

补充do_pgfault函数

```c
/* do_pgfault - interrupt handler to process the page fault execption
 * @mm         : the control struct for a set of vma using the same PDT
 * @error_code : the error code recorded in trapframe->tf_err which is setted by x86 hardware
 * @addr       : the addr which causes a memory access exception, (the contents of the CR2 register)
 *
 * CALL GRAPH: trap--> trap_dispatch-->pgfault_handler-->do_pgfault
 * The processor provides ucore's do_pgfault function with two items of information to aid in diagnosing
 * the exception and recovering from it.
 *   (1) The contents of the CR2 register. The processor loads the CR2 register with the
 *       32-bit linear address that generated the exception. The do_pgfault fun can
 *       use this address to locate the corresponding page directory and page-table
 *       entries.
 *   (2) An error code on the kernel stack. The error code for a page fault has a format different from
 *       that for other exceptions. The error code tells the exception handler three things:
 *         -- The P flag   (bit 0) indicates whether the exception was due to a not-present page (0)
 *            or to either an access rights violation or the use of a reserved bit (1).
 *         -- The W/R flag (bit 1) indicates whether the memory access that caused the exception
 *            was a read (0) or write (1).
 *         -- The U/S flag (bit 2) indicates whether the processor was executing at user mode (1)
 *            or supervisor mode (0) at the time of the exception.
 */
int
do_pgfault(struct mm_struct *mm, uint32_t error_code, uintptr_t addr) {
    int ret = -E_INVAL;
    //try to find a vma which include addr
    struct vma_struct *vma = find_vma(mm, addr);

    pgfault_num++;
    //If the addr is in the range of a mm's vma?
    if (vma == NULL || vma->vm_start > addr) {
        cprintf("not valid addr %x, and  can not find it in vma\n", addr);
        goto failed;
    }
    //check the error_code
    switch (error_code & 3) {
    default:
            /* error code flag : default is 3 ( W/R=1, P=1): write, present */
    case 2: /* error code flag : (W/R=1, P=0): write, not present */
        if (!(vma->vm_flags & VM_WRITE)) {
            cprintf("do_pgfault failed: error code flag = write AND not present, but the addr's vma cannot write\n");
            goto failed;
        }
        break;
    case 1: /* error code flag : (W/R=0, P=1): read, present */
        cprintf("do_pgfault failed: error code flag = read AND present\n");
        goto failed;
    case 0: /* error code flag : (W/R=0, P=0): read, not present */
        if (!(vma->vm_flags & (VM_READ | VM_EXEC))) {
            cprintf("do_pgfault failed: error code flag = read AND not present, but the addr's vma cannot read or exec\n");
            goto failed;
        }
    }
    /* IF (write an existed addr ) OR
     *    (write an non_existed addr && addr is writable) OR
     *    (read  an non_existed addr && addr is readable)
     * THEN
     *    continue process
     */
    uint32_t perm = PTE_U;
    if (vma->vm_flags & VM_WRITE) {
        perm |= PTE_W;
    }
    addr = ROUNDDOWN(addr, PGSIZE);

    ret = -E_NO_MEM;

    pte_t *ptep=NULL;
    /*LAB3 EXERCISE 1: YOUR CODE
    * Maybe you want help comment, BELOW comments can help you finish the code
    *
    * Some Useful MACROs and DEFINEs, you can use them in below implementation.
    * MACROs or Functions:
    *   get_pte : get an pte and return the kernel virtual address of this pte for la
    *             if the PT contians this pte didn't exist, alloc a page for PT (notice the 3th parameter '1')
    *   pgdir_alloc_page : call alloc_page & page_insert functions to allocate a page size memory & setup
    *             an addr map pa<--->la with linear address la and the PDT pgdir
    * DEFINES:
    *   VM_WRITE  : If vma->vm_flags & VM_WRITE == 1/0, then the vma is writable/non writable
    *   PTE_W           0x002                   // page table/directory entry flags bit : Writeable
    *   PTE_U           0x004                   // page table/directory entry flags bit : User can access
    * VARIABLES:
    *   mm->pgdir : the PDT of these vma
    *
    */
#if 0
    /*LAB3 EXERCISE 1: YOUR CODE*/
    ptep = ???              //(1) try to find a pte, if pte's PT(Page Table) isn't existed, then create a PT.
    if (*ptep == 0) {
                            //(2) if the phy addr isn't exist, then alloc a page & map the phy addr with logical addr

    }
    else {
    /*LAB3 EXERCISE 2: YOUR CODE
    * Now we think this pte is a  swap entry, we should load data from disk to a page with phy addr,
    * and map the phy addr with logical addr, trigger swap manager to record the access situation of this page.
    *
    *  Some Useful MACROs and DEFINEs, you can use them in below implementation.
    *  MACROs or Functions:
    *    swap_in(mm, addr, &page) : alloc a memory page, then according to the swap entry in PTE for addr,
    *                               find the addr of disk page, read the content of disk page into this memroy page
    *    page_insert ： build the map of phy addr of an Page with the linear addr la
    *    swap_map_swappable ： set the page swappable
    */
        if(swap_init_ok) {
            struct Page *page=NULL;
                                    //(1）According to the mm AND addr, try to load the content of right disk page
                                    //    into the memory which page managed.
                                    //(2) According to the mm, addr AND page, setup the map of phy addr <---> logical addr
                                    //(3) make the page swappable.
        }
        else {
            cprintf("no swap_init_ok but ptep is %x, failed\n",*ptep);
            goto failed;
        }
   }
#endif
    ptep = get_pte(mm->pgdir, addr, 1);
    if(*ptep == 0){
        pgdir_alloc_page(mm->pgdir, addr, perm);
    }
    else{
        if(swap_init_ok){
            struct Page *page = NULL;
            swap_in(mm, addr, &page);					
          														//根据mm结构和addr地址，尝试将硬盘中的内容换入至page中
            page_insert(mm->pgdir, page, addr, perm);
          														//根据mm结构、addr地址和page，建立物理地址和逻辑地址的映射
            swap_map_swappable(check_mm_struct, addr, page, 1);
          														//将此页面设置为可交换的 
            page->pra_vaddr = addr;
          														//记录物理页对应的虚拟页起始地址
        }
        else {
            cprintf("no swap_init_ok but ptep is %x, failed\n",*ptep);
            goto failed;
        }
    }
    ret = 0;


failed:
    return ret;
}

```

#### 换出部分

当一个物理页 （struct Page） 需要被 swap 出去的时候，首先需要确保它已经分配了一个位于磁盘上的swap page（由连续的8个扇区组成）。这里为了简化设计，在swap_check函数中建立了每个虚拟页唯一对应的swap page，其对应关系设定为：虚拟页对应的PTE的索引值 = swap page的扇区起始位置*8。

为了实现各种页替换算法，我们设计了一个页替换算法的类框架swap_manager:

```c
struct swap_manager  
{  
    const char *name;  
    /* Global initialization for the swap manager */  
    int (*init) (void);  
    /* Initialize the priv data inside mm_struct */  
    int (*init_mm) (struct mm_struct *mm);  
    /* Called when tick interrupt occured */  
    int (*tick_event) (struct mm_struct *mm);  
    /* Called when map a swappable page into the mm_struct */  
    int (*map_swappable) (struct mm_struct *mm, uintptr_t addr, struct Page *page, int swap_in);   
    /* When a page is marked as shared, this routine is called to delete the addr entry from the swap manager */
    int (*set_unswappable) (struct mm_struct *mm, uintptr_t addr);  
    /* Try to swap out a page, return then victim */  
    int (*swap_out_victim) (struct mm_struct *mm, struct Page *ptr_page, int in_tick);  
    /* check the page relpacement algorithm */  
    int (*check\_swap)(void);   
};
```

这里关键的两个函数指针是map_swappable和swap_out_vistim，前一个函数用于**记录页访问情况相关属性**，后一个函数用于**挑选需要换出的页**。显然第二个函数依赖于第一个函数记录的页访问情况。tick_event函数指针也很重要，结合定时产生的中断，可以实现一种积极的换页策略。

在ucore实验中，对链表的插入有两种操作，即在表头插入（list_add_after）或在表尾插入（list_add_before）。因为双向循环链表的链表头的next、prev分别指向链表中的第一个和最后一个节点，在表头插入是插入在listelm之后，即插在链表的最前位置。而在表尾插入是插入在listelm->prev之后，即插在链表的最后位置

当需要删除空闲块链表中的Page结构的链表节点时，可调用内联函数list_del，而list_del进一步调用了__list_del来完成具体的删除操作。其实现为：

```c
static inline void
list_del(list_entry_t *listelm) {
    __list_del(listelm->prev, listelm->next);
}
static inline void
__list_del(list_entry_t *prev, list_entry_t *next) {
    prev->next = next;
    next->prev = prev;
}
```

如果要确保被删除的节点listelm不再指向链表中的其他节点，这可以通过调用list_init函数来把listelm的prev、next指针分别自身，即将节点置为空链状态。这可以通过list_del_init函数来完成。

补充_fifo_map_swappable函数

```c
/*
 * (3)_fifo_map_swappable: According FIFO PRA, we should link the most recent arrival page at the back of pra_list_head qeueue
 */
static int
_fifo_map_swappable(struct mm_struct *mm, uintptr_t addr, struct Page *page, int swap_in)
{
    list_entry_t *head=(list_entry_t*) mm->sm_priv;
    list_entry_t *entry=&(page->pra_page_link);
 
    assert(entry != NULL && head != NULL);
    //record the page access situlation
    /*LAB3 EXERCISE 2: YOUR CODE*/ 
    //(1)link the most recent arrival page at the back of the pra_list_head qeueue.
    list_add_before(head, entry);//只需要把物理页插入列表的尾部（头部为存在时间最久）
    return 0;
}
```



补充_fifo_swap_out_victim函数

```c
/*
 *  (4)_fifo_swap_out_victim: According FIFO PRA, we should unlink the  earliest arrival page in front of pra_list_head qeueue,
 *                            then assign the value of *ptr_page to the addr of this page.
 */
static int
_fifo_swap_out_victim(struct mm_struct *mm, struct Page ** ptr_page, int in_tick)
{
     list_entry_t *head=(list_entry_t*) mm->sm_priv;
         assert(head != NULL);
     assert(in_tick==0);
     /* Select the victim */
     /*LAB3 EXERCISE 2: YOUR CODE*/ 
     //(1)  unlink the  earliest arrival page in front of pra_list_head qeueue
     //(2)  assign the value of *ptr_page to the addr of this page
     list_entry_t *le = list_next(head); //找到链表头，即存在时间最久的节点
     assert(head!=le);                   //断言这不是头节点
     struct Page *p = le2page(le, pra_page_link);//用le2page函数将该节点对应的页分配给p指针
     list_del(le);                       //从列表中取出节点
     assert(p !=NULL);
     *ptr_page = p;											 //将该节点对应的页分配给ptr_page指针
     
     return 0;
}
```

- 如果要在ucore上实现"extended clock页替换算法"请给你的设计方案，现有的swap_manager框架是否足以支持在ucore中实现此算法？如果是，请给你的设计方案。如果不是，请给出你的新的扩展和基此扩展的设计方案。并需要回答如下问题

    - 需要被换出的页的特征是什么？
    - 在ucore中如何判断具有这样特征的页？
    - 何时进行换入和换出操作？

    这里摘了简书上师傅的答案

    >在现有框架基础上可以支持Extended clock算法，具体设计方案如下所示：
    >
    >- 根据上文中提及到的PTE的组成部分可知，PTE中包含了dirty位和访问位，因此可以确定某一个虚拟页是否被访问过以及写过，但是，考虑到在替换算法的时候是将物理页面进行换出，而可能存在着多个虚拟页面映射到同一个物理页面这种情况，也就是说某一个物理页面是否dirty和是否被访问过是有这些所有的虚拟页面共同决定的，而在原先的实验框架中，物理页的描述信息Page结构中默认只包括了一个对应的虚拟页的地址，应当采用链表的方式，在Page中扩充一个成员，把物理页对应的所有虚拟页都给保存下来；而物理页的dirty位和访问位均为只需要某一个对应的虚拟页对应位被置成1即可置成1；
    >- 完成了上述对物理页描述信息的拓展之后，考虑对FIFO算法的框架进行修改得到拓展时钟算法的框架，由于这两种算法都是将所有可以换出的物理页面均按照进入内存的顺序连成一个环形链表，因此初始化，将某个页面置为可以/不可以换出这些函数均不需要进行大的修改(小的修改包括在初始化当前指针等)，唯一需要进行重写的函数是选择换出物理页的函数swap_out_victim，对该函数的修改如下：
    >    - 从当前指针开始，对环形链表进行扫描，根据指针指向的物理页的状态（表示为(access, dirty)）来确定应当进行何种修改：
    >        - 如果状态是(0, 0)，则将该物理页面从链表上取下，该物理页面记为换出页面，但是由于这个时候这个页面不是dirty的，因此事实上不需要将其写入swap分区；
    >        - 如果状态是(0, 1)，则将该物理页对应的虚拟页的PTE中的dirty位都改成0，并且将该物理页写入到外存中，然后指针跳转到下一个物理页；
    >        - 如果状态是(1, 0), 将该物理页对应的虚拟页的PTE中的访问位都置成0，然后指针跳转到下一个物理页面；
    >        - 如果状态是(1, 1)，则该物理页的所有对应虚拟页的PTE中的访问为置成0，然后指针跳转到下一个物理页面；
    >
    >**需要被换出的页的特征是什么？**
    >
    >- 该物理页在当前指针上一次扫过之前没有被访问过；
    >- 该物理页的内容与其在外存中保存的数据是一致的, 即没有被修改过;
    >
    >**在ucore中如何判断具有这样特征的页？**
    >
    >- 在ucore中判断具有这种特征的页的方式已经在上文设计方案中提及过了，具体为：
    >    - 假如某物理页对应的所有虚拟页中存在一个dirty的页，则认为这个物理页为dirty，否则不这么认为；
    >    - 假如某物理页对应的所有虚拟页中存在一个被访问过的页，则认为这个物理页为被访问过的，否则不这么认为；
    >
    >**何时进行换入和换出操作？**
    >
    >- 在产生page fault的时候进行换入操作；
    >- 换出操作源于在算法中将物理页的dirty从1修改成0的时候，因此这个时候如果不进行写出到外存，就会造成数据的不一致，具体写出内存的时机是比较细节的问题, 可以在修改dirty的时候写入外存，或者是在这个物理页面上打一个需要写出的标记，到了最终删除这个物理页面的时候，如果发现了这个写出的标记，则在这个时候再写入外存；后者使用一个写延迟标记，有利于多个写操作的合并，从而降低缺页的代价；
    >
    >
    >
    >作者：AmadeusChan
    >链接：https://www.jianshu.com/p/8d6ce61ac678
    >来源：简书
    >著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

