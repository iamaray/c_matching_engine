#ifndef ORDERHEAP_H
#define ORDERHEAP_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "order.h"

typedef enum
{
    BUY_HEAP,
    SELL_HEAP
} HeapType;

typedef struct OrderHeap
{
    Order **arr;
    int capacity;
    int size;
    HeapType type;
} OrderHeap;

OrderHeap *createOrderHeap(int capacity, HeapType type);
void swap(Order **a, Order **b);
void heapify(OrderHeap *heap, int idx);
void insertOrderHeap(OrderHeap *heap, Order *key);
Order *extractTop(OrderHeap *heap);
Order *getTop(OrderHeap *heap);
void increaseHeapCapacity(OrderHeap *heap, int increment);

#endif