## 知识点

### 连续内存分配

#### 最先匹配策略

##### 原理

- 空间分区列表按地址顺序排序

- 分配过程时，搜索一个合适的分区

- 释放分区时，检查是否可与邻近的空闲分区合并

##### 优点

- 简单

- 在高地址空间有大块的空闲分区

##### 缺点

- 外部碎片

- 分配大块时较慢

#### 最佳匹配策略

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1ghgzmmuvd7j31ha0u0b29.jpg)

#### 最差匹配策略

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1ghgzmi068uj31hc0u0b29.jpg)



### 碎片整理

通过调整进程占用的分区位置来减少或避免分区碎片

- 紧凑：通过移动分配给进程的内存分区，以合并外部碎片（所有应用程序可动态重定位）
- 分区对换：通过抢占并回收处于等待状态进程的分区，以增大可用的内存空间



### 伙伴系统

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1ghgzrvqpwcj31ha0u07wh.jpg)

伙伴系统实现

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1ghgzygc59dj31hh0u0e81.jpg)



### 非连续内存分配

#### 段式存储管理

用段表示访问方式和存储数据等属性相同的一段地址空间

访问由二元组（段号,段内偏移）表示

在段描述符中查找段基址和长度

通过段基址和段内偏移计算物理地址

#### 页式存储管理

把物理地址空间划分成大小相同的基本分配单位（大小为2的n次方），即页帧；把逻辑地址划分成大小都与页帧相同的页

访问由二元组（页号,页内偏移）表示

将页号作为索引在页表中查找对应页表项（页表基址在页表基址寄存器中），页表项中有帧号和标志位（存在位、修改位、引用位、只读位）

通过帧号和页内偏移计算物理地址

对页表访问性能问题解决

- 快表：把近期访问过的页表项缓存在cpu中，若命中则直接计算访问物理地址，否则正常访问页表，并把对应页表项更新到快表中

