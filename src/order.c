#include "order.h"

Order *create_order(int order_id, int price, int quantity, int timestamp, char side)
{
    Order *order = (Order *)malloc(sizeof(Order));
    order->order_id = order_id;
    order->price = price;
    order->quantity = quantity;
    order->timestamp = timestamp;
    order->side = side;

    return order;
}

void print_order(Order *order)
{
    printf("Order ID: %d || Price: %d || Quantity: %d || Timestamp: %d || Side: %c\n", order->order_id, order->price, order->quantity, order->timestamp, order->side);
}

void free_order(Order *order)
{
    if (order == NULL)
        return;
    free(order);
    order = NULL;
    return;
}

int compare_buy_orders(Order *order1, Order *order2)
{
    if (order1->price > order2->price)
        return -1;
    else if (order1->price == order2->price && order1->timestamp < order2->timestamp)
        return -1;
    else if (order1->price == order2->price && order1->timestamp == order2->timestamp)
        return 0;
    else
        return 1;
}

int compare_sell_orders(Order *order1, Order *order2)
{
    if (order1->price < order2->price)
        return -1;
    else if (order1->price == order2->price && order1->timestamp < order2->timestamp)
        return -1;
    else if (order1->price == order2->price && order1->timestamp == order2->timestamp)
        return 0;
    else
        return 1;
}