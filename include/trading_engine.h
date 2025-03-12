#ifndef TRADING_ENGINE_H
#define TRADING_ENGINE_H

/* Primary public entrypoint for the trading engine API. */

#include <stdint.h>

typedef struct Order Order;
typedef struct OrderBook OrderBook;

/* order CRUD */
// returns copy of order
Order *trading_create_order(
    int order_id,
    double price,
    int quantity,
    double timestamp,
    char side);
// returns copy of order
Order *trading_read_order(OrderBook *book, int order_id);
// returns 0 if successful, -1 otherwise
int *trading_cancel_order(OrderBook *book, int order_id);
// returns 0 if successful, -1 otherwise
int *trading_modify_order(
    OrderBook *book,
    int order_id,
    double new_price,
    int new_quantity);

/* orderbook CRUD */
OrderBook *trading_create_ordebook(void);
OrderBook *trading_read_orderbook(OrderBook *book);
OrderBook *trading_add_order(OrderBook *book, Order *order);
OrderBook *trading_free_orderbook(OrderBook *book);

/* retrieve market data */
double trading_get_best_bid(OrderBook *book);
double trading_get_best_ask(OrderBook *book);
double trading_get_last_price(OrderBook *book);

#endif