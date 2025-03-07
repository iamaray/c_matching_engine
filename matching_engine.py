from collections import deque
import heapq


class MatchingEngine:
    def __init__(self):
        pass

    def match(buy_orders, sell_orders, order_map):
        raise NotImplementedError


class PriceTimeEngine(MatchingEngine):
    def __init__(self):
        super().__init__()

    def _execute_trade(self, maker, taker, buy_orders, sell_orders, order_map):
        traded_quantity = min(maker.quantity, taker.quantity)

        trade_price = maker.price
        if taker.order_type != 'market':
            trade_price = taker.price

        maker.quantity -= traded_quantity
        taker.quantity -= traded_quantity

        # Remove fully filled taker orders if the relevant order book exists
        if taker.quantity == 0:
            if taker.side == 'buy' and buy_orders:
                heapq.heappop(buy_orders)
            elif taker.side == 'sell' and sell_orders:
                heapq.heappop(sell_orders)
            order_map.pop(taker.order_id, None)

        # Remove fully filled maker orders if the relevant order book exists
        if maker.quantity == 0:
            if maker.side == 'buy' and buy_orders:
                heapq.heappop(buy_orders)
            elif maker.side == 'sell' and sell_orders:
                heapq.heappop(sell_orders)
            order_map.pop(maker.order_id, None)

        return {
            'taker_order': taker.order_id,
            'taker_order_type': taker.order_type,
            'maker_order': maker.order_id,
            'maker_order_type': maker.order_type,
            'quantity': traded_quantity,
            'price': trade_price
        }

    def match(self, buy_orders, sell_orders, order_map):
        executed_trades = []
        while buy_orders and sell_orders:
            top_buy = buy_orders[0]
            top_sell = sell_orders[0]

            print(f"Attempting to match: Buy Order {top_buy.order_id} @ {top_buy.price}, Quantity: {
                  top_buy.quantity} | Sell Order {top_sell.order_id} @ {top_sell.price}, Quantity: {top_sell.quantity}")

            if top_buy.price >= top_sell.price:
                executed = self._execute_trade(
                    top_buy, top_sell, buy_orders, sell_orders, order_map)
                executed_trades.append(executed)
                print(f"Matched buy order {executed['maker_order']} with sell order {
                      executed['taker_order']} | ({executed['quantity']}) @ ${executed['price']}")
            else:
                print("No match found. Exiting matching loop.")
                break
        return executed_trades

    def match_market_buy(self, order, sell_orders, order_map):
        assert (order.side == 'buy' and order.order_type == 'market')

        leftover = order.copy()
        execution_history = []
        while leftover.quantity > 0 and sell_orders:
            top_sell = sell_orders[0]
            executed = self._execute_trade(
                maker=top_sell, taker=leftover, buy_orders=None, sell_orders=sell_orders, order_map=order_map)
            execution_history.append(executed)

        return leftover, execution_history

    def match_market_sell(self, order, buy_orders, order_map):
        assert (order.side == 'sell' and order.order_type == 'market')

        leftover = order.copy()
        execution_history = []
        while leftover.quantity > 0 and buy_orders:
            top_buy = buy_orders[0]
            executed = self._execute_trade(
                maker=top_buy, taker=leftover, buy_orders=buy_orders, sell_orders=None, order_map=order_map)
            execution_history.append(executed)

        return leftover, execution_history

    # def match_market_order(self):
    #     assert order.order_type == 'market'
