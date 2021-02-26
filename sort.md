# 十大经典排序算法


## 1. 冒泡排序 (bubble sort)

  ```
function bubbleSort(arr) {
    var len = arr.length;
    for (var i = 0; i < len - 1; i++) {
        for (var j = 0; j < len - 1 - i; j++) {
            if (arr[j] > arr[j+1]) {        // 相邻元素两两对比
                var temp = arr[j+1];        // 元素交换
                arr[j+1] = arr[j];
                arr[j] = temp;
            }
        }
    }
    return arr;
}
  ```



## 2. 选择排序（selection sort）

  ```
function selectionSort(arr) {
    var len = arr.length;
    var minIndex, temp;
    for (var i = 0; i < len - 1; i++) {
        minIndex = i;
        for (var j = i + 1; j < len; j++) {
            if (arr[j] < arr[minIndex]) {     // 寻找最小的数
                minIndex = j;                 // 将最小数的索引保存
            }
        }
        temp = arr[i];
        arr[i] = arr[minIndex];
        arr[minIndex] = temp;
    }
    return arr;
}
  ```



## 3.插入排序（Insertion Sort）

- 从第一个元素开始，该元素可以认为已经被排序；
- 取出下一个元素，在已经排序的元素序列中从后向前扫描；
- 如果该元素（已排序）大于新元素，将该元素移到下一位置；
- 重复步骤3，直到找到已排序的元素小于或者等于新元素的位置；
- 将新元素插入到该位置后；
- 重复步骤2~5。
  ```
function insertionSort(arr) {
    var len = arr.length;
    var preIndex, current;
    for (var i = 1; i < len; i++) {
        preIndex = i - 1;
        current = arr[i];
        while (preIndex >= 0 && arr[preIndex] > current) {
            arr[preIndex + 1] = arr[preIndex];
            preIndex--;
        }
        arr[preIndex + 1] = current;
    }
    return arr;
}
  ```
  
  
  
## 4. 希尔排序（Shell Sort）
  ```
function shellSort(arr) {
    var len = arr.length;
    for (var gap = Math.floor(len / 2); gap > 0; gap = Math.floor(gap / 2)) {
        // 注意：这里和动图演示的不一样，动图是分组执行，实际操作是多个分组交替执行
        for (var i = gap; i < len; i++) {
            var j = i;
            var current = arr[i];
            while (j - gap >= 0 && current < arr[j - gap]) {
                 arr[j] = arr[j - gap];
                 j = j - gap;
            }
            arr[j] = current;
        }
    }
    return arr;
}
  ```



## 5. 归并排序（Merge Sort）
  ```
/* 将序列对半拆分直到序列长度为1*/
void MergeSort_UptoDown(int *num, int start, int end) {
	int mid = start + (end - start) / 2;
	if (start >= end) {
		return;
	}
	MergeSort_UptoDown(num, start, mid);
	MergeSort_UptoDown(num, mid + 1, end);
	Merge(num, start, mid, end);
}
  ```
  ```
void Merge(int *num, int start, int mid, int end) {
     int *temp = (int *)malloc((end-start+1) * sizeof(int));    //申请空间来存放两个有序区归并后的临时区域
     int i = start;
     int j = mid + 1;
     int k = 0;

     while (i <= mid && j <= end) {
     	if (num[i] <= num[j]){
     		temp[k++] = num[i++];
     	} else {
     		temp[k++] = num[j++];
     	}
     }
     
     while (i <= mid) {        
    	temp[k++] = num[i++];     
     }
     while (j <= end) {
    	temp[k++] = num[j++];
     }
     
     //将临时区域中排序后的元素，整合到原数组中
     for (i = 0; i < k; i++) {
     	num[start + i] = temp[i];
     }
    
  ```



## 6. 快速排序（Quick Sort））

