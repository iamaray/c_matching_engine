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

// int _fill_trade(
//     Order *maker,
//     Order *taker,
//     OrderHeap *buy_orders,
//     OrderHeap *sell_orders);

int match_orderbook(OrderBook *book);


#endif