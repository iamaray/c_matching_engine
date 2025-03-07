#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "order.h"
#include "orderheap.h"
#include "ordermap.h"
typedef struct OrderBook
{
    // buy orders
    OrderHeap *buy_orders;
    // sell orders
    OrderHeap *sell_orders;
    // order map
    OrderMap *order_map;
    // trade history
    Order **trade_history;
    int trade_history_size;
    int trade_history_capacity;
    // price history
    double *price_history;
    int price_history_size;
    int price_history_capacity;

} OrderBook;

OrderBook *create_orderbook();
void free_orderbook(OrderBook *orderbook);
void add_order(OrderBook *orderbook, Order *order);
void print_orderbook(OrderBook *orderbook);

#endif