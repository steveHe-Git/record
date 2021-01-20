# 日志彩色打印
## 日志打印的格式
  ```
echo -e "\033[41m Hello World Red\033[0m"
  ```
-  echo -e是激活特殊字符格式，这不必多说
-  “\033”引导非常规字符序列
-  “m”意味着设置属性然后结束非常规字符序列

1. 数字代表颜色
|          | 字颜色    |   背景颜色|
|:--------:|:--------:|:--------:|
|红色       |31        |        41|
|绿色       |32        |42        |
|黄色       |33        |  43      |
|蓝色       |34        |44        |
|紫色       |35        |45        |
|蓝绿色     |36         |46       |
|正常       |37        |40        |

3. 特殊效果
| Column 1 | Column 2 |
|:--------:|:--------:|
|0		   |OFF       |
|1		   |高亮显示   |
|4		   |下划线     |
|5		   |闪烁      |
|7		   |反白显示   |
|8		   |不可见     |

4. 实例

echo -e "\033[44;31;5m That is extremely cool \033[0m really?"
1. 起作用的是 44;31;5，这个就是蓝色背景，红色字体，闪烁
2. 这三个参数的摆放位置是无关
3. 最后要回归正常\033[0m

  ```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <sys/timeb.h>
#define L_BLUE      "\e[1;34m"
#define log_time(color)\
    do\
    {\
        struct tm *t;\
        struct timeb stTimeb;\
        ftime(&stTimeb);\
        t = localtime(&stTimeb.time); \
        printf(color "[%4d%02d%02d-%02d:%02d:%02d:%03d]", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, stTimeb.millitm); \
        fflush(stdout);\
        printf("\033[0m"); \
    } while (0)
  ```
  ```
#define LOG_INFO(format, ...)   \
    do\
    {\
        log_time(L_CYAN);\
        printf(L_CYAN "[INFO][%s][%s][%d]" format "\r\n", __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
        printf(NONE);\
    } while (0)
  ```