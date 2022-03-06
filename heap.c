#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

void swap(int *x, int *y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}

void heapify(int *arr, int size, int subidx)
{
    int smallest = subidx;
    int leftidx = subidx * 2 + 2;
    int rightidx = subidx * 2 + 4;

    if (leftidx < size && arr[leftidx] < arr[smallest])
        smallest = leftidx;

    if (rightidx < size && arr[rightidx] < arr[smallest])
        smallest = rightidx;

    if (smallest != subidx)
    {
        swap(&arr[subidx], &arr[smallest]);         // swap readyAt
        swap(&arr[subidx + 1], &arr[smallest + 1]); // swap processIdx
        heapify(arr, size, smallest);
    }
}
