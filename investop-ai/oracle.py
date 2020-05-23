#!/usr/bin/python

# -*- coding: utf-8 -*-

from order import *


def compute_gain_or_lost(buy, sell):
    return (sell.stock_data.avg_price -
            buy.stock_data.avg_price) / buy.stock_data.avg_price


def futute(base2016, STOCKS):
    result = {stock: {'orders': [], 'weight': 1.0 /
                      len(STOCKS)} for stock in STOCKS}

    for ticker, data in base2016.items():
        orders = []
        valley = data[0].avg_price
        peak = data[0].avg_price

        for i in range(1, len(data)):
            if data[i].avg_price >= peak:
                peak = data[i].avg_price
            else:
                peak = data[i].avg_price
                orders.append(Order(data[i-1], SELL_ORDER_TYPE))

            if data[i].avg_price <= valley:
                valley = data[i].avg_price
            else:
                valley = data[i].avg_price
                if len(orders) == 0 or orders[-1].type == SELL_ORDER_TYPE:
                    orders.append(Order(data[i-1], BUY_ORDER_TYPE))

        result[ticker]['orders'] = orders

    for ticker in result.keys():
        orders = result[ticker]['orders']
        for i in range(1, len(orders), 2):
            buy_order = orders[i-1] if orders[i -
                                              1].type == BUY_ORDER_TYPE else orders[i]
            sell_order = orders[i] if orders[i].type == SELL_ORDER_TYPE else orders[i-1]
            result[ticker]['weight'] = result[ticker]['weight'] + result[ticker]['weight'] * \
                compute_gain_or_lost(buy_order, sell_order)

        if len(orders) % 2 != 0:
            last_order = orders[-1]
            if last_order.type == SELL_ORDER_TYPE:
                buy_order = orders[-2]
                result[ticker]['weight'] = result[ticker]['weight'] + result[ticker]['weight'] * \
                    compute_gain_or_lost(buy_order, last_order)

    sum = 0
    for ticker in result.keys():
        sum += result[ticker]['weight']

    result['total'] = sum

    return result
