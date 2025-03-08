#include "orderbook.h"

#define INITIAL_HISTORY_CAPACITY 100

OrderBook *create_orderbook()
{
    OrderBook *orderbook = (OrderBook *)malloc(sizeof(OrderBook));
    if (!orderbook)
    {
        fprintf(stderr, "Memory allocation failed for OrderBook\n");
        exit(EXIT_FAILURE);
    }

    orderbook->buy_orders = createOrderHeap(50, BUY_HEAP);
    orderbook->sell_orders = createOrderHeap(50, SELL_HEAP);

    orderbook->order_map = create_ordermap();

    orderbook->trade_history_capacity = INITIAL_HISTORY_CAPACITY;
    orderbook->trade_history_size = 0;
    orderbook->trade_history = (Order **)malloc(INITIAL_HISTORY_CAPACITY * sizeof(Order *));
    if (!orderbook->trade_history)
    {
        fprintf(stderr, "Memory allocation failed for trade history\n");
        exit(EXIT_FAILURE);
    }

    orderbook->price_history_capacity = INITIAL_HISTORY_CAPACITY;
    orderbook->price_history_size = 0;
    orderbook->price_history = (double *)malloc(INITIAL_HISTORY_CAPACITY * sizeof(double));
    if (!orderbook->price_history)
    {
        fprintf(stderr, "Memory allocation failed for price history\n");
        exit(EXIT_FAILURE);
    }

    return orderbook;
}

void free_orderbook(OrderBook *orderbook)
{
    if (!orderbook)
        return;

    // Note: The orders themselves are managed by the order_map
    if (orderbook->buy_orders)
    {
        free(orderbook->buy_orders->arr);
        free(orderbook->buy_orders);
    }

    if (orderbook->sell_orders)
    {
        free(orderbook->sell_orders->arr);
        free(orderbook->sell_orders);
    }

    // Note: The orders themselves should be freed separately
    free_ordermap(orderbook->order_map);

    if (orderbook->trade_history)
        free(orderbook->trade_history);

    if (orderbook->price_history)
        free(orderbook->price_history);

    free(orderbook);
}

// Helper function to expand trade history capacity
static void expand_trade_history(OrderBook *orderbook)
{
    int new_capacity = orderbook->trade_history_capacity * 2;
    Order **new_history = (Order **)realloc(orderbook->trade_history,
                                            new_capacity * sizeof(Order *));
    if (!new_history)
    {
        fprintf(stderr, "Memory reallocation failed for trade history\n");
        exit(EXIT_FAILURE);
    }

    orderbook->trade_history = new_history;
    orderbook->trade_history_capacity = new_capacity;
}

// Helper function to expand price history capacity
static void expand_price_history(OrderBook *orderbook)
{
    int new_capacity = orderbook->price_history_capacity * 2;
    double *new_history = (double *)realloc(orderbook->price_history,
                                            new_capacity * sizeof(double));
    if (!new_history)
    {
        fprintf(stderr, "Memory reallocation failed for price history\n");
        exit(EXIT_FAILURE);
    }

    orderbook->price_history = new_history;
    orderbook->price_history_capacity = new_capacity;
}

// Helper function to record a trade
static void record_trade(OrderBook *orderbook, Order *order, double price)
{
    if (orderbook->trade_history_size >= orderbook->trade_history_capacity)
    {
        expand_trade_history(orderbook);
    }

    orderbook->trade_history[orderbook->trade_history_size++] = order;

    if (orderbook->price_history_size >= orderbook->price_history_capacity)
    {
        expand_price_history(orderbook);
    }

    orderbook->price_history[orderbook->price_history_size++] = price;
}

void add_order(OrderBook *orderbook, Order *order)
{
    if (!orderbook || !order)
        return;

    ordermap_put(orderbook->order_map, order->order_id, order);

    if (order->side == 'B')
    {
        insertOrderHeap(orderbook->buy_orders, order);
    }
    else if (order->side == 'S')
    {
        insertOrderHeap(orderbook->sell_orders, order);
    }
    else
    {
        fprintf(stderr, "Invalid order side: %c\n", order->side);
        return;
    }

    // Try to match orders and execute trades
    match_orders(orderbook);
}

void print_orderbook(OrderBook *orderbook)
{
    if (!orderbook)
        return;

    printf("\n===== ORDER BOOK =====\n");

    printf("\nBUY ORDERS:\n");
    printf("-------------\n");

    int buy_size = orderbook->buy_orders->size;
    Order **buy_orders_temp = (Order **)malloc(buy_size * sizeof(Order *));

    for (int i = 0; i < buy_size; i++)
    {
        buy_orders_temp[i] = extractTop(orderbook->buy_orders);
    }

    for (int i = 0; i < buy_size; i++)
    {
        print_order(buy_orders_temp[i]);
        insertOrderHeap(orderbook->buy_orders, buy_orders_temp[i]);
    }

    free(buy_orders_temp);

    printf("\nSELL ORDERS:\n");
    printf("-------------\n");

    int sell_size = orderbook->sell_orders->size;
    Order **sell_orders_temp = (Order **)malloc(sell_size * sizeof(Order *));

    for (int i = 0; i < sell_size; i++)
    {
        sell_orders_temp[i] = extractTop(orderbook->sell_orders);
    }

    for (int i = 0; i < sell_size; i++)
    {
        print_order(sell_orders_temp[i]);
        insertOrderHeap(orderbook->sell_orders, sell_orders_temp[i]);
    }

    free(sell_orders_temp);

    printf("\nRECENT TRADES:\n");
    printf("-------------\n");

    int start_idx = (orderbook->trade_history_size > 5) ? orderbook->trade_history_size - 5 : 0;

    for (int i = start_idx; i < orderbook->trade_history_size; i++)
    {
        printf("Trade at price: %.2f - ", orderbook->price_history[i]);
        print_order(orderbook->trade_history[i]);
    }

    printf("\n======================\n");
}
