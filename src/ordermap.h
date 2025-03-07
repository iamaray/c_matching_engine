#ifndef ORDERMAP_H
#define ORDERMAP_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "order.h"

#define INITIAL_CAPACITY 16
#define LOAD_FACTOR_THRESHOLD 0.75

typedef struct MapEntry
{
    int key;               // order_id
    Order *value;          // pointer to the order
    struct MapEntry *next; // for handling collisions
} MapEntry;

typedef struct OrderMap
{
    MapEntry **buckets; // array of entry pointers
    int capacity;       // total number of buckets
    int size;           // current number of entries
} OrderMap;

// Function declarations
OrderMap *create_ordermap();
void free_ordermap(OrderMap *map);
void ordermap_put(OrderMap *map, int order_id, Order *order);
Order *ordermap_get(OrderMap *map, int order_id);
Order *ordermap_remove(OrderMap *map, int order_id);
int ordermap_contains(OrderMap *map, int order_id);
void ordermap_resize(OrderMap *map, int new_capacity);
unsigned int hash_function(int key, int capacity);

#endif