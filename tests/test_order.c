#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "order.h"

// Test order creation and basic properties
void test_order_creation()
{
    printf("Testing order creation...\n");

    Order *buy_order = create_order(1, 100, 10, 1000, 'B');
    assert(buy_order != NULL);
    assert(buy_order->order_id == 1);
    assert(buy_order->price == 100);
    assert(buy_order->quantity == 10);
    assert(buy_order->timestamp == 1000);
    assert(buy_order->side == 'B');

    Order *sell_order = create_order(2, 105, 5, 1001, 'S');
    assert(sell_order != NULL);
    assert(sell_order->order_id == 2);
    assert(sell_order->price == 105);
    assert(sell_order->quantity == 5);
    assert(sell_order->timestamp == 1001);
    assert(sell_order->side == 'S');

    printf("Order creation test passed!\n");

    free_order(buy_order);
    free_order(sell_order);
}

// Test buy order comparison
void test_buy_order_comparison()
{
    printf("Testing buy order comparison...\n");

    // Higher price should come first for buy orders
    Order *high_price = create_order(1, 110, 10, 1000, 'B');
    Order *low_price = create_order(2, 100, 10, 1000, 'B');
    assert(compare_buy_orders(high_price, low_price) < 0);
    assert(compare_buy_orders(low_price, high_price) > 0);

    // Same price, earlier timestamp should come first
    Order *early_time = create_order(3, 100, 10, 900, 'B');
    Order *late_time = create_order(4, 100, 10, 1000, 'B');
    assert(compare_buy_orders(early_time, late_time) < 0);
    assert(compare_buy_orders(late_time, early_time) > 0);

    // Same price and timestamp should be equal
    Order *same_order1 = create_order(5, 100, 10, 1000, 'B');
    Order *same_order2 = create_order(6, 100, 10, 1000, 'B');
    assert(compare_buy_orders(same_order1, same_order2) == 0);

    printf("Buy order comparison test passed!\n");

    free_order(high_price);
    free_order(low_price);
    free_order(early_time);
    free_order(late_time);
    free_order(same_order1);
    free_order(same_order2);
}

// Test sell order comparison
void test_sell_order_comparison()
{
    printf("Testing sell order comparison...\n");

    // Lower price should come first for sell orders
    Order *high_price = create_order(1, 110, 10, 1000, 'S');
    Order *low_price = create_order(2, 100, 10, 1000, 'S');
    assert(compare_sell_orders(low_price, high_price) < 0);
    assert(compare_sell_orders(high_price, low_price) > 0);

    // Same price, earlier timestamp should come first
    Order *early_time = create_order(3, 100, 10, 900, 'S');
    Order *late_time = create_order(4, 100, 10, 1000, 'S');
    assert(compare_sell_orders(early_time, late_time) < 0);
    assert(compare_sell_orders(late_time, early_time) > 0);

    // Same price and timestamp should be equal
    Order *same_order1 = create_order(5, 100, 10, 1000, 'S');
    Order *same_order2 = create_order(6, 100, 10, 1000, 'S');
    assert(compare_sell_orders(same_order1, same_order2) == 0);

    printf("Sell order comparison test passed!\n");

    free_order(high_price);
    free_order(low_price);
    free_order(early_time);
    free_order(late_time);
    free_order(same_order1);
    free_order(same_order2);
}

// Test order memory management
void test_order_memory()
{
    printf("Testing order memory management...\n");

    // Test freeing NULL order
    free_order(NULL);

    // Test creating and freeing multiple orders
    for (int i = 0; i < 1000; i++)
    {
        Order *order = create_order(i, i * 10, i, i * 1000, (i % 2 == 0) ? 'B' : 'S');
        free_order(order);
    }

    printf("Order memory management test passed!\n");
}

int main()
{
    printf("=== RUNNING ORDER TESTS ===\n\n");

    test_order_creation();
    test_buy_order_comparison();
    test_sell_order_comparison();
    test_order_memory();

    printf("\n=== ALL ORDER TESTS PASSED ===\n");
    return 0;
}
