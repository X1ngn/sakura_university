这是参考网上的教程动手实现的一个仿pwndbg插件下gdb的mini调试器，在此过程中学习C++和ptrace的使用

实现pwndbg每运行一步都会显示寄存器的功能

实现了c、b、x、q的功能

仅支持

b 0xaaaaaaaa || b 12345678

c

x 0xaaaaaaaa || x 12345678

q
