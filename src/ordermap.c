#include "ordermap.h"

// Hash function to convert order_id to bucket index
unsigned int hash_function(int key, int capacity)
{
    // Simple hash function for integers
    return (unsigned int)key % capacity;
}

// Create a new OrderMap
OrderMap *create_ordermap()
{
    OrderMap *map = (OrderMap *)malloc(sizeof(OrderMap));
    if (!map)
    {
        fprintf(stderr, "Memory allocation failed for OrderMap\n");
        exit(EXIT_FAILURE);
    }

    map->capacity = INITIAL_CAPACITY;
    map->size = 0;

    map->buckets = (MapEntry **)calloc(map->capacity, sizeof(MapEntry *));
    if (!map->buckets)
    {
        fprintf(stderr, "Memory allocation failed for OrderMap buckets\n");
        free(map);
        exit(EXIT_FAILURE);
    }

    return map;
}

// Free the OrderMap and all its entries
void free_ordermap(OrderMap *map)
{
    if (!map)
        return;

    // Free all entries and their chains
    for (int i = 0; i < map->capacity; i++)
    {
        MapEntry *entry = map->buckets[i];
        while (entry)
        {
            MapEntry *next = entry->next;
            // Note: We don't free the Order* as it's managed elsewhere
            free(entry);
            entry = next;
        }
    }

    free(map->buckets);
    free(map);
}

// Resize the map when it gets too full
void ordermap_resize(OrderMap *map, int new_capacity)
{
    MapEntry **old_buckets = map->buckets;
    int old_capacity = map->capacity;

    // Allocate new buckets
    map->buckets = (MapEntry **)calloc(new_capacity, sizeof(MapEntry *));
    if (!map->buckets)
    {
        fprintf(stderr, "Memory allocation failed during resize\n");
        exit(EXIT_FAILURE);
    }

    map->capacity = new_capacity;
    map->size = 0;

    // Rehash all existing entries
    for (int i = 0; i < old_capacity; i++)
    {
        MapEntry *entry = old_buckets[i];
        while (entry)
        {
            MapEntry *next = entry->next;

            // Rehash and insert into new buckets
            unsigned int index = hash_function(entry->key, new_capacity);
            entry->next = map->buckets[index];
            map->buckets[index] = entry;
            map->size++;

            entry = next;
        }
    }

    free(old_buckets);
}

// Add or update an order in the map
void ordermap_put(OrderMap *map, int order_id, Order *order)
{
    // Check if resize is needed
    if ((float)map->size / map->capacity >= LOAD_FACTOR_THRESHOLD)
    {
        ordermap_resize(map, map->capacity * 2);
    }

    unsigned int index = hash_function(order_id, map->capacity);

    // Check if key already exists
    MapEntry *current = map->buckets[index];
    while (current)
    {
        if (current->key == order_id)
        {
            // Update existing entry
            current->value = order;
            return;
        }
        current = current->next;
    }

    // Create new entry
    MapEntry *new_entry = (MapEntry *)malloc(sizeof(MapEntry));
    if (!new_entry)
    {
        fprintf(stderr, "Memory allocation failed for new map entry\n");
        exit(EXIT_FAILURE);
    }

    new_entry->key = order_id;
    new_entry->value = order;
    new_entry->next = map->buckets[index];
    map->buckets[index] = new_entry;
    map->size++;
}

// Get an order by its ID
Order *ordermap_get(OrderMap *map, int order_id)
{
    if (!map)
        return NULL;

    unsigned int index = hash_function(order_id, map->capacity);
    MapEntry *entry = map->buckets[index];

    while (entry)
    {
        if (entry->key == order_id)
        {
            return entry->value;
        }
        entry = entry->next;
    }

    return NULL; // Order not found
}

// Remove an order from the map and return it
Order *ordermap_remove(OrderMap *map, int order_id)
{
    if (!map)
        return NULL;

    unsigned int index = hash_function(order_id, map->capacity);
    MapEntry *entry = map->buckets[index];
    MapEntry *prev = NULL;

    while (entry)
    {
        if (entry->key == order_id)
        {
            // Found the entry to remove
            if (prev)
            {
                prev->next = entry->next;
            }
            else
            {
                map->buckets[index] = entry->next;
            }

            Order *order = entry->value;
            free(entry);
            map->size--;
            return order;
        }

        prev = entry;
        entry = entry->next;
    }

    return NULL; // Order not found
}

// Check if an order ID exists in the map
int ordermap_contains(OrderMap *map, int order_id)
{
    if (!map)
        return 0;

    unsigned int index = hash_function(order_id, map->capacity);
    MapEntry *entry = map->buckets[index];

    while (entry)
    {
        if (entry->key == order_id)
        {
            return 1; // Found
        }
        entry = entry->next;
    }

    return 0; // Not found
}
