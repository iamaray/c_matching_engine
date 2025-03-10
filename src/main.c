#include <stdio.h>
#include "core/orderbook.h"

int main()
{
    printf("Order Book Matching Engine\n");

    // Create an orderbook
    OrderBook *orderbook = create_orderbook();

    // Add some sample orders
    Order *buy1 = create_order(1, 100, 10, 1000, 'B');
    Order *sell1 = create_order(2, 105, 5, 1001, 'S');

    add_order(orderbook, buy1);
    add_order(orderbook, sell1);

    // Print the orderbook state
    print_orderbook(orderbook);

    // Clean up
    free_orderbook(orderbook);

    return 0;
}
