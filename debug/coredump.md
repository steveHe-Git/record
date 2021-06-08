vi /etc/sysctl.conf
加上如下两句话
kernel.core_pattern = /var/core/core_%e_%p
kernel.core_uses_pid = 0

%c 转储文件的大小上限
%e 所dump的文件名
%g 所dump的进程的实际组ID
%h 主机名
%p 所dump的进程PID
%s 导致本次coredump的信号
%t 转储时刻(由1970年1月1日起计的秒数)
%u 所dump进程的实际用户ID

可以使用以下命令，使修改结果马上生效。
sysctl –p /etc/sysctl.conf


/etc/profile/rc.local
# 设置开机自启动
ulimit -c unlimited
sysctl –p /etc/sysctl.conf


//gdb 调试coredump文件
gdb 可执行文件 coredump文件
bt 查看堆栈信息

//gdb 调试coredump文件 文件函数名显示？？问题 
打印输出 
#0  0x4022b178 in ?? ()
#1  0x4022b134 in ?? ()
#2  0x4022b134 in ?? ()

解决方案：由于gdb -c core这样的使用在有些系统下支持不是很好，所以推荐用如下方法：
1. gdb exe
2. core-file coredump文件

另外一种方法
#查看共享库信息
(gdb) info sharedlibrary
From        To          Syms Read   Shared Object Library
                        No          /lib/libpthread.so.0
                        No          /lib/libgcc_s.so.1
                        No          /lib/libc.so.6
                        No          /lib/ld-linux.so.3
#发现共享库的路径都是绝对路径,所以要设置库的根路径.
(gdb) set sysroot /work/xxx/xxx/ (设置共享库的绝对路径)
#载入要调试的二进制文件
(gdb) file xxx
#载入core dump文件
(gdb) core-file core
Program terminated with signal SIGSEGV, Segmentation fault.
#0 0x402084b4 in strlen () from /work/nfs_root/rootfs_NodeTest1.1/lib/libc.so.6

这样就找到了出现段错误的地方了,原来是调用strlen出错了,后面再根据哪里调用strlen找具体出错的地方.
这里遇到了一个坑,就是set sysroot,,之前用set solib-search-path和set solib-absolute-prefix,出错的地方都是显示问号,换成set sysroot就可以了,应该还是共享库的路径问题,有空再仔细深纠下.

#出现问号的可能原因
(gdb) file /usr/local/bin/fj_serviced 
Reading symbols from /usr/local/bin/fj_serviced...(no debugging symbols found)...done.

1. 编译时未加调试选项（(g)cc/g++  -g ... ）
2. strip了(通过file 可执行文件  就能知道这个可执行文件有没有被strip)
3. gdb加了-g仍然(no debugging symbols found) Makefile的flag写的不对，查找，发现有个-s的选项，查资料发现，原来是它把我的符号给去掉了，去掉-s
	其实-s我理解的是strip

