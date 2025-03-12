#ifndef MATCHER_H
#define MATCHER_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "order.h"
#include "orderbook.h"
#include "orderheap.h"
#include "ordermap.h"

/*
    Matching engine logic
*/

typedef struct
{
    int maker_id;
    int taker_id;
    int traded_quantity;
    int maker_leftover;
    int taker_leftover;
    double traded_price;

} FilledOrder;

// int _fill_trade(
//     Order *maker,
//     Order *taker,
//     OrderHeap *buy_orders,
//     OrderHeap *sell_orders);

// -1: error filling and/or logging trade
// 0: successfully filled and logged trade
// 1: orderbook is non-crossing
int match_orderbook(OrderBook *book);

#endif