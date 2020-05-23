#!/usr/bin/python

import helpers
import order
from stocks import STOCKS

EMA_SHORT = 3
EMA_LONG = 21


def ema(closing_prices, n):
    """Computes Exponential Moving Average (EMA)

    Keyword arguments:
    closing_prices -- list of closing prices
    n -- period of time
    """
    result = 0
    M = 2.0/(1+n)
    N = len(closing_prices)
    i = N - n

    while i < N:
        price = closing_prices[i]
        result = price * M + result * (1 - M)
        i += 1

    return result


def compute_gain_or_lost(buy, sell):
    return (sell.stock_data.avg_price -
            buy.stock_data.avg_price) / buy.stock_data.avg_price


def choice(short, long):
    """choice(short, long) -> int

    Makes a buying or selling choice given a historic of short and long EMAs

    Keyword argumens:
    short -- historic of short EMA
    long -- historic of long EMA

    Return:
     1: buy
    -1: sell
     0: keep
    """
    n = len(short) - 1
    if n == 0:  # if n == 0, then is the first iteration and we must start buying the stocks.
        return 0

    # compute the difference between the last short and long averages.
    last_diff = long[n-1] - short[n-1]
    # compute the current difference between short and long averages.
    curr_diff = long[n] - short[n]

    # if the last diff > 0 and current diff < 0, it means the averages crossed.
    # At this case, the short averaged increased and crossed long, the we should buy it.
    if last_diff > 0 and curr_diff < 0:
        return 1

    last_diff = short[n-1] - long[n-1]
    curr_diff = short[n] - long[n]

    # At this case, the short averaged decreased and crossed long, then we should sell it.
    if last_diff > 0 and curr_diff < 0:
        return -1

    return 0


def simulate(base2014, base2015, base2016):
    full_base = helpers.merge(base2014, base2015)

    result = {stock: {'orders': [], 'weight': 1.0 /
                      len(STOCKS)} for stock in STOCKS}

    # Start simulation
    for ticker, data2016 in base2016.items():
        stock_data = full_base[ticker]  # Data from 2014/2015 for ticker

        short_averages = []
        long_averages = []
        variances = []

        closing_prices = map(lambda x: x.closing_price, stock_data)

        for data in data2016:
            variances.append(compute_variance(closing_prices))

            # Compute short EMA
            short_averages.append(ema(closing_prices, EMA_SHORT))
            # Compute long EMA
            long_averages.append(ema(closing_prices, EMA_LONG))

            # Save current data for using on next iteration
            closing_prices.append(data.closing_price)

            # Compute choice based on short and long EMA
            c = choice(short_averages, long_averages)

            if c == 1:  # if choice is 1, then send a buying order.
                result[ticker]['orders'].append(
                    order.Order(data, order.BUY_ORDER_TYPE))
            elif c == -1:  # if choice is -1, send a selling order
                result[ticker]['orders'].append(
                    order.Order(data, order.SELL_ORDER_TYPE))

        # print(variances)

        result[ticker]['short_averages'] = short_averages
        result[ticker]['long_averages'] = long_averages

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
