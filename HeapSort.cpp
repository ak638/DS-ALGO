#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <stack>
#include <vector>
#include <map>
#include <string.h>

using namespace std;

#define PARENT(x) ((x)-1)/2
#define LC(x) (x)*2+1
#define RC(x) (x)*2+2

void ShowHeap(int arr[], int num)
{
	int cnt = 0;
	for (int i = 1; ; i *= 2)
	{
		int j = i;
		while (j-- && cnt < num)
		{
			printf("%d	\t", arr[cnt++]);
		}
		printf("\n");

		if (cnt >= num) break;
	}
	printf("\n");
}

void ShowArray(int arr[], int num)
{
	for (int i = 0; i < num; ++i) printf("%d\t", arr[i]);
	printf("\n");
}

void Swap(int arr[], int i, int j)
{
	arr[i] ^= arr[j];
	arr[j] ^= arr[i];
	arr[i] ^= arr[j];
}

void AdjustHeap(int arr[], int num, int idx)
{
	int target = arr[idx];
	int pos;

	for (pos = LC(idx); pos < num; pos = LC(pos))
	{
		if (pos+1 < num && arr[pos] < arr[pos+1]) pos++; //move to right child

		if (target >= arr[pos]) break; //got

		arr[idx] = arr[pos];
		idx = pos;
	}
	
	arr[idx] = target;
}

void BuildHeap(int arr[], int num)
{
	//start from last not-leaf node to adjust heap
	for (int pos = PARENT(num-1); pos >= 0; --pos)
	{
		AdjustHeap(arr, num, pos);
	}
}

void HeapSort(int arr[], int num)
{
	//建立最大堆
	BuildHeap(arr, num);

	//test
	ShowHeap(arr, num);
	ShowArray(arr, num);
	printf("=========\n");

	for (int i = num-1; i > 0; --i)
	{
		//交换最小值到最后一个
		Swap(arr, i, 0);
		//从顶开始调整最大堆
		AdjustHeap(arr, i, 0);

		//ShowHeap(arr, num);
	}

	ShowArray(arr, num);
}

int main(int argc, char *argv[])
{
	freopen("./input.txt", "r", stdin);
	//Solution poSolution;

	int testcase = 0;
	scanf("%d", &testcase);

	while (testcase--)
	{
		int num = 0;
		scanf("%d", &num);

		int arr[20];
		for (int i = 0; i < num; ++i) scanf("%d", &arr[i]);

		HeapSort(arr, num);
	}

	return 0;
}
