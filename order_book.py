from collections import deque
import heapq
import time
from math import floor, ceil
import matplotlib.pyplot as plt
import numpy as np

from order_types import Order, LimitOrder, MarketOrder
from matching_engine import MatchingEngine, PriceTimeEngine


class OrderBook:
    def __init__(self, matching_engine: MatchingEngine = PriceTimeEngine()):
        self.buy_orders = []  # max-heap
        self.sell_orders = []  # min-heap
        self.order_map = {}  # order_id -> Order
        self.matching_engine = matching_engine
        self.trade_history = []
        self.price_history = []  # holds {mid, VWAP, last traded}
        self.spread_history = []
        self.partially_filled = []

        self.stop_buy_orders = []
        self.stop_sell_orders = []

    def add_order(self, order_id, side, price, quantity):
        timestamp = time.time()
        order = LimitOrder(order_id=order_id, side=side, price=price,
                           quantity=quantity, timestamp=timestamp)

        if side == 'buy':
            heapq.heappush(self.buy_orders, order)
        elif side == 'sell':
            heapq.heappush(self.sell_orders, order)

        self.order_map[order_id] = order
        self.match_orders()  # Check for immediate matching

    def cancel_order(self, order_id):
        if order_id in self.order_map:
            order = self.order_map.pop(order_id)
            order.quantity = 0  # Mark as canceled
            print(f"Order {order_id} canceled.")

    def match_orders(self):
        executed_trades = self.matching_engine.match(
            buy_orders=self.buy_orders, sell_orders=self.sell_orders, order_map=self.order_map)

        self.trade_history.extend(executed_trades)

    def submit_market_order(self, order_id, side, quantity):
        timestamp = time.time()
        order = MarketOrder(order_id=order_id, side=side,
                            quantity=quantity, timestamp=timestamp)

        unfilled, history = None, []
        if side == 'buy':
            unfilled, history = self.matching_engine.match_market_buy(
                order=order, sell_orders=self.sell_orders, order_map=self.order_map)

        else:
            unfilled, history = self.matching_engine.match_market_sell(
                order=order, buy_orders=self.buy_orders, order_map=self.order_map)

        self.matching_engine.match(
            buy_orders=self.buy_orders, sell_orders=self.sell_orders, order_map=self.order_map)

        self.trade_history.extend(history)

        # As of now, I am simply discarding unfilled portions of limit orders.
        # TODO: figure out how to handle.

    def _check_stop_cond(self, side, trigger_price):
        current_price = self.price_history[-1]['midpoint']
        return (
            (side == 'sell' and current_price <= trigger_price) or
            (side == 'buy' and current_price >= trigger_price)
        )

    def submit_stop_order(self, order_id, trigger_price, side, quantity, limit=False, limit_price=None):
        should_trigger = self._check_stop_cond(side, trigger_price)
        if should_trigger:
            if limit:
                self.add_order(order_id=order_id, side=side,
                               price=price, quantity=quantity)
            else:
                self.submit_market_order(
                    order_id=order_id, side=side, quantity=quantity)

        else:
            self.stop_orders.append({
                'order_id': order_id,
                'trigger_price': trigger_price,
                'side': side,
                'quantity': quantity,
                'limit': limit
            })

    def handle_stop_orders(self):
        for order_map in self.stop_orders:
            if self._check_stop_cond(order_map['side'], order_map['trigger_price']):
                if order_map['limit']:
                    self.add_order(order_id=order_id, side=side,
                                   price=price, quantity=quantity)
                else:
                    self.submit_market_order(
                        order_id=order_id, side=side, quantity=quantity)

    def compute_market_price(self):
        prices = {}
        # Midpoint price
        if self.buy_orders and self.sell_orders:
            highest_bid = self.buy_orders[0].price
            lowest_ask = self.sell_orders[0].price
            midpoint_price = (highest_bid + lowest_ask) / 2
            prices['midpoint'] = midpoint_price

            # Record bid-ask spread
            spread = lowest_ask - highest_bid
            self.spread_history.append(spread)
        else:
            prices['midpoint'] = None
            self.spread_history.append(None)

        # Last trade price
        last_trade_price = self.trade_history[-1]['price'] if self.trade_history else None
        prices['last_trade'] = last_trade_price

        # VWAP (top 5 orders)
        combined_orders = self.buy_orders[:5] + self.sell_orders[:5]
        total_value = sum(
            order.price * order.quantity for order in combined_orders)
        total_quantity = sum(order.quantity for order in combined_orders)
        vwap = total_value / total_quantity if total_quantity > 0 else None
        prices['vwap'] = vwap

        self.price_history.append(prices)

    def compute_market_depth(self, band_length=1.0):
        def get_band(price):
            return floor(price / band_length) * band_length, ceil(price / band_length) * band_length

        buy_depth = {}
        sell_depth = {}

        for order in self.buy_orders:
            band = get_band(order.price)

            if band not in buy_depth:
                buy_depth[band] = 0

            buy_depth[band] += order.quantity

        for order in self.sell_orders:
            band = get_band(order.price)

            if band not in sell_depth:
                sell_depth[band] = 0

            sell_depth[band] += order.quantity

        return buy_depth, sell_depth

    def display(self):
        print("Buy Orders:")
        for order in sorted(self.buy_orders, key=lambda x: (-x.price, x.timestamp)):
            if order.quantity > 0:
                print(f"ID: {order.order_id}, Price: {order.price}, Quantity: {
                      order.quantity}, Time: {order.timestamp}")
        print("Sell Orders:")
        for order in sorted(self.sell_orders, key=lambda x: (x.price, x.timestamp)):
            if order.quantity > 0:
                print(f"ID: {order.order_id}, Price: {order.price}, Quantity: {
                      order.quantity}, Time: {order.timestamp}")


class OrderBookUtils:
    @staticmethod
    def plot_depth_chart(order_book: OrderBook, band_length=1.0):
        midpoint = order_book.price_history[-1]['midpoint']
        if midpoint is None:
            print("Cannot plot depth chart: No midpoint price available.")
            return

        buy_depth, sell_depth = order_book.compute_market_depth(band_length)

        buy_prices = [band[0] for band in sorted(buy_depth.keys())]
        buy_quantities = [buy_depth[band]
                          for band in sorted(buy_depth.keys())]

        sell_prices = [band[0] for band in sorted(sell_depth.keys())]
        sell_quantities = [sell_depth[band]
                           for band in sorted(sell_depth.keys())]

        plt.bar(buy_prices, buy_quantities, width=band_length,
                color='green', align='edge', label='Buy Depth')

        plt.bar(sell_prices, sell_quantities, width=-band_length,
                color='red', align='edge', label='Sell Depth')

        # Annotate midpoint price
        plt.axvline(midpoint, color='blue', linestyle='--',
                    label=f'Midpoint Price: {midpoint:.2f}')

        plt.xlabel('Price')
        plt.ylabel('Liquidity')
        plt.title('Market Depth Chart')
        plt.legend()
        plt.show()


def trade_slippage(trade):
    d = 1 if trade['side'] == 'buy' else -1

    return d * (trade['price'] - trade['last_price'])


def compute_slippage(trade_list):
    weighted_slippage = np.array(
        [trade['quantity'] * slippage(trade) for trade in trade_list])

    total_quantity = np.sum([trade['quantity'] for trade in trade_list])

    return np.sum(weighted_slippage) / total_quantity
