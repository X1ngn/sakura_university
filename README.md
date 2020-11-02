# sakura_university

这是在sakura大学的摸爬滚打记录

前两个月

- csapplab
- ucore实验


<details>
  <summary>2020.10.20:搭建内核pwn环境</summary>
  qemu参数：
  
  ```
   -cpu kvm64,+smep,+smap 设置CPU的安全选项，这里开启了smap和smep
   -kernel 设置内核bzImage文件的路径
   -initrd 设置刚才利用busybox创建的rootfs.img，作为内核启动的文件系统
   -gdb tcp::1234 设置gdb的调试端口为1234
  ```
  
   内核中命令：
   
   ```
   lsmod
   insmod xx.ko
   rmmod xx.ko
   ```
  
   gdb命令：
   
   ```
   gdb vmlinux
   add-symbol-file ./xx.ko 0x(模块基地址) //加载驱动的符号文件
   target remote :1234
   ```
</details>

<details>
  <summary>2020.10.21:学习Double Fetch漏洞原理 && 复现2018 0CTF Finals Baby Kernel</summary>
</details>

<details>
  <summary>2020.10.22:复现2018 0CTF Finals Baby Kernel && 学习MIPS架构汇编语言</summary>
  wp：https://x1ng.top/2020/10/22/kernel-pwn%E5%85%A5%E9%97%A8%E4%B9%8B%E8%B7%AF-%E6%9B%B4%E6%96%B0%E4%B8%AD/
  
  学习笔记：https://x1ng.top/2020/10/22/%E6%B1%87%E7%BC%96%E8%AF%AD%E8%A8%80%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0-%E6%9B%B4%E6%96%B0%E4%B8%AD/
  
</details>

<details>
  <summary>2020.10.23:补作业</summary>
</details>

<details>
  <summary>2020.10.24:看bytectf题 && 看工控ctf题</summary>
  学了一下golang语言的竞争漏洞
  又是零解的一天呜呜呜
</details>

<details>
  <summary>2020.10.25:<s>继续看题</s> 做不出题 && 看了一些ARM架构汇编</summary>
</details>

<details>
  <summary>2020.10.26:重新装了梯子 && 看了下pwnable.tw的CVE-2018-1160</summary>
  看着ida的反汇编有点迷，准备先复现一遍，，环境又搭不好
</details>

<details>
  <summary>2020.10.27:补作业 && 研究byte的wp</summary>
</details>

<details>
  <summary>2020.10.28:补作业</summary>
</details>

<details>
  <summary>2020.10.29:装mips pwn环境</summary>
</details>

<details>
  <summary>2020.10.30:看了一些RTOS的知识</summary>
  看到一个比赛有道rt-thread的题，去学了一下
</details>

<details>
  <summary>2020.10.31:<s>继续学RTOS</s> 做不出题 && 看了pwnable.tw上calc的思路 && 参加了一个AWD的训练赛</summary>
  等RTOS的wp
  
  还没打过AWD，然而两个小时没有队做出pwn来，感觉白白浪费了时间看一道堆题
</details>

<details>
  <summary>2020.11.1:培训新生</summary>
</details>

<details>
  <summary>2020.11.2:学习mips架构下gdb调试 && 复现西湖论剑的mips pwn题 massagesystem</summary>
  主要跟着这个师傅的博客 https://ray-cp.github.io/archivers/MIPS_Debug_Environment_and_Stack_Overflow 走
  
  静态编译  mipsel-linux-gcc -o test test.c -static
  
  qemu运行  qemu-mipsel ./test
  
  调试
  
   shell1:`qemu-mipsel -g 1234 -L /Path/buildroot/output/target/ test`
   
   shell2:
           ```
           gdb-multiarch ./test
           target remote 127.0.0.1:1234`
           ```
  
  与pwntools结合
  
  ```
  from pwn import*
  import sys

  context.binary = "ma"
  context.arch='mips'
  context.log_level = "debug"

  if sys.argv[1] == "r":
    p = remote("remote_addr", remote_port)
  elif sys.argv[1] == "l":
    p = process(["qemu-mipsel-static", "-L", "./", "./ma"])
  else:
    p = process(["qemu-mipsel-static", "-g", "1234", "-L", "./", "./ma"])
  ```
  
  另一个shell执行shell2的操作
  
  复现wp:https://x1ng.top/2020/11/02/mips-pwn%E5%85%A5%E9%97%A8%E4%B9%8B%E8%B7%AF/
</details>
