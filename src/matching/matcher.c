#include "matcher.h"

int match_orderbook(OrderBook *book)
{
    // see if top buy and top sell can be matched
    Order *top_sell = extractTop(book->sell_orders);
    Order *top_buy = extractTop(book->buy_orders);
    if (top_buy->price >= top_sell->price)
    {
        // if so, fill the trade
        // top_sell = extractTop(book->sell_orders);
        // top_buy = extractTop(book->sell_orders);
        int traded_quantity;
        if (top_buy->quantity <= top_sell->quantity)
        {
            traded_quantity = top_buy->quantity;
        }
        else
        {
            traded_quantity = top_sell->quantity;
        }
        top_sell->quantity -= traded_quantity;
        top_buy->quantity -= traded_quantity;

        if (top_sell->quantity == 0)
        {
            top_sell = extractTop(book->sell_orders);
        }
        if (top_buy->quantity == 0)
        {
            top_buy = extractTop(book->buy_orders);
        }
    }
    else
    {
        return 1;
    }

    // record data
}