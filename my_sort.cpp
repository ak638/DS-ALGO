#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string>
#include <vector>

#include <utility>

void ShowArray(int arr[], int n)
{
	for (int i = 0; i < n; ++i)
	{
		printf("%d ", arr[i]);
	}
	putchar('\n');
}

//=========SelectSort BEGIN=========
int SelectSort(int arr[], int n)
{
	//选择排序
	//0->i, i->[0,n), 维护有序状态，每次选择一个最小的追加到有序队列的后面
	
	int cur_min = 0;	
	int cur_min_idx = 0;
	
	for (int i = 0; i < n-1; ++i)
	{
		//i是当前需要放最小值的位置
		cur_min = arr[i];
		cur_min_idx = i;

		for (int j = i+1; j < n; ++j)
		{
			if (cur_min > arr[j]) 
			{
				cur_min = arr[j];
				cur_min_idx = j;
			}
		}

		if (i != cur_min_idx) std::swap(arr[i], arr[cur_min_idx]);
	}

	return 0;
}
//=========SelectSort END=========

//=========BubbleSort BEGIN=========
int BubbleSort(int arr[], int n)
{
	//冒泡排序
	//冒泡滚动最大的数到末端，跟选择排序倒过来，有序状态在末端形成
	
	for (int i = n-1; i > 0; --i)
	{
		//i是当前循环最大值放的位置，i之后都是已经排序了的；
		for (int j = 1; j <= i; ++j)
		{
			if (arr[j-1] > arr[j]) std::swap(arr[j-1], arr[j]);
		}
	}

	return 0;
}

//=========BubbleSort END=========


//=========InsertSort BEGIN=========
int InsertSort(int arr[], int n)
{
	//插入排序，可配合在快排对小数据量进行使用
	//跟选择排序类似，把有序状态维护在前面，每次把有序后面的第一个无序插进去
	
	for (int i = 1; i < n; ++i)
	{
		//i是第一个无序位置，需要往前面寻找合适的位置
		int target = arr[i];
		//j为可填充的位置
		int j = i;
		for ( ; j > 0; --j)
		{
			if (arr[j-1] > target) arr[j] = arr[j-1];
			else break;
		}

		arr[j] = target;
	}

	return 0;
}
//=========InsertSort END=========

//=========ShellSort BEGIN=========
int ShellSort(int arr[], int n)
{
	//希尔排序
	//减少逆序对调整的次数，譬如调整一次，两个逆序对修复过来
	
	//跨度delta指数下降
	for (int delta = n >> 1; delta > 0; delta >>= 1)
	{
		//里面两重循环差不多就是插入排序的过程
		for (int i = delta; i < n; ++i)
		{
			int target = arr[i];
			int j = i;
			//for ( ; j > 0; j -= delta) //WRONG! j-delta may <0 !
			for ( ; j >= delta; j -= delta)
			{
				if (arr[j-delta] > target) arr[j] = arr[j-delta];
				else break;
			}

			arr[j] = target;
		}

	}

	return 0;
}

//=========ShellSort END=========

//=========MergeSort BEGIN===========

void Merge(int arr[], 
			int l_low, int l_high, 
			int r_low, int r_high, 
			int tmp[])
{
	int i = l_low;
	int j = r_low;
	int k = 0;

	for ( ; i <= l_high && j <= r_high; )
	{
		if (arr[i] <= arr[j]) tmp[k++] = arr[i++];
		else tmp[k++] = arr[j++];
	}

	//copy rest to tmp
	if (i <= l_high) std::copy(arr+i, arr+l_high+1, tmp+k);
	if (j <= r_high) std::copy(arr+j, arr+r_high+1, tmp+k);

	//copy tmp back to arr
	std::copy(tmp, tmp + (l_high-l_low+1) + (r_high-r_low+1), arr+l_low);
}

int MSort(int arr[], int low, int high, int tmp[])
{
	if (low < high)
	{
		int mid = (low + high) >> 1;
		MSort(arr, low, mid, tmp);
		MSort(arr, mid+1, high, tmp);
		Merge(arr, low, mid, mid+1, high, tmp);
	}

	return 0;
}

int MergeSort(int arr[], int n)
{
	//归并排序
	//divide&conquer
	
	int *tmp = new int[n]; //归并用
	MSort(arr, 0, n-1, tmp);

	return 0;
}

//=========MergeSort END===========


//=========HeapSort BEGIN===========
#define LEFT_CHILD(x) (((x) << 1) + 1)
#define RIGHT_CHILD(x) ((x+1) << 1)
#define PARENT(x) ((x-1) >> 1)