- 首先取出一个key，一般取第一个元素
- 从后往前遍历，如果数组中的数据小于了key，那么就将从前往后未比较过的第一个位置即fisrt位置替换为该数据
- 然后从前往后遍历，如果数组中的数据大于了key，那么就将从后往前的第一个比较过数据位置替换
- 直到左右两边的位置重合，说明key就找到了正确的位置，每次循环就能找到一个数的正确位置
- 然后将key左右两边的数据分为两组，递归调用自己。
1. 代码实现
  ```
//快速排序
void quickSort(int arr[], int low, int high)
{
    int first = low;
    int last  = high;
    int key = arr[first];
    if(low >= high)
        return;
    while(first < last){
        while(first < last && arr[last] > key){
            last--;
        }
     	if (first < last) 
        	arr[first] = arr[last];

        while(first < last && arr[first] < key){
            first++;
        }
        if (first < last) 
        	arr[last] = arr[first];
    }
    arr[first] = key;
    
    quickSort(arr, low, first-1);
    quickSort(arr, first+1, high);
}
  ```



## 7. 堆排序（Heap Sort）

堆排序（Heapsort）是指利用堆这种数据结构所设计的一种排序算法。堆积是一个近似完全二叉树的结构，并同时满足堆积的性质：即子结点的键值或索引总是小于（或者大于）它的父节点。

1. 第1种方式代码

  ```
var len;    // 因为声明的多个函数都需要数据长度，所以把len设置成为全局变量

function buildMaxHeap(arr) {   // 建立大顶堆
    len = arr.length;
    for (var i = Math.floor(len/2); i >= 0; i--) { // len/2 获取的是(从左往右)倒数第二排的最后一个节点i
        heapify(arr, i);						   // 堆调整，获取i(父节点)和两个子节点的最大值；
    }
}
  ```

  ```
function heapify(arr, i) {     // 堆调整
    var left = 2 * i + 1,
        right = 2 * i + 2,
        largest = i;

    if (left < len && arr[left] > arr[largest]) {
        largest = left;
    }
     
    if (right < len && arr[right] > arr[largest]) {
        largest = right;
    }
     
    if (largest != i) {
        swap(arr, i, largest);  //交换父节点和子节点的最大值
        heapify(arr, largest);	//交换完成之后，子节点作为父节点再次迭代计算；
    }
}
  ```

  ```
function swap(arr, i, j) {
    var temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}
  ```

  ```
function heapSort(arr) {
    buildMaxHeap(arr); //初始构建一个大顶堆(第一个父节点为最大)

    for (var i = arr.length - 1; i > 0; i--) {
        swap(arr, 0, i); //将第一个最大的父节点和二叉树的最后一个节点交换
        len--;			 //交换一次之后，末端len节点为最大值，不需要在参与构造大顶堆；
        heapify(arr, len);//构造交换之后的大顶堆
    }
    return arr;
}
  ```



## 8. 计数排序（Counting Sort）

- 找出待排序的数组中最大和最小的元素；
- 统计数组中每个值为i的元素出现的次数，存入数组C的第i项；
- 对所有的计数累加（从C中的第一个元素开始，每一项和前一项相加）；
- 反向填充目标数组：将每个元素i放在新数组的第C(i)项，每放一个元素就将C(i)减去1。

1. 代码实现
  ```
function countingSort(arr, maxValue) { //maxValue为数组元素中的最大值
    var bucket = new Array(maxValue + 1),
        sortedIndex = 0;
        arrLen = arr.length,
        bucketLen = maxValue + 1;

    for (var i = 0; i < arrLen; i++) {
        if (!bucket[arr[i]]) {
            bucket[arr[i]] = 0;
        }
        bucket[arr[i]]++;
    }
     
    for (var j = 0; j < bucketLen; j++) {
        while(bucket[j] > 0) {
            arr[sortedIndex++] = j;
            bucket[j]--;
        }
    }
     
    return arr;
}
  ```


