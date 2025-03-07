#ifndef ORDER_H
#define ORDER_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct Order
{
    int order_id;
    double price;
    int quantity;
    double timestamp;
    char side; // 'B' for "buy", 'S' for "sell"
} Order;

Order *create_order(int order_id, int price, int quantity, int timestamp, char side);

#endif
