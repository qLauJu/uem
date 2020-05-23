#!/usr/bin/python

# -*- coding: utf-8 -*-

import csv
from collections import defaultdict
from itertools import chain

from stocks import StockData, STOCKS


def read_file(filename):
    result = {stock: [] for stock in STOCKS}

    with open(filename, 'r') as file:
        lines = file.readlines()[1:-1]  # skip header and footer

        for line in lines:
            ticker = line[12:24].strip()

            # load only predefined stocks
            if not ticker in result:
                continue

            date = line[2:10]
            opening_price = line[56:69]
            max_price = line[69:82]
            min_price = line[82:95]
            avg_price = line[95:108]
            closing_price = line[108:121]
            best_buying_price = line[121:133]
            best_selling_price = line[134:147]

            stock = StockData(ticker, date, opening_price, max_price, min_price,
                              avg_price, closing_price, best_buying_price, best_selling_price)

            result[ticker].append(stock)

    return result


def convert_to_money(str_value):
    return float(str_value) / 100


def merge(dict1, dict2):
    result = defaultdict(list)
    for k, v in chain(dict1.items(), dict2.items()):
        result[k] += v

    return result


def date_formatted(date):
    return date[6:] + '/' + date[4:6] + '/' + date[:4]


def print_base(base):
    print("-" * 55)
    print("| {:^51} |".format("GREATEST AVERAGE PRICE PER ticker"))
    print("-" * 55)
    print("| {:^15} | {:^15} | {:^15} |".format("ticker", "DATE", "AVERAGE"))
    print("-" * 55)

    for key, data in base.items():
        biggest_avg_price = max(data, key=lambda x: x.avg_price)
        print("| {:^15} | {:^15} | {:^15} |".format(
            key, biggest_avg_price.date, biggest_avg_price.avg_price))

    print("-" * 55)
