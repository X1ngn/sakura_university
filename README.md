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
  
  学习笔记：https://x1ng.top/2020/11/14/%E6%B1%87%E7%BC%96%E8%AF%AD%E8%A8%80%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/
  
</details>

<details>
  <summary>2020.10.23:补作业</summary>
</details>

<details>
  <summary>2020.10.24:补作业 && 看bytectf题</summary>
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
           target remote 127.0.0.1:1234
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

<details>
  <summary>2020.11.3:继续写了之前的调试器 && 学了一些stm32的知识</summary>
  调试器看了一些原理后还是得跟着网上写，实现了c、b、q、x的功能
</details>

<details>
  <summary>2020.11.4:补作业 && 看sctf 2020 Password Lock Plus逆向题wp</summary>
  毛概开题报告花了点时间
  
  大概看了一下google到[师傅的stm32的wp](https://xuanxuanblingbling.github.io/iot/2020/07/08/stm32/)，但是看得不是很懂
</details>

<details>
  <summary>2020.11.5:补作业 && 继续看sctf 2020 Password Lock Plus逆向题wp</summary>
</details>

<details>
  <summary>2020.11.6:补作业 && 学习stm引脚知识</summary>
  结合stm32中文手册看了四节课，才勉强看明白了sctf 2020 Password Lock Plus逆向题wp  or2
</details>


<details>
  <summary>2020.11.7:参加太湖杯</summary>
  打了一天还是卑微一解，mips的程序调用返回还是不太明白，，有空再多看几遍
  wp:https://x1ng.top/2020/11/08/%E5%A4%AA%E6%B9%96%E6%9D%AF-easyKooc-wp/
</details>


<details>
  <summary>2020.11.8:学习codeql</summary>
  搭环境就花了大半天or2，，看itergator的文档看不懂，找了几篇博客从0开始学
</details>

<details>
  <summary>2020.11.9:继续学习codeql && 刷几道leetcode简单题</summary>
  大概了解了语法，递归谓词还不是很理解，itegator的文档依旧看的不是很懂，看了一篇用codeql挖洞的文章
</details>

<details>
  <summary>2020.11.10:复习c++的迭代器 && 继续学习codeql</summary>
</details>

<details>
  <summary>2020.11.11:补全了之前calc的wp && 学焊电</summary>
</details>

<details>
  <summary>2020.11.12:补作业</summary>
</details>


<details>
  <summary>2020.11.13:补作业 && 学习arm程序调试</summary>
  arm下的调试跟mips还是差不多的
  
  直接运行
  
  `./filename`
  
  调试
  
   shell1:`qemu-mipsel -g 1234 -L /Path/buildroot/output/target/ test`
   
   shell2:
           ```
           gdb-multiarch ./test    
           target remote 127.0.0.1:1234
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
    p = process(["qemu-arm", "-L", "./", "./ma"])
  else:
    p = process(["qemu-arm", "-g", "1234", "-L", "./", "./ma"])
  ```
  
  另一个shell执行shell2的操作
  
</details>


<details>
  <summary>2020.11.14:复习了iofile泄露libc的利用方式 && 折腾了一下博客</summary>
  IOfile泄露地址学习笔记:https://x1ng.top/2020/11/14/IO-file%E6%B3%84%E9%9C%B2libc%E5%9C%B0%E5%9D%80%E5%AD%A6%E4%B9%A0/
</details>


<details>
  <summary>2020.11.15:期中考试复习</summary>
</details>

<details>
  <summary>2020.11.16:期中考试复习 && 复现Codegate2018 melong</summary>
  wp:https://x1ng.top/2020/11/16/arm-pwn%E5%85%A5%E9%97%A8%E4%B9%8B%E8%B7%AF/
</details>


<details>
  <summary>2020.11.17:期中考试复习 && 学习C++网络编程</summary>
</details>

<details>
  <summary>2020.11.18:期中考试复习 && 补作业</summary>
</details>

<details>
  <summary>2020.11.19:期中考试复习 && 补作业</summary>
</details>

<details>
  <summary>2020.11.20:期中考试复习 && 补作业 && 学习多线程gdb调试</summary>
  查看当前所有线程
  
  `i threads`
  
  切换跟踪线程
  
  `thread N`(N为gdb中的线程编号)
</details>

<details>
  <summary>2020.11.21:期中考试 && 参加祥云杯</summary>
</details>

<details>
  <summary>2020.11.22:参加祥云杯</summary>
</details>


<details>
  <summary>2020.11.23:复现西湖论剑IOT赛的两道PWN题 && 学习路由器处理HTTP请求方式</summary>
  wp:https://x1ng.top/2020/11/24/%E8%A5%BF%E6%B9%96%E8%AE%BA%E5%89%912020IOT%E9%97%AF%E5%85%B3%E8%B5%9B-PWN/
</details>


<details>
  <summary>2020.11.24:看了一些挖路由器的思路</summary>
  水了一天，感觉考试复习太浪费时间，申请免考跑几趟花了点时间，摸鱼
</details>


<details>
  <summary>2020.11.25:参加安洵杯</summary>
  wp:https://x1ng.top/2020/11/26/%E5%AE%89%E6%B4%B5%E6%9D%AF2020/
</details>


<details>
  <summary>2020.11.26:补作业</summary>
</details>

<details>
  <summary>2020.11.27:补作业</summary>
</details>

<details>
  <summary>2020.11.28:赶报告 && 复现强网杯一道路由器realworld赛题</summary>
</details>

<details>
  <summary>2020.11.29:继续复现路由器题</summary>
</details>

<details>
  <summary>2020.11.30:复现路由器题 && C++大作业</summary>
  wp:
</details>

<details>
  <summary>2020.12.1:了解AWD赛技巧</summary>
</details>

<details>
  <summary>2020.12.2:学习patch程序</summary>
  尝试动手写了一个通防脚本，，下周去试试能不能用
</details>
