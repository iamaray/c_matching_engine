#include "orderheap.h"

OrderHeap *createOrderHeap(int capacity, HeapType type)
{
    OrderHeap *heap = (OrderHeap *)malloc(sizeof(OrderHeap));
    if (!heap)
    {
        fprintf(stderr, "Memory error\n");
        exit(EXIT_FAILURE);
    }

    heap->capacity = capacity;
    heap->size = 0;
    heap->type = type;

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

void heapify(OrderHeap *heap, int idx)
{
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    int smallest = idx;

    if (heap->type == BUY_HEAP)
    {
        if (left < heap->size &&
            compare_buy_orders(heap->arr[left], heap->arr[smallest]) < 0)
        {
            smallest = left;
        }

        if (right < heap->size &&
            compare_buy_orders(heap->arr[right], heap->arr[smallest]) < 0)
        {
            smallest = right;
        }
    }
    else
    {
        if (left < heap->size &&
            compare_sell_orders(heap->arr[left], heap->arr[smallest]) < 0)
        {
            smallest = left;
        }

        if (right < heap->size &&
            compare_sell_orders(heap->arr[right], heap->arr[smallest]) < 0)
        {
            smallest = right;
        }
    }

    if (smallest != idx)
    {
        swap(&heap->arr[idx], &heap->arr[smallest]);
        heapify(heap, smallest);
    }
}

void insertOrderHeap(OrderHeap *heap, Order *key)
{
    if (heap->size == heap->capacity)
    {
        printf("Heap is full; cannot insert key %d\n", key->order_id);
        return;
    }

    int i = heap->size;
    heap->arr[i] = key;
    heap->size++;

    if (heap->type == BUY_HEAP)
    {
        while (i != 0 && compare_buy_orders(heap->arr[(i - 1) / 2], heap->arr[i]) > 0)
        {
            swap(&heap->arr[i], &heap->arr[(i - 1) / 2]);
            i = (i - 1) / 2;
        }
    }
    else
    {
        while (i != 0 && compare_sell_orders(heap->arr[(i - 1) / 2], heap->arr[i]) > 0)
        {
            swap(&heap->arr[i], &heap->arr[(i - 1) / 2]);
            i = (i - 1) / 2;
        }
    }
}

Order *extractTop(OrderHeap *heap)
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

    heapify(heap, 0);

    return root;
}

Order *getTop(OrderHeap *heap)
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