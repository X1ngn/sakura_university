## 知识点

### 同步互斥

**临界区**——进程中访问临界资源的一段需要互斥执行的代码

**进入区**——检查是否可进入临界区的一段代码（如可进入，则设置相应“正在访问临界区”标记）

**退出区**——清除“正在访问临界区”标记

剩余区——代码其余部分（与同步互斥无关）



### 临界区的访问规则

- 空闲则入——没有进程在临界区时，任何进程都可以进入
- 忙则等待——有进程在临界区时，任何进程均不能进入
- 有限等待——等待进入临界区的进程不能无限期等待
- 让权等待（可选）——不能进入等待区的进程应释放cpu，如转化到阻塞状态

### 临界区的实现方法

#### 禁用硬件中断（仅限单处理器）

#### 基于软件的同步方法——进程间通过共享变量的访问实现进程同步（复杂）

- Peterson算法（两个线程）

    共享变量

    ```c
    int turn;//表示哪个进程要进入临界区
    boolean flag[N];//表示进程是否准备好进入临界区
    ```

    进入区代码（对于线程pid为i）

    ```c
    flag[i] = true;//编号为i的进程已经准备好进入临界区
    turn = i;//编号为i的进程要进入临界区
    while(flag[j] && turn = i);//进程满足条件则循环等待
    ```

    退出区代码

    ```c
    flag[i] = false;
    ```

    有一个进程时，等待条件不满足，直接进入临界区

    有两个进程时，对于进程编号为i的进程和进程编号为j的进程

    ![](https://tva1.sinaimg.cn/large/007S8ZIlgy1gif4411bnxj30ma0h8wf3.jpg)

- Dekkers算法（多个线程）

    暂未理解

#### 更高级的抽象同步方法（单处理器多处理器均可）

##### 锁

- 一个二进制变量表示两种状态（锁定/解锁）
- Lock::Acquire() 锁被释放前一直等待，返回的时候得到锁（也就是访问临界区）
- Lock::Release() 释放锁，唤醒其他等待这个锁的线程



锁基于原子操作指令，如测试与置位指令(TS)、交换指令

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1gifixmvpmvj30rm0gajy0.jpg)

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1gifixqxk14j30s00d8n1n.jpg)

使用TS指令实现自旋锁

**忙等待锁**

初始化锁

```c++
class Lock{
	int value = 0;//解锁状态
}
```

请求操作

```c
void Lock::Acquire() {
	while(test-and-set(value));//锁定
		; //spin
}
```

释放操作

```c
void Lock::Release() {
	value = 0;//解锁
}
```

**无忙等待锁**

初始化锁

```c
class Lock{
	int value = 0;
	WaitQueue q;//等待该锁的线程所排成的队列
}
```

请求操作

```c
void Lock::Acquire() {
	while(test-and-set(value)) {//如果返回1，则代表锁正在被其他线程使用
    ...(将该线程的TCB加入队列q)
    schedule();//该进程进入等待状态，调度其他线程
  }
}
```

释放操作

```c
void Lock::Release() {
	value = 0;
  ...(将该线程的TCB从等待队列q删除)
  wakeup(t);//唤醒在等待状态的线程
}
```

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1giflhmlvhzj317y0n8ncu.jpg)

##### 信号量

是一种抽象数据类型，由一个整型和两个原子操作组成

```c
int sem; //可用的资源数
P(); //sem减一，如果sem小于0则进入等待，否则继续
V(); //sem加一，如果sem小于等于0则唤醒一个等待进程
```

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1giflhhhxexj317w0m44c0.jpg)

信号量的实现

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1giflkuj3rsj31800n0tk2.jpg)

信号量的使用

- 互斥访问——临界区的互斥访问控制

    每类资源设置一个信号量，其初值为1

    初始化

    ```c++
    mutex = new Samaphore(1);
    ```

    互斥访问控制

    ```c
    mutex->P();
    Critical Section;//临界区执行
    mutex->V();
    ```