## 9. 桶排序（Bucket Sort）
- 设置一个定量的数组当作空桶；
- 遍历输入数据，并且把数据一个一个放到对应的桶里去；
- 对每个不是空的桶进行排序；
- 从不是空的桶里把排好序的数据拼接起来。 
1. 代码实现
  ```
function bucketSort(arr, bucketSize) {
    if (arr.length === 0) {
      return arr;
    }

    var i;
    var minValue = arr[0];
    var maxValue = arr[0];
    for (i = 1; i < arr.length; i++) {
      if (arr[i] < minValue) {
          minValue = arr[i];                // 输入数据的最小值
      } else if (arr[i] > maxValue) {
          maxValue = arr[i];                // 输入数据的最大值
      }
    }
     
    // 桶的初始化
    var DEFAULT_BUCKET_SIZE = 5;            // 设置桶的默认数量为5
    bucketSize = bucketSize || DEFAULT_BUCKET_SIZE;
   //这一步可以保证每个桶中的是从小到大分配的(但是桶中的元素可能不是顺序排列，需要插入排序进行重新排列一次，然后拼接在一起)
    var bucketCount = Math.floor((maxValue - minValue) / bucketSize) + 1; 
    var buckets = new Array(bucketCount);
    for (i = 0; i < buckets.length; i++) {
        buckets[i] = [];
    }

    // 利用映射函数将数据分配到各个桶中
    for (i = 0; i < arr.length; i++) {
        buckets[Math.floor((arr[i] - minValue) / bucketSize)].push(arr[i]);
    }
     
    arr.length = 0;
    for (i = 0; i < buckets.length; i++) {
        insertionSort(buckets[i]);                      // 对每个桶进行排序，这里使用了插入排序
        for (var j = 0; j < buckets[i].length; j++) {
            arr.push(buckets[i][j]);                     
        }
    }
     
    return arr;
}
  ```


## 10.基数排序（Radix Sort）

基数排序(Radix Sort)是**[桶排序](http://www.cnblogs.com/skywang12345/p/3602737.html)**的扩展，它的基本思想是：将整数按位数切割成不同的数字，然后按每个位数分别比较。
具体做法是：将所有待比较数值统一为同样的数位长度，数位较短的数前面补零。然后，从最低位开始，依次进行一次排序。这样从最低位排序一直到最高位排序完成以后, 数列就变成一个有序序列。

- 按照个位数进行排序。
- 按照十位数进行排序。
-  按照百位数进行排序。
- 排序后，数列就变成了一个有序序列。

1. 代码实现
  ```
int get_max(int a[], int n) {
    int i, max;
    max = a[0];
    for (i = 1; i < n; i++)
    	if (a[i] > max)
    		max = a[i];
    return max;
}

void count_sort(int a[], int n, int exp)
{
    int output[n];             // 存储"被排序数据"的临时数组
    int i, buckets[10] = {0};

    // 将数据出现的次数存储在buckets[]中
    for (i = 0; i < n; i++)
    	buckets[ (a[i]/exp)%10 ]++;

    // 更改buckets[i]。目的是让更改后的buckets[i]的值，是该数据在output[]中的位置。
    for (i = 1; i < 10; i++)
    	buckets[i] += buckets[i - 1];

    // 将数据存储到临时数组output[]中
    for (i = n - 1; i >= 0; i--) {
    	output[buckets[ (a[i]/exp)%10 ] - 1] = a[i];
    	buckets[ (a[i]/exp)%10 ]--;
    }

    // 将排序好的数据赋值给a[]
    for (i = 0; i < n; i++)
    	a[i] = output[i];
}
  
void radix_sort(int a[], int n){
    int exp;    // 指数。当对数组按各位进行排序时，exp=1；按十位进行排序时，exp=10；...
    int max = get_max(a, n);    // 数组a中的最大值

    // 从个位开始，对数组a按"指数"进行排序
    for (exp = 1; max/exp > 0; exp *= 10)
    	count_sort(a, n, exp);
 }
  ```
