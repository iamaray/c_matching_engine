#include <stdio.h>
#include "orderbook.h"

int main()
{
    printf("Order Book Matching Engine\n");

    OrderBook *orderbook = create_orderbook();

    Order *buy1 = create_order(1, 100, 10, 1000, 'B');
    Order *sell1 = create_order(2, 105, 5, 1001, 'S');

    add_order(orderbook, buy1);
    add_order(orderbook, sell1);

    print_orderbook(orderbook);

    free_orderbook(orderbook);

    return 0;
}