- 条件同步——线程间的事件等待

    设置一个信号量，其初值为0

    初始化

    ```c++
    condition = new Samaphore(0);
    ```

    如必须先执行X再执行N

    ![](https://tva1.sinaimg.cn/large/007S8ZIlgy1giftu5nn4wj30tc0e00vj.jpg)

**生产者-消费者问题**

- 一个或多个生产者在生成数据后放在一个缓冲区里
- 单个消费者从缓冲区里取出数据处理
- 任何时刻只能有一个生产者或消费者可访问缓冲区

分析

- 任何时刻只能有一个线程操作缓冲区（互斥访问）
- 缓冲区空时，消费者必须等待生产者（条件同步）
- 缓冲区满时，生产者必须等待消费者（条件同步）

初始化资源信号量

```c++
class BoundedBuffer {
	mutex = new Samaphore(1);
	fullBuffers = new Samaphore(0);
	emptyBuffers = new Samaphore(n);
}
```

生产者

```c++
void BoundedBuffer::Deposit(c) {
	emptyBuffers->P();//条件同步，检查是否有空缓冲区
	mutex->P();//互斥访问临界区，检查是否有其他生产者或消费者在访问缓冲区
	...()//向缓冲区中写入
	mutex->V();
	fullBuffers->V();//写数据后释放一个满缓冲区资源
}
```

消费者

```c++
void BoundedBuffer::Remove(c) {
  fullBuffers->V();//条件同步，检查缓冲区中是否有数据
	mutex->P();//互斥访问临界区，检查是否有生产者在访问缓冲区
	...()//从缓冲区中取出
	mutex->V();
	emptyBuffers->P();//取出后释放一个空缓冲区资源
}
```

##### 管程

是一种用于多线程互斥访问共享资源的程序结构

管程与临界区相似，只是正在管程中的线程可临时放弃管程的互斥访问，等待时间出现时恢复

通过0个到多个条件变量（0个）管理共享数据的并发访问

- 每个条件变量对应一个等待队列，进入管程的线程因某种资源被占用而进入等待状态，则放在对应的等待队列中
- 条件变量中的Wait()操作
    - 将自己阻塞在等待队列中
    - 唤醒一个等待或释放管程的互斥访问
- 条件变量中的Signal()操作
    - 将等待队列中的一个线程唤醒
    - 如果等待队列为空，则等同空操作

管程的实现

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1gigtqv400lj316t0icwi1.jpg)

- 对于某个条件Condition，设置一个等待队列和一个整型代表等待的线程数
- A线程条件不满足时调用Wait函数，则将A线程加入等待队列，并释放锁，调度B线程
- A线程条件满足时，B线程调用Signal，唤醒原进程，有两种唤醒方式：
    - 让B线程继续执行直到释放B线程后再调度A线程
    - 马上调度A线程，A线程执行完释放之后再恢复B线程（ucore采用的方式）

用管程解决生产者-消费者问题

初始化

```c++
class BoundedBuffer {
	...
	Lock lock;
	int count = 0;
	Condition notFull, notEmpty;
  //缓冲区全满 这个条件满足的时候，进程进入notFull对应队列，等待不再缓冲区全满时唤醒
  //缓冲区全为空 这个条件满足的时候，进程进入notEmpty对应队列，等待不再缓冲区全为空时唤醒
}
```

生产者

```c++
void BoundedBuffer::Deposit(c) {
	lock->Acquire();//访问临界区
	while(count == n)//条件变量，判断是否有空的缓冲区
		notFull.wait(&lock);//没有空的缓冲区则进入notFull条件变量的等待队列
	...(向缓冲区写入字符)
	count++;
	notEmpty.signal();//如果notEmpty条件变量的等待队列中有线程在等待，则唤醒
	lock->Release;
}
```

消费者

```c++
void BoundedBuffer::Remove(c) {
	lock->Acquire();//访问临界区
	while(count == 0)//条件变量，判断所有缓冲区都是空的
		notEmpty.wait(&lock);//所有缓冲区都是空的则进入notEmpty条件变量的等待队列
	...(从缓冲区读取字符)
	count--;
	notFull.signal();//如果notFull条件变量的等待队列中有线程在等待，则唤醒
	lock->Release;
}
```

