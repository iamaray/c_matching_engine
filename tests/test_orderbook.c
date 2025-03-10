#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "orderbook.h"

// Test orderbook creation and initialization
void test_orderbook_creation()
{
    printf("Testing orderbook creation...\n");

    OrderBook *orderbook = create_orderbook();
    assert(orderbook != NULL);
    assert(orderbook->buy_orders != NULL);
    assert(orderbook->sell_orders != NULL);
    assert(orderbook->order_map != NULL);
    assert(orderbook->trade_history != NULL);
    assert(orderbook->trade_history_size == 0);
    assert(orderbook->trade_history_capacity > 0);
    assert(orderbook->price_history != NULL);
    assert(orderbook->price_history_size == 0);
    assert(orderbook->price_history_capacity > 0);

    printf("Orderbook creation test passed!\n");

    free_orderbook(orderbook);
}

// Test adding orders to the orderbook
void test_add_orders()
{
    printf("Testing adding orders to orderbook...\n");

    OrderBook *orderbook = create_orderbook();

    // Add buy orders
    Order *buy1 = create_order(1, 100, 10, 1000, 'B');
    Order *buy2 = create_order(2, 105, 5, 1001, 'B');
    Order *buy3 = create_order(3, 95, 15, 1002, 'B');

    add_order(orderbook, buy1);
    add_order(orderbook, buy2);
    add_order(orderbook, buy3);

    // Add sell orders
    Order *sell1 = create_order(4, 110, 8, 1003, 'S');
    Order *sell2 = create_order(5, 108, 12, 1004, 'S');
    Order *sell3 = create_order(6, 115, 7, 1005, 'S');

    add_order(orderbook, sell1);
    add_order(orderbook, sell2);
    add_order(orderbook, sell3);

    // Verify orders are in the map
    assert(ordermap_contains(orderbook->order_map, 1));
    assert(ordermap_contains(orderbook->order_map, 2));
    assert(ordermap_contains(orderbook->order_map, 3));
    assert(ordermap_contains(orderbook->order_map, 4));
    assert(ordermap_contains(orderbook->order_map, 5));
    assert(ordermap_contains(orderbook->order_map, 6));

    // Verify heap sizes
    assert(orderbook->buy_orders->size == 3);
    assert(orderbook->sell_orders->size == 3);

    // Verify price-time ordering for buy orders (highest price first, then earliest timestamp)
    Order *top_buy = extractTop(orderbook->buy_orders);
    assert(top_buy->order_id == 2); // Highest price (105)
    
    Order *next_buy = extractTop(orderbook->buy_orders);
    assert(next_buy->order_id == 1); // Second highest price (100)
    
    Order *last_buy = extractTop(orderbook->buy_orders);
    assert(last_buy->order_id == 3); // Lowest price (95)
    
    // Reinsert the orders to restore the heap
    insertOrderHeap(orderbook->buy_orders, top_buy);
    insertOrderHeap(orderbook->buy_orders, next_buy);
    insertOrderHeap(orderbook->buy_orders, last_buy);
    
    // Verify price-time ordering for sell orders (lowest price first, then earliest timestamp)
    Order *top_sell = extractTop(orderbook->sell_orders);
    assert(top_sell->order_id == 5); // Lowest price (108)
    
    Order *next_sell = extractTop(orderbook->sell_orders);
    assert(next_sell->order_id == 4); // Second lowest price (110)
    
    Order *last_sell = extractTop(orderbook->sell_orders);
    assert(last_sell->order_id == 6); // Highest price (115)
    
    // Reinsert the orders to restore the heap
    insertOrderHeap(orderbook->sell_orders, top_sell);
    insertOrderHeap(orderbook->sell_orders, next_sell);
    insertOrderHeap(orderbook->sell_orders, last_sell);

    printf("Adding orders test passed!\n");

    // Print the orderbook for visual inspection
    printf("\nOrderbook state after adding orders:\n");
    print_orderbook(orderbook);

    // Clean up
    free_orderbook(orderbook);
    // Note: Orders are freed by free_orderbook
}

// Test matching orders in the orderbook
void test_order_matching()
{
    printf("Testing order matching in orderbook...\n");

    OrderBook *orderbook = create_orderbook();

    // Add non-matching orders first
    Order *buy1 = create_order(1, 100, 10, 1000, 'B');
    Order *sell1 = create_order(2, 110, 10, 1001, 'S');

    add_order(orderbook, buy1);
    add_order(orderbook, sell1);

    // Verify no trades occurred
    assert(orderbook->trade_history_size == 0);
    assert(orderbook->buy_orders->size == 1);
    assert(orderbook->sell_orders->size == 1);

    // Add matching order (buy at higher price than existing sell)
    Order *buy2 = create_order(3, 115, 5, 1002, 'B');
    add_order(orderbook, buy2);

    // Verify trade occurred
    assert(orderbook->trade_history_size == 1);
    assert(orderbook->price_history_size == 1);

    // Verify quantities were updated
    Order *remaining_sell = ordermap_get(orderbook->order_map, 2);
    assert(remaining_sell->quantity == 5);

    // Add another matching order
    Order *sell2 = create_order(4, 95, 8, 1003, 'S');
    add_order(orderbook, sell2);

    // Verify another trade occurred
    assert(orderbook->trade_history_size == 2);
    assert(orderbook->price_history_size == 2);

    printf("Order matching test passed!\n");

    // Print the orderbook for visual inspection
    printf("\nOrderbook state after matching orders:\n");
    print_orderbook(orderbook);

    // Clean up
    free_orderbook(orderbook);
}

// Test edge cases
void test_edge_cases()
{
    printf("Testing orderbook edge cases...\n");

    OrderBook *orderbook = create_orderbook();

    // Test NULL order
    add_order(orderbook, NULL);
    assert(orderbook->buy_orders->size == 0);
    assert(orderbook->sell_orders->size == 0);

    // Test invalid side
    Order *invalid_order = create_order(1, 100, 10, 1000, 'X');
    add_order(orderbook, invalid_order);
    assert(orderbook->buy_orders->size == 0);
    assert(orderbook->sell_orders->size == 0);

    // Test zero quantity
    Order *zero_qty = create_order(2, 100, 0, 1001, 'B');
    add_order(orderbook, zero_qty);

    // Test negative price
    Order *neg_price = create_order(3, -10, 10, 1002, 'S');
    add_order(orderbook, neg_price);

    printf("Edge cases test passed!\n");

    // Clean up
    free_orderbook(orderbook);
    free_order(invalid_order);
    free_order(zero_qty);
    free_order(neg_price);
}

int main()
{
    printf("=== RUNNING ORDERBOOK TESTS ===\n\n");

    test_orderbook_creation();
    test_add_orders();
    test_order_matching();
    test_edge_cases();

    printf("\n=== ALL ORDERBOOK TESTS PASSED ===\n");
    return 0;
}
