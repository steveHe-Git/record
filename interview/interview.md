# 面试经验

## 面试题目

```cpp
1. 进程之间的调度方式；
2. 进程通信有哪几种；优缺点；
3. new和malloc的区别
```

## 面试题目

```cpp
1. 求一个uint32位的 二进制的1 有多少位？
    int count_one(int n) {
    	int count = 0;
    	while(num) {
            ++count
            num &= (num -1); 
        }
    	return count;
	}
	#解析： num &= (num -1); 很巧妙，假设num = 0111 ,num-1= 0110,两者相&，如果num是奇数，最后一位必然是1，如果相&之后的结果不大于0，说明1的个数没有了； &之后的结果为num=0110，为偶数num-1= 0101， 那最后相邻的两位进行查看10，01，当时偶数时，两者相与必然为00，可以巧妙的计算第二位存不存在1; 然后相&的结果为 num = 0100,刚好可以判断第三位是否为0，依次类推，当num不大于0，即可统计1的个数

2. 提取一个 IP 表示的int整数；
    uint32_t ipToInt(string ip) {
    	//判断一个ip是否合法
    	if (!isIP(ip)) {
            return 0;
        }
    	
    	vector<int>ipList;
    	int len = ip.size();
    	for (int i=0; i < len; i++) {
            int j = i;
            while(j < len && ip[j] != '.') {
                j++;
            }
            ipList.push_back(atoi(ip.substr(i, j-i)));
            i = j;
        }
    
    	uint32_t res = 0;
    	int list_len = ipList.size();
    	for (int i =0 ; i < list_len; i++) {
            res = res << 8 | ipList[i];
        }
    	return res;
	}

	bool isIp(string str) {
        vector<string> str = split(str, '.');
        for (int i = 0; i < str.size(); ++i) {
			if (str.size != 4 || str[i].empty()) {
                return false;
            } else {
                if (atoi(str[i])<0 || atoi(str[i]) > 255) {
                    return false;
                }
            }
        }
        return true;
    }
    
3. 在c++中string 没有直接分割的函数，可以利用c的strtok函数封装一个分割方法
    char *strtok(char *str, const char *delim) 
    该函数返回被分解的第一个子字符串，如果没有可检索的字符串，则返回一个空指针。
    
    示例：
    1. strtok只要碰到一次delim,就会将delim替换成'\0'；
    2. 若要在第一次提取子串完毕之后，继续对源字符串s进行提取，应在其后（第二次，第三次。。。第n次）的调用中将strtok的第一个参数赋为空值NULL(表示函数继续从上一次调用隐式保存的位置，继续分解字符串；对于上述的第二次调用来说，第一次调用结束前用一个this指针指向了分界符的下一位);
	3. 如果大于两次调用，再一次传递str，而不是NULL，相当于还从字符串的开头查找分界符delim，而且此时buffer已经被修改(delim第一次被修改为了'\0'),因此是找不到delim的；
	//c++版本        
    vector<string> split(const string& str, const string& delim) {
        vector<string> res;
        if (str == "") {
            return res;
        }
        char* s = new char[str.length() + 1]; //char* 都是字符串字面值，后面都会有‘\0’;
        strcpy(s, str.c_str());
        
        char* d = new char[delim.length() + 1];
        strcpy(d, delim.c_str());
        
        char* token = strtok(s, d);
        
        while(token != NULL) {
            res.push_back(string(token);
            token = strtok(NULL, delim);
        }
        return res;
    }
	
	//c语言版本
    type define struct {
        char** res_str;
        int num;
    }splitResult;                 
    void split(char* str, char* delim, splitResult* res) {
        int count = 0;
        char* token; = strtok(str, delim);
        
        while(token != NULL) {
            ++count;
            *(res->res_str)++ = token;
            token = strtok(NULL, delim);
        }
        res->num = count;
    }
```



