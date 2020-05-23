#!/usr/bin/python

BUY_ORDER_TYPE = 0
SELL_ORDER_TYPE = 1


class Order:
    def __init__(self, stock_data, type):
        self.stock_data = stock_data
        self.type = type
