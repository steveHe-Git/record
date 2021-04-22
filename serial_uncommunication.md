## 串口不同的调试经验

```cpp
现象: 重启19A会出现一定概率的串口通信不通的现象；
 
排查方式：
    cat /proc/tty/driver/ttyAMA
    查看串口的读写情况；(可以直接看串口的tx rx的读写情况)
    
    dmesg|grep tty
	查看所有的串口的信息;

	cat /proc/interrupts
    查看硬件中断的情况
        
    还有就是串口有没有被其他的进程打开过
    //设置串口波特率
    手动执行stty -F /dev/ttyAMA2 raw speed 115200 cs8 -parenb -cstopb
    之后看看数据是否能自动恢复正常
    
    cat /var/log/messages |grep "tty"
    通过日志去查哪些串口被哪些进程打开
```