void AdjustUp(int arr[], int n, int pos)
{
	//在堆排序里面不会使用，最为最小堆，如果向堆push，
	//先放到最后面，然后从它开始由下往上调整
	
	int target = arr[pos];

	int parent = 0;
	int cur = pos;
	
	for ( ; cur > 0; cur = parent)
	{
		parent = PARENT(cur);
		if (arr[parent] < target) arr[cur] = arr[parent];
		else break;
	}

	arr[cur] = target;
}

void AdjustDown(int arr[], int n, int pos)
{
	int target = arr[pos];

	int child = 0;
	int cur = pos;
	for ( ; LEFT_CHILD(cur) < n; cur = child)
	{
		//选择最大的一个子节点
		child = LEFT_CHILD(cur);
		if (child+1 < n && arr[child] < arr[child+1]) ++child;
		
		if (arr[child] > target) arr[cur] = arr[child];
		else break;
	}

	arr[cur] = target;
}

void BuildHeap(int arr[], int n)
{
	//从第一个非叶子节点开始调整，k为非叶子节点个数，最多比较2k次，swap k次
	//所以整体O(n)的复杂度
	
	for (int i = PARENT(n-1); i >= 0; --i)
	{
		AdjustDown(arr, n, i);
	}
}

int HeapSort(int arr[], int n)
{
	//堆排序
	//建立最大堆，每次把最大的swap到最后面

	BuildHeap(arr, n);

	//循环n-1次够了
	for (int i = n-1; i > 0; --i)
	{
		std::swap(arr[0], arr[i]);

		AdjustDown(arr, i, 0);
	}

	return 0;
}


//=========HeapSort END===========

//=========QSort BEGIN===========
int SelectPivot(int arr[], int low, int high)
{
	//median3, instead of random
	int mid = (low + high) >> 1;
	if (low == mid) return arr[low]; //only two left

	//printf("BEFORE %d %d %d\n", arr[low], arr[mid], arr[high]);

	//arr[low] arr[mid] arr[high]
	//把大的放在high位置，免得后面Parition再移动一次
	//最终把中间的放在low位置
	
	if (arr[high] < arr[low]) std::swap(arr[high], arr[low]);
	if (arr[high] < arr[mid]) std::swap(arr[high], arr[mid]);
	//now max at high
	
	if (arr[low] < arr[mid]) std::swap(arr[low], arr[mid]);

	//printf("AFTER %d %d %d\n", arr[low], arr[mid], arr[high]);
	
	return arr[low];
}

int Partition(int arr[], int low, int high)
{
	int pivot = SelectPivot(arr, low, high);
	//now pivot at low
	
	int i = low;
	int j = high;
	while (i < j)
	{
		while (i < j && arr[j] >= pivot) --j;
		arr[i] = arr[j];
		while (i < j && arr[i] <= pivot) ++i;
		arr[j] = arr[i];
	}

	arr[i] = pivot;

	return i;
}

int QSort(int arr[], int low, int high)
{
	if (low < high)
	{
		int mid = Partition(arr, low, high);
		QSort(arr, low, mid-1);
		QSort(arr, mid+1, high);
	}
	
	return 0;
}

int QSort_v2(int arr[], int low, int high)
{
	//在数据量小的时候，改用插入排序，减少递归消耗
	int cutoff = 3;

	if (low + cutoff <= high)
	{
		int mid = Partition(arr, low, high);
		QSort(arr, low, mid-1);
		QSort(arr, mid+1, high);
	}
	else
	{
		InsertSort(arr+low, high-low+1);
	}

	return 0;
}

//=========QSort END===========

int Sort(int arr[], int n)
{
	if (!arr || n < 0) return -1;

	//快排
	//QSort(arr, 0, n-1);
	
	//快排+插入排序优化
	//QSort_v2(arr, 0, n-1);
	
	//选择排序	
	//SelectSort(arr, n);
	
	//冒泡排序
	//BubbleSort(arr, n);

	//插入排序
	//InsertSort(arr, n);
	
	//希尔排序
	//ShellSort(arr, n);
	
	//归并排序
	//MergeSort(arr, n);

	//堆排序
	HeapSort(arr, n);

	return 0;
}


int main(int argc, char *argv[])
{
	freopen("./input.txt", "r", stdin);

	int arr[128] = {0};

	int test_case = 0;
	
	int n = 0;
	scanf("%d", &n);

	for (int i = 0; i < n; ++i)
	{
		scanf("%d", &arr[i]);
	}

	ShowArray(arr, n);

	Sort(arr, n);

	ShowArray(arr, n);

	return 0;
}

/*
 * test cases:
8
4 10 7 9 4 8 6 1

4 3 7 1 4 2 6 1
4 4 4 4 4 4 4 4
 */