- 多级页表：通过间接引用将页号分为k级

    ![](https://tva1.sinaimg.cn/large/007S8ZIlgy1ghh739uojsj317s0mgamn.jpg)

- 反置页表：让页表与物理地址空间的大小相对应

    将进程标识pid与页号进行hash变换，将结果作为索引，在页帧作为地址的表中对比对应页表项与hash变换前的pid与页号

    如果一致则页表项地址就是物理地址帧号

    ![](https://tva1.sinaimg.cn/large/007S8ZIlgy1ghh7cv5xqyj317q0nak47.jpg)

    如果不一致，即发生hash冲突，则需要解决hash冲突——核对下一项，直到找到对应表项

    ![](https://tva1.sinaimg.cn/large/007S8ZIlgy1ghh77mxz3rj318k0n4dts.jpg)

    

#### 段页式存储管理

由n元组表示（段号，1级页表号，……，n级页表号，页内偏移）

由进程段基址找到段表基址，段号索引找到段表项（段长度+段基址），段基址中找到对应页表项中的对应物理页帧号，根据帧号和偏移地址访问物理内存单位



## lab2

>### 练习
>
>为了实现lab2的目标，lab2提供了3个基本练习和2个扩展练习，要求完成实验报告。
>
>对实验报告的要求：
>
>- 基于markdown格式来完成，以文本方式为主
>- 填写各个基本练习中要求完成的报告内容
>- 完成实验后，请分析ucore_lab中提供的参考答案，并请在实验报告中说明你的实现与参考答案的区别
>- 列出你认为本实验中重要的知识点，以及与对应的OS原理中的知识点，并简要说明你对二者的含义，关系，差异等方面的理解（也可能出现实验中的知识点没有对应的原理知识点）
>- 列出你认为OS原理中很重要，但在实验中没有对应上的知识点

ucore用低12位作为页内偏移，中间10位作为二级页表索引，高10位作为一级页表索引

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1ghh8plyurhj31820n6ako.jpg)

以0xc1234567为例

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1ghh8oplbivj31840lwano.jpg)

PDE与PTE都是4kb偏移的，需要右移3位

（为了在保护模式下使能页机制，os需要置CR0寄存器中的第31位为1）

### 练习0

>**练习0：填写已有实验**
>
>本实验依赖实验1。请把你做的实验1的代码填入本实验中代码中有“LAB1”的注释相应部分。提示：可采用diff和patch工具进行半自动的合并（merge），也可用一些图形化的比较/merge工具来手动合并，比如meld，eclipse中的diff/merge工具，understand中的diff/merge工具等。

将lab1中修改的部分填充到lab1中

### 练习1

>**练习1：实现 first-fit 连续物理内存分配算法（需要编程）**
>
>在实现first fit 内存分配算法的回收函数时，要考虑地址连续的空闲块之间的合并操作。提示:在建立空闲页块链表时，需要按照空闲页块起始地址来排序，形成一个有序的链表。可能会修改default_pmm.c中的default_init，default_init_memmap，default_alloc_pages， default_free_pages等相关函数。请仔细查看和理解default_pmm.c中的注释。
>
>请在实验报告中简要说明你的设计实现过程。请回答如下问题：
>
>- 你的first fit算法是否有进一步的改进空间

在ucore实验中，对链表的插入有两种操作，即在表头插入（list_add_after）或在表尾插入（list_add_before）。因为双向循环链表的链表头的next、prev分别指向链表中的第一个和最后一个节点，在表头插入是插入在listelm之后，即插在链表的最前位置。而在表尾插入是插入在listelm->prev之后，即插在链表的最后位置（list_add等于list_add_after）

由于要实现首次分配算法，为了更好控制插入位置，有些地方使用了内联函数

```c
static inline void
__list_add(list_entry_t *elm, list_entry_t *prev, list_entry_t *next) {
    prev->next = next->prev = elm;
    elm->next = next;
    elm->prev = prev;
}
```

```c
// 头插法
list_add_after(list_entry_t *listelm, list_entry_t *elm) {
    __list_add(elm, listelm, listelm->next);
}
static inline void    
list_add(list_entry_t *listelm, list_entry_t *elm) {
    list_add_after(listelm, elm);
}
// 尾插法
list_add_before(list_entry_t *listelm, list_entry_t *elm) {
    __list_add(elm, listelm->prev, listelm);
}
```

>(2) `default_init`:
>
>You can reuse the demo `default_init` function to initialize the `free_list` and set `nr_free` to 0. 
>
>`free_list` is used to record the free memory blocks.
>
>`nr_free` is the total number of the free memory blocks.

```c
static void
default_init(void) {
    list_init(&free_list);
    nr_free = 0;
}//没有改变
```



>(3) `default_init_memmap`:
>
>CALL GRAPH: `kern_init` --> `pmm_init` --> `page_init` --> `init_memmap` -->pmm_manager` --> `init_memmap`.
>
>This function is used to initialize a free block (with parameter `addr_base`,page_number`). In order to initialize a free block, firstly, you should initialize each page (defined in memlayout.h) in this freeblock. This procedure includes:
>
>* Setting the bit `PG_property` of `p->flags`, which means this page is valid. P.S. In function `pmm_init` (in pmm.c), the bit `PG_reserved` of `p->flags` is already set.
>
>* If this page is free and is not the first page of a free block,`p->property` should be set to 0.If this page is free and is the first page of a free block, `p->property` should be set to be the total number of pages in the block.
>
>* `p->ref` should be 0, because now `p` is fre and has no reference.
>
>    After that, We can use `p->page_link` to link this page into `free_list`.
>
>    (e.g.: `list_add_before(&free_list, &(p->page_link));` )
>
>* Finally, we should update the sum of the free memory blocks: `nr_free += n`.

```c
static void
default_init_memmap(struct Page *base, size_t n) {
    assert(n > 0);
    struct Page *p = base;
    for (; p != base + n; p ++) {
        assert(PageReserved(p));										//断言初始化的页为可保留页
        p->flags = p->property = 0;									//物理页属性初始设为0
        SetPageProperty(p); 												//空闲页设置为1
        set_page_ref(p, 0);													//映射此物理页的虚拟页个数为0
    }
    base->property = n;															//第一页的property表示后面连续的物理页个数
    nr_free += n;																		//空闲页数多了n页
    list_add(&free_list, &(base->page_link));   		//将第一页放入空闲列表中，列表中只有一个连续物理页
}
```



