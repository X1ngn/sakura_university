# sakura_university

这是在sakura大学的摸爬滚打记录

前两个月

- csapplab
- ucore实验


<details>
  <summary>2020.10.20:搭建内核pwn环境</summary>
  
  qemu参数：
  
   -cpu kvm64,+smep,+smap 设置CPU的安全选项，这里开启了smap和smep
   
   -kernel 设置内核bzImage文件的路径 
   
   -initrd 设置刚才利用busybox创建的rootfs.img，作为内核启动的文件系统 
   
   -gdb tcp::1234 设置gdb的调试端口为1234
  
  
   内核中命令：
   
   lsmod
   
   insmod xx.ko
   
   rmmod xx.ko
  
  
  
   gdb命令：
   
   gdb vmlinux
   
   add-symbol-file ./xx.ko 0x(模块基地址) //加载驱动的符号文件
   
   target remote :1234
    
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
