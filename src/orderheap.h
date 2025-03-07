#ifndef ORDERHEAP_H
#define ORDERHEAP_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <order.h>

typedef struct OrderHeap
{
    Order **arr;
    int capacity;
    int size;
} OrderHeap;

OrderHeap *createOrderHeap(int capacity);
void swap(Order **a, Order **b);
void minHeapify(OrderHeap *heap, int idx);
void insertMinHeap(OrderHeap *heap, Order *key);
Order *extractMin(OrderHeap *heap);
Order *getMin(OrderHeap *heap);
void increaseHeapCapacity(OrderHeap *heap, int increment);

#endif