对条件变量的判断使用while，当前运行的线程优先级更高，被唤醒的线程条件判断不满足，只有当前进程释放后才能运行被唤醒的线程

用管程解决哲学家就餐问题

假设有五位哲学家围坐在一张圆形餐桌旁，做以下两件事情之一：吃面，或者思考。吃面的时候停止思考，思考的时候也停止吃面。每两个哲学家之间有一只餐叉。假设哲学家必须用两只餐叉吃东西。他们吃面的时候会拿起自己左右手边的两只餐叉，两只餐叉都拿起才可以吃面。要保证哲学家们动作有序地进行，即不会有人永远也拿不到餐叉

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1gih8xlvafeg305k05rglq.gif)

对五位哲学家编号，让奇数编号的哲学家先拿右边的餐叉

```c++
#define N 5
samphore fork[5];//五只餐叉

void philosopher(int i){
  while(true){
    think();//思考
    if(i%2==0){//编号为偶数的哲学家
      P(fork[i]);//先拿左边
      P(fork[(i+1) % N]);
    }
    else{//编号为奇数的哲学家
      P(fork[(i+1) % N]);//先拿右边
      P(fork[i]);
    }
    eat();//吃面条中...
    V(fork[i]);//放下餐叉（V操作）不会引起死锁
    V(fork[(i+1) % N]);
  }
}
```



## lab7

### 练习0

>#### 练习0：填写已有实验
>
>本实验依赖实验1/2/3/4/5/6。请把你做的实验1/2/3/4/5/6的代码填入本实验中代码中有“LAB1”/“LAB2”/“LAB3”/“LAB4”/“LAB5”/“LAB6”的注释相应部分。并确保编译通过。注意：为了能够正确执行lab7的测试应用程序，可能需对已完成的实验1/2/3/4/5/6的代码进行进一步改进。

需要修改的只有

trap_dispatch函数(trap/trap.c)

```c
static void
trap_dispatch(struct trapframe *tf) {
				...      
        /* LAB7 YOUR CODE */
        /* you should upate you lab6 code
         * IMPORTANT FUNCTIONS:
	       * run_timer_list
         */
        ticks ++;
        run_timer_list();//更新，使用run_timer_list函数
        break;
        ...
}
```



### 练习1

>#### 练习1: 理解内核级信号量的实现和基于内核级信号量的哲学家就餐问题（不需要编码）
>
>完成练习0后，建议大家比较一下（可用meld等文件diff比较软件）个人完成的lab6和练习0完成后的刚修改的lab7之间的区别，分析了解lab7采用信号量的执行过程。执行`make grade`，大部分测试用例应该通过。
>
>请在实验报告中给出内核级信号量的设计描述，并说明其大致执行流程。
>
>请在实验报告中给出给用户态进程/线程提供信号量机制的设计方案，并比较说明给内核级提供信号量机制的异同。

内核级信号量

结构体：内核级信号量中设置一个信号量当前值和一个信号量等待队列

P操作：

- 通过关开中断保证原子操作
- 如果信号量减一后还是大于0，则直接返回；否则申请一个等待项放入等待队列，并将该线程设置为等待状态，然后调用schedule函数进行调度
- 在该线程被唤醒的时候，还要将对应等待项从等待队列中删除掉，并检查唤醒线程的原因与线程进入等待状态的原因是否相同

V操作：

- 通过关开中断保证原子操作
- 先判断等待队列是否为空，如果为空，则信号量加一后返回；如果等待队列不为空，则从队列中找到对应线程，将其从等待状态变为就绪状态

在ucore中，down函数相当于调用信号量的P操作，up函数相当于调用信号量的V操作

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1giit4cttnvj317y0n0qft.jpg)

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1giit410hc2j31800n2n9m.jpg)

用户态进程/线程的信号量机制与内核相差不多，也要设置信号量当前值和信号量等待队列，但是进行PV操作时需要通过系统调用进入内核态执行



