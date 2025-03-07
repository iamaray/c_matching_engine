class Order:
    def __init__(self, order_id, side, quantity, timestamp, order_type):
        self.order_id = order_id
        self.side = side
        self.quantity = quantity
        self.timestamp = timestamp
        self.order_type = order_type

    def copy(self):
        return Order(
            order_id=self.order_id,
            side=self.side,
            quantity=self.quantity,
            timestamp=self.timestamp,
            order_type=self.order_type
        )


class LimitOrder(Order):
    def __init__(self, order_id, side, price, quantity, timestamp):
        super().__init__(order_id, side, quantity, timestamp, order_type='limit')
        self.price = price

    def __lt__(self, other):
        if self.side == 'buy':
            if self.price == other.price:
                return self.timestamp < other.timestamp
            return self.price > other.price
        elif self.side == 'sell':
            if self.price == other.price:
                return self.timestamp < other.timestamp
            return self.price < other.price


class MarketOrder(Order):
    def __init__(self, order_id, side, quantity, timestamp):
        super().__init__(order_id, side, quantity, timestamp, order_type='market')