>(4) `default_alloc_pages`:
>
>Search for the first free block (block size >= n) in the free list and reszie the block found, returning the address of this block as the address required by `malloc`.
>
>(4.1)So you should search the free list like this:
>
> ```
>list_entry_t le = &free_list;
>while((le=list_next(le)) != &free_list) {
>...
>}
>
> ```
>
>(4.1.1)In the while loop, get the struct `page` and check if `p->property` (recording the num of free pages in this block) >= n.
>
>```
>struct Page *p = le2page(le, page_link);
>if(p->property >= n){ 
>	...
>}
>```
>
>(4.1.2)If we find this `p`, it means we've found a free block with its size >= n, whose first `n` pages can be malloced. Some flag bits of this page should be set as the following: `PG_reserved = 1`, `PG_property = 0`.Then, unlink the pages from `free_list`.
>
>(4.1.2.1)If `p->property > n`, we should re-calculate number of the rest pages of this free block. (e.g.: `le2page(le,page_link))->property = p->property - n;`)
>
>(4.1.3)Re-caluclate `nr_free` (number of the the rest of all free block).
>
>(4.1.4)return `p`.
>
>(4.2) If we can not find a free block with its size >=n, then return NULL.

```c
static struct Page *
default_alloc_pages(size_t n) {
    assert(n > 0);
    if (n > nr_free) {
        return NULL;
    }
    struct Page *page = NULL;
    list_entry_t *le = &free_list;
    while ((le = list_next(le)) != &free_list) {					//找第一个合适大小的连续物理页
        struct Page *p = le2page(le, page_link);
        if (p->property >= n) {
            page = p;
            break;
        }
    }
    if (page != NULL) {
        if (page->property > n) {													//如果这个页较大，需要进行切割
            struct Page *p = page + n;
            p->property = page->property - n;
            SetPageProperty(p);
            __list_add(&(p->page_link),&page->page_link,list_next(&page->page_link));
            																							//使用内联函数在page后插入节点
        }
        for(struct Page *p1 = page; p1 != page + n; p1++){//设置选中的即将进行分配的物理页标志位
            ClearPageProperty(p1);
            ClearPageReserved(p1);
            set_page_ref(p1, 0);
        }
        nr_free -= n;
        list_del(&(page->page_link));
    }
    return page;
}
```



写不出来，，抄了参考答案

>(5) `default_free_pages`:
>
>re-link the pages into the free list, and may merge small free blocks into the big ones.
>
>(5.1)According to the base address of the withdrawed blocks, search the free list for its correct position (with address from low to high), and insert the pages. (May use `list_next`, `le2page`, `list_add_before`)
>
>(5.2)Reset the fields of the pages, such as `p->ref` and `p->flags` (PageProperty)
>
>(5.3)Try to merge blocks at lower or higher addresses. Notice: This should change some pages' `p->property` correctly.

```c
static void
default_free_pages(struct Page *base, size_t n) {
    assert(n > 0);
    struct Page *p = base;
    for (; p != base + n; p ++) {												
        assert(!PageReserved(p) && !PageProperty(p));		//断言连续物理页base中没有保留页和空闲页
        p->flags = 0;																		//将连续物理页base中的标志置位
        set_page_ref(p, 0);
    }
    base->property = n;																	//将第一页置位
    SetPageProperty(base);
    list_entry_t *le = list_next(&free_list);
    while (le != &free_list) {													//循环空闲页链表
        p = le2page(le, page_link);
        le = list_next(le);
        if (base + base->property == p) {								//如有相邻则从链表取出合并
            base->property += p->property;
            ClearPageProperty(p);
            list_del(&(p->page_link));
        }
        else if (p + p->property == base) {
            p->property += base->property;
            ClearPageProperty(base);
            base = p;
            list_del(&(p->page_link));
        }
    }
    nr_free += n;
    le = list_next(&free_list);
    while (le != &free_list) {													//再次循环
        p = le2page(le, page_link);											//在合适的位置将合并后的连续物理页插入空闲链表
        if (base + base->property <= p){								//往next过去地址为从大到小
        	assert(base + base->property != p);
        	break;
        }																								//若找到比base
        le = list_next(le);															//插入
    }
    list_add_before(le, &(base->page_link));
}
```



### 练习2