### 练习2

>#### 练习2: 完成内核级条件变量和基于内核级条件变量的哲学家就餐问题（需要编码）
>
>首先掌握管程机制，然后基于信号量实现完成条件变量实现，然后用管程机制实现哲学家就餐问题的解决方案（基于条件变量）。
>
>执行：`make grade` 。如果所显示的应用程序检测都输出ok，则基本正确。如果只是某程序过不去，比如matrix.c，则可执行
>
>```
>make run-matrix
>```
>
>命令来单独调试它。大致执行结果可看附录。
>
>请在实验报告中给出内核级条件变量的设计描述，并说明其大致执行流程。
>
>请在实验报告中给出给用户态进程/线程提供条件变量机制的设计方案，并比较说明给内核级提供条件变量机制的异同。
>
>请在实验报告中回答：能否不用基于信号量机制来完成条件变量？如果不能，请给出理由，如果能，请给出设计说明和具体实现。

对于条件变量的定义：

```c
typedef struct condvar{
    semaphore_t sem;        //条件变量对应队列 用semaphore结构体 即一个信号量表示
    int count;              //对应等待队列中的线程数
    monitor_t * owner;      //这个条件变量的宿主管程
} condvar_t;
```

假设有两个线程——线程A和线程B

先运行线程A，线程A因某种条件不满足进入等待状态后调度线程B，线程B在运行时检查到线程A的条件满足时，线程B进入等待状态并唤醒线程A，线程A执行完毕后，唤醒线程B继续执行

```c
typedef struct monitor{
    semaphore_t mutex;      //互斥锁，下图中对线程A中mutex成员的wait和signal操作类似于PV操作
    semaphore_t next;       //用于线程B的wait操作使线程B睡眠或线程A的signal操作唤醒线程B
    int next_count;         //执行signal操作将当前线程（线程A）唤醒的线程（线程B） 的个数
    condvar_t *cv;          //条件变量,线程A执行wait操作让线程A进入条件对应等待队列或线程B执行signal操作唤醒线程A
} monitor_t;

```

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1giirku8uz9j317w0n8avv.jpg)

1. 7处monitor.next_count++使计数加一，且9处sem_wait(monitor.next)使线程B进入等待状态，在线程A执行完后进入12的if分支，执行13处sem_signal(monitor.next)函数唤醒线程B继续运行
2. 0 3、5 17 是两对PV操作，即在线程B中5处通过sem_wait(monitor.mutex)函数申请对临界区的访问(P操作)，直到唤醒线程A后执行完线程A，再唤醒线程B后执行完线程B，才在17处释放对临界区的访问(V操作)

在sync/monitor.c中实现cond_signal和cond_wait函数，实现唤醒和等待功能

用down函数来执行P操作，用up函数来执行V操作

cond_signal函数

```c
// Unlock one of threads waiting on the condition variable. 
void 
cond_signal (condvar_t *cvp) {
   //LAB7 EXERCISE1: YOUR CODE
   cprintf("cond_signal begin: cvp %x, cvp->count %d, cvp->owner->next_count %d\n", cvp, cvp->count, cvp->owner->next_count);  
  /*
   *      cond_signal(cv) {
   *          if(cv.count>0) {
   *             mt.next_count ++;
   *             signal(cv.sem);
   *             wait(mt.next);
   *             mt.next_count--;
   *          }
   *       }
   */
  if(cvp->count>0) {//判断该条件变量对应等待队列中是否有等待的线程
    cvp->owner->next_count++;
    up(&(cvp->sem));
    down(&(cvp->owner->next));
    cvp->owner->next_count--;
  }//没有则相当于空操作
   cprintf("cond_signal end: cvp %x, cvp->count %d, cvp->owner->next_count %d\n", cvp, cvp->count, cvp->owner->next_count);
}
```

cond_wait函数

