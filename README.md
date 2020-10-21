# sakura_university

这是在sakura大学的摸爬滚打记录
前两个月
- csapplab
- ucore实验

<details>
<summary>2020.10.20:搭建内核pwn环境 && 看了一些内核pwn的博客</summary>
  `aaa`
  ``` qemu-system-x86_64 -kernel ~/tools/linux-5.2.1/arch/x86_64/boot/bzImage -initrd ~/tools/busybox-1.31.0/rootfs.img 
  -append "console=ttyS0 root=/dev/ram rdinit=/sbin/init" \
  -cpu kvm64,+smep,+smap \
  -nographic \
  -gdb tcp::1234
  ``` 
  
  > -cpu kvm64,+smep,+smap 设置CPU的安全选项，这里开启了smap和smep
  > 
  > -kernel 设置内核bzImage文件的路径
  > 
  > -initrd 设置刚才利用busybox创建的rootfs.img，作为内核启动的文件系统
  > 
  > -gdb tcp::1234 设置gdb的调试端口为1234

</details>