>**练习2：实现寻找虚拟地址对应的页表项（需要编程）**
>
>通过设置页表和对应的页表项，可建立虚拟内存地址和物理内存地址的对应关系。其中的get_pte函数是设置页表项环节中的一个重要步骤。此函数找到一个虚地址对应的二级页表项的内核虚地址，如果此二级页表项不存在，则分配一个包含此项的二级页表。本练习需要补全get_pte函数 in kern/mm/pmm.c，实现其功能。请仔细查看和理解get_pte函数中的注释。get_pte函数的调用关系图如下所示：
>
>![img](https://chyyuu.gitbooks.io/ucore_os_docs/content/lab2_figs/image001.png) 图1 get_pte函数的调用关系图
>
>请在实验报告中简要说明你的设计实现过程。请回答如下问题：
>
>- 请描述页目录项（Page Directory Entry）和页表项（Page Table Entry）中每个组成部分的含义以及对ucore而言的潜在用处。
>- 如果ucore执行过程中访问内存，出现了页访问异常，请问硬件要做哪些事情？

参考百度上师傅的

> 逻辑地址通过段式管理的地址映射可以得到线性地址，线性地址通过页式管理的地址映射得到物理地址。
>
> 有时我们也把逻辑地址称为虚拟地址。
>
> 函数调用到get_pte的时候，只需要使用线性地址操作即可

```c
//get_pte - get pte and return the kernel virtual address of this pte for la
//        - if the PT contians this pte didn't exist, alloc a page for PT
// parameter:
//  pgdir:  the kernel virtual base address of PDT
//  la:     the linear address need to map
//  create: a logical value to decide if alloc a page for PT
// return vaule: the kernel virtual address of this pte
pte_t *
get_pte(pde_t *pgdir, uintptr_t la, bool create) {
    /* LAB2 EXERCISE 2: YOUR CODE
     *
     * If you need to visit a physical address, please use KADDR()
     * please read pmm.h for useful macros
     *
     * Maybe you want help comment, BELOW comments can help you finish the code
     *
     * Some Useful MACROs and DEFINEs, you can use them in below implementation.
     * MACROs or Functions:
     *   PDX(la) = the index of page directory entry of VIRTUAL ADDRESS la.
     *   KADDR(pa) : takes a physical address and returns the corresponding kernel virtual address.
     *   set_page_ref(page,1) : means the page be referenced by one time
     *   page2pa(page): get the physical address of memory which this (struct Page *) page  manages
     *   struct Page * alloc_page() : allocation a page
     *   memset(void *s, char c, size_t n) : sets the first n bytes of the memory area pointed by s to the specified value c.
     * DEFINEs:
     *   PTE_P           0x001                   // page table/directory entry flags bit : Present
     *   PTE_W           0x002                   // page table/directory entry flags bit : Writeable
     *   PTE_U           0x004                   // page table/directory entry flags bit : User can access
     */
#if 0
    pde_t *pdep = NULL;   // (1) find page directory entry
    if (0) {              // (2) check if entry is not present
                          // (3) check if creating is needed, then alloc page for page table
                          // CAUTION: this page is used for page table, not for common data page
                          // (4) set page reference
        uintptr_t pa = 0; // (5) get linear address of page
                          // (6) clear page content using memset
                          // (7) set page directory entry's permission
    }
    return NULL;          // (8) return page table entry
#endif
    pde_t *pgtab = pgdir + PDX(la);									//从页目录表中获取页表入口地址
    if(!(*pgtab & 1)){																//确认页目录表表项是否存在
        if(!create)																	//如不存在是否建立
            return NULL;
        else{																				//建立页表
            struct Page *page = alloc_page();
            if(page == NULL)
                return NULL;
            set_page_ref(page,1);										//设置引用计数为1
            uintptr_t pa = page2pa(page);						//得到该页物理地址
            memset(KADDR(pa), 0, PGSIZE);						//初始化为0，因为这个页所代表的虚拟地址都没有被映射
            																				//参数为虚拟地址而不是物理地址
            *pgtab = pa | PTE_U | PTE_W | PTE_P;		//设置页表存在位，可写位和用户使用位
        }
    }
  //这里用PDE_ADDR宏得到页表地址
  //kern/mm/mmu.h
  //#define PTE_ADDR(pte)   ((uintptr_t)(pte) & ~0xFFF)
	//#define PDE_ADDR(pde)   PTE_ADDR(pde)
  //用PTX宏得到页表中的索引
  //kern/mm/mmu.h
  //#define PTX(la) ((((uintptr_t)(la)) >> PTXSHIFT) & 0x3FF)
  //转化为对应内核虚拟地址后，再强制类型转换为页入口地址类型返回
    return ((pte_t *)KADDR(PDE_ADDR(*pgtab))+PTX(la))
}
```

- 请描述页目录项（Page Directory Entry）和页表项（Page Table Entry）中每个组成部分的含义以及对ucore而言的潜在用处。

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

- 如果ucore执行过程中访问内存，出现了页访问异常，请问硬件要做哪些事情？

    1. 将发生错误的线性地址保存在cr2寄存器中;
    2. 在中断栈中依次压入EFLAGS，CS, EIP，以及页访问异常码error code，如果page fault是发生在用户态，则还需要先压入ss和esp，并且切换到内核栈；
    3. 根据中断描述符表查询到对应page fault的ISR，跳转到对应的ISR处执行，接下来将由软件进行page fault处理；



### 练习3

>**练习3：释放某虚地址所在的页并取消对应二级页表项的映射（需要编程）**
>
>当释放一个包含某虚地址的物理内存页时，需要让对应此物理内存页的管理数据结构Page做相关的清除处理，使得此物理内存页成为空闲；另外还需把表示虚地址与物理地址对应关系的二级页表项清除。请仔细查看和理解page_remove_pte函数中的注释。为此，需要补全在 kern/mm/pmm.c中的page_remove_pte函数。page_remove_pte函数的调用关系图如下所示：
>
>![img](https://chyyuu.gitbooks.io/ucore_os_docs/content/lab2_figs/image002.png)
>
>图2 page_remove_pte函数的调用关系图
>
>请在实验报告中简要说明你的设计实现过程。请回答如下问题：
>
>- 数据结构Page的全局变量（其实是一个数组）的每一项与页表中的页目录项和页表项有无对应关系？如果有，其对应关系是啥？
>- 如果希望虚拟地址与物理地址相等，则需要如何修改lab2，完成此事？ **鼓励通过编程来具体完成这个问题**

按照给出的步骤写

```c
//page_remove_pte - free an Page sturct which is related linear address la
//                - and clean(invalidate) pte which is related linear address la
//note: PT is changed, so the TLB need to be invalidate 
static inline void
page_remove_pte(pde_t *pgdir, uintptr_t la, pte_t *ptep) {
    /* LAB2 EXERCISE 3: YOUR CODE
     *
     * Please check if ptep is valid, and tlb must be manually updated if mapping is updated
     *
     * Maybe you want help comment, BELOW comments can help you finish the code
     *
     * Some Useful MACROs and DEFINEs, you can use them in below implementation.
     * MACROs or Functions:
     *   struct Page *page pte2page(*ptep): get the according page from the value of a ptep
     *   free_page : free a page
     *   page_ref_dec(page) : decrease page->ref. NOTICE: ff page->ref == 0 , then this page should be free.
     *   tlb_invalidate(pde_t *pgdir, uintptr_t la) : Invalidate a TLB entry, but only if the page tables being edited are the ones currently in use by the processor.
     * DEFINEs:
     *   PTE_P           0x001                   // page table/directory entry flags bit : Present
     */
#if 0
    if (0) {                      //(1) check if this page table entry is present
        struct Page *page = NULL; //(2) find corresponding page to pte
                                  //(3) decrease page reference
                                  //(4) and free this page when page reference reachs 0
                                  //(5) clear second page table entry
                                  //(6) flush tlb
    }
#endif
    assert(*ptep & PTE_P); 									//断言页表项是存在的
    struct Page *page = pte2page(*ptep);		//从页表项获取页表地址
    page_ref_dec(page);											//该页表引用位减一
    if(page->ref == 0)											//如果引用位为0的话需要释放页表
        free_page(page);
    *ptep &= (~PTE_P);											//页表项存在位清零
    tlb_invalidate(pgdir, la);							//刷新TLB（页表缓存）条目
}
```

- 数据结构Page的全局变量（其实是一个数组）的每一项与页表中的页目录项和页表项有无对应关系？如果有，其对应关系是啥？

    Page结构体与物理页一一对应，即页目录项保存的物理页面地址（某个页表）、页表项保存的物理页面地址都对应于Page数组中的某一页

- 如果希望虚拟地址与物理地址相等，则需要如何修改lab2，完成此事？ **鼓励通过编程来具体完成这个问题**

    >通过ld工具形成的ucore的起始虚拟地址从0xC0100000开始，而bootloader把ucore放在了起始物理地址为0x100000的物理内存空间。要使得虚拟地址与物理地址相等：首先更改KERNBASE，从0xC000000到0x00000000，原因在于在没有开启页式地址转换前，内核逻辑地址经过一次段地址转换即直接得到物理地址： PA（物理地址）=LA（线性地址）=VA（逻辑地址）-KERNBASE；其次，拟地址由0xC0100000改成0x00100000

