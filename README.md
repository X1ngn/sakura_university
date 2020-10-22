# sakura_university

这是在sakura大学的摸爬滚打记录

前两个月

- csapplab
- ucore实验


<details>
  <summary>2020.10.20:搭建内核pwn环境</summary>
  
   -cpu kvm64,+smep,+smap 设置CPU的安全选项，这里开启了smap和smep
   
   -kernel 设置内核bzImage文件的路径 
   
   -initrd 设置刚才利用busybox创建的rootfs.img，作为内核启动的文件系统 
   
   -gdb tcp::1234 设置gdb的调试端口为1234
  
  
   内核中命令
   
   lsmod
   
   insmod xx.ko
   
   rmmod xx.ko
  
  
  
   gdb命令
   
   gdb vmlinux
   
   add-symbol-file ./xx.ko 0x(模块基地址) //加载驱动的符号文件
   
   target remote :1234
    
</details>

<details>
  <summary>2020.10.21:学习Double Fetch漏洞原理 && 复现2018 0CTF Finals Baby Kernel</summary>
</details>
