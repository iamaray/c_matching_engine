#include "orderheap.h"

OrderHeap *createMinHeap(int capacity)
{
    OrderHeap *heap = (OrderHeap *)malloc(sizeof(OrderHeap));
    if (!heap)
    {
        fprintf(stderr, "Memory error\n");
        exit(EXIT_FAILURE);
    }

    heap->capacity = capacity;
    heap->size = 0;

    heap->arr = (Order **)malloc(capacity * sizeof(Order *));
    if (!heap->arr)
    {
        fprintf(stderr, "Memory error\n");
        exit(EXIT_FAILURE);
    }

    return heap;
}

void swap(Order **a, Order **b)
{
    Order *temp = *a;
    *a = *b;
    *b = temp;
}

void minHeapify(OrderHeap *heap, int idx)
{
    // heapify based on price and timestamp
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    int smallest = idx;

    if (left < heap->size &&
        heap->arr[left]->price < heap->arr[smallest]->price &&
        heap->arr[left]->timestamp < heap->arr[smallest]->timestamp)
    {
        smallest = left;
    }

    if (right < heap->size &&
        heap->arr[right]->price < heap->arr[smallest]->price &&
        heap->arr[right]->timestamp < heap->arr[smallest]->timestamp)
    {
        smallest = right;
    }

    if (smallest != idx)
    {
        swap(&heap->arr[idx], &heap->arr[smallest]);
        minHeapify(heap, smallest);
    }
}

void insertMinHeap(OrderHeap *heap, Order *key)
{
    if (heap->size == heap->capacity)
    {
        printf("Heap is full; cannot insert key %d\n", key->order_id);
        return;
    }

    int i = heap->size;
    heap->arr[i] = key;
    heap->size++;

    while (i != 0 && heap->arr[(i - 1) / 2]->price > heap->arr[i]->price)
    {
        swap(&heap->arr[i], &heap->arr[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

int extractMin(OrderHeap *heap)
{
    if (heap->size <= 0)
        return NULL;

    if (heap->size == 1)
    {
        heap->size--;
        return heap->arr[0];
    }

    Order *root = heap->arr[0];
    heap->arr[0] = heap->arr[heap->size - 1];
    heap->size--;

    minHeapify(heap, 0);

    return root;
}

Order *getMin(OrderHeap *heap)
{
    if (heap->size <= 0)
        return NULL;

    return heap->arr[0];
}

void increaseHeapCapacity(OrderHeap *heap, int increment)
{
    if (increment <= 0)
    {
        return;
    }

    int newCapacity = heap->capacity + increment;

    Order **newArr = (Order **)realloc(heap->arr, newCapacity * sizeof(Order *));
    if (newArr == NULL)
    {
        fprintf(stderr, "Error: memory realloc failed while increasing capacity.\n");
        exit(EXIT_FAILURE);
    }

    heap->arr = newArr;
    heap->capacity = newCapacity;
}