```c
// Suspend calling thread on a condition variable waiting for condition Atomically unlocks 
// mutex and suspends calling thread on conditional variable after waking up locks mutex. Notice: mp is mutex semaphore for monitor's procedures
void
cond_wait (condvar_t *cvp) {
    //LAB7 EXERCISE1: YOUR CODE
    cprintf("cond_wait begin:  cvp %x, cvp->count %d, cvp->owner->next_count %d\n", cvp, cvp->count, cvp->owner->next_count);
   /*
    *         cv.count ++;
    *         if(mt.next_count>0)
    *            signal(mt.next)
    *         else
    *            signal(mt.mutex);//release
    *         wait(cv.sem);
    *         cv.count --;
    */
    cvp->count++;
    if(cvp->owner->next_count>0)
      up(&(cvp->owner->next));
    else
      up(&(cvp->owner->mutex));
    down(&(cvp->sem));
    cvp->count --;
    cprintf("cond_wait end:  cvp %x, cvp->count %d, cvp->owner->next_count %d\n", cvp, cvp->count, cvp->owner->next_count);
}
```

实现哲学家就餐问题的解决方案

ucore通过phi_test_condvar函数来判断哲学家i在hungry状态是否可以eating

```c
void phi_test_condvar (i) { 
    if(state_condvar[i]==HUNGRY&&state_condvar[LEFT]!=EATING
            &&state_condvar[RIGHT]!=EATING) {
      //当哲学家i饥饿时，如果其左右的哲学家都不在eating状态，则哲学家i进入eating状态
        cprintf("phi_test_condvar: state_condvar[%d] will eating\n",i);
        state_condvar[i] = EATING ;
        cprintf("phi_test_condvar: signal self_cv[%d] \n",i);
        cond_signal(&mtp->cv[i]) ;//唤醒哲学家i对应进程
    }
}
```

补充phi_take_forks_condvar函数(sync/check_sync.c)

```c
/*
 *  void pickup(int i) {
 *      state[i] = hungry;
 *      if ((state[(i+4)%5] != eating) && (state[(i+1)%5] != eating)) {
 *        state[i] = eating;
 *      else
 *         self[i].wait();
 *   }
 */
void phi_take_forks_condvar(int i) {
     down(&(mtp->mutex));//占用临界区资源
//--------into routine in monitor--------------
     // LAB7 EXERCISE1: YOUR CODE
     // I am hungry
     // try to get fork
//--------leave routine in monitor--------------
      state_condvar[i] = HUNGRY;//将哲学家i设置为hungry状态
      phi_test_condvar(i);//调用test函数判断是否可以eating
      if(state_condvar[i] != EATING)//如果不满足条件，未能进入eating状态
        cond_wait(&mtp->cv[i]);     //则进入等待状态，等待邻座哲学家都放下餐叉后才被唤醒
      if(mtp->next_count>0)
         up(&(mtp->next));
      else
         up(&(mtp->mutex));//唤醒释放临界区资源
}
```

补充phi_put_forks_condvar函数(sync/check_sync.c)

```c
/*
 *   void putdown(int i) {
 *      state[i] = thinking;
 *      if ((state[(i+4)%5] == hungry) && (state[(i+3)%5] != eating)) {
 *          state[(i+4)%5] = eating;
 *          self[(i+4)%5].signal();
 *      }
 *      if ((state[(i+1)%5] == hungry) && (state[(i+2)%5] != eating)) {
 *          state[(i+1)%5] = eating;
 *          self[(i+1)%5].signal();
 *      }
 *   }
 */
void phi_put_forks_condvar(int i) {
     down(&(mtp->mutex));//占用临界区资源
//--------into routine in monitor--------------
     // LAB7 EXERCISE1: YOUR CODE
     // I ate over
     // test left and right neighbors
//--------leave routine in monitor--------------
     state_condvar[i] = THINKING;//哲学家线程状态变为thinking
     phi_test_condvar(LEFT);//检测左右邻座是否处于hungry状态，将处于hungry状态的邻座置位eating状态并唤醒
     phi_test_condvar(RIGHT);
     if(mtp->next_count>0)
        up(&(mtp->next));
     else
        up(&(mtp->mutex));//唤醒释放临界区资源
}
```

