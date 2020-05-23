#!/usr/bin/python

import helpers
import order
from stocks import STOCKS


PERIOD = 10


def sma(prices, p):
    """Computes Simple Moving Average (SMA)

    Keyword arguments:
    closing_prices -- list of closing prices either
    n -- period of time
    """
    result = 0
    N = len(prices)
    i = N - p

    while i < N:
        price = prices[i]
        result = price + result
        i += 1

    result = result / p

    return result


def choice(max_average, min_average, closing_price):
    """choice(max, min) -> int
    Makes a buying or selling choice given a historic of comparison of closing princes with Max or Min price averages

    Keyword argumens:
    max -- historic of max_price SMA
    min -- historic of min_price SMA

    Return:
     1: buy
    -1: sell
     0: keep
    """
    n = len(max_average) - 1
    if n == 0:  # if n == 0, then is the first iteration and we must wait for entry signal
        return 0

    #have to implement the trade signal

    #if closing price is higher than max price average, buy choice
    if closing_price > max_average[-1]:
        return 1

    #if closing price is lower than min price average, sell choice
    if closing_price < min_average[-1]:
        return -1

def compute_gain_or_lost(buy, sell):
    return (sell.stock_data.avg_price - buy.stock_data.avg_price) / buy.stock_data.avg_price

def simulate(base2014, base2015, base2016):
    full_base = helpers.merge(base2014, base2015)

    result = {stock: {'orders': [], 'weight': 1.0/len(STOCKS)} for stock in STOCKS}

    # Start simulation
    for ticker, data2016 in base2016.items():
        stock_data = full_base[ticker]  # Data from 2014/2015 for ticker

        max_average = []
        min_average = []

        for data in data2016:

            # defining lists
            closing_prices = map(lambda x: x.closing_price, stock_data)
            max_price = map(lambda x: x.max_price, stock_data)
            min_price = map(lambda x: x.min_price, stock_data)

            # Compute max SMA
            max_average.append(sma(max_price, PERIOD))
            # Compute min SMA
            min_average.append(sma(min_price, PERIOD))

            # Compute choice based on short and long MME
            c = choice(max_average, min_average, closing_prices[-1])

            stock_data.append(data)


            if c == 1:  # if choice is 1, then send a buying order.
                result[ticker]['orders'].append(
                    order.Order(data, order.BUY_ORDER_TYPE))
            elif c == -1:  # if choice is -1, send a selling order
                result[ticker]['orders'].append(
                    order.Order(data, order.SELL_ORDER_TYPE))


    for ticker in result.keys():
        orders = result[ticker]['orders']
        for i in range(1, len(orders), 2):
            buy_order = orders[i-1]
            sell_order = orders[i]
            result[ticker]['weight'] = result[ticker]['weight'] + result[ticker]['weight'] * \
                compute_gain_or_lost(buy_order, sell_order)

        if len(orders) % 2 != 0:
            last_order = orders[-1]
            if last_order.type == order.SELL_ORDER_TYPE:
                buy_order = orders[-2]
                result[ticker]['weight'] = result[ticker]['weight'] + result[ticker]['weight'] * \
                    compute_gain_or_lost(buy_order, last_order)

    sum = 0
    for ticker in result.keys():
        sum += result[ticker]['weight']

    result['total'] = sum

    return result
