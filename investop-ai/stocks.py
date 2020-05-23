# -*- coding: utf-8 -*-

import helpers as helpers
from datetime import datetime

STOCKS = set(['ITUB4', 'BBSE3', 'BBAS3', 'CIEL3', 'ECOR3',
              'GGBR4', 'CSAN3', 'PETR4', 'RUMO3', 'MGLU3'])


class StockData:
    def __init__(self, ticket, date, opening_price, max_price, min_price, avg_price, closing_price, best_buying_price, best_selling_price):
        """Initializes a new StockData object, all arguments must be `strings`"""
        self.ticket = ticket
        self.date = datetime.strptime(date, "%Y%m%d").date()
        self.opening_price = helpers.convert_to_money(opening_price)
        self.max_price = helpers.convert_to_money(max_price)
        self.min_price = helpers.convert_to_money(min_price)
        self.avg_price = helpers.convert_to_money(avg_price)
        self.closing_price = helpers.convert_to_money(closing_price)
        self.best_buying_price = helpers.convert_to_money(best_buying_price)
        self.best_selling_price = helpers.convert_to_money(best_selling_price)

    def __str__(self):
        return "ticket: {}\ndate: {}\navg_price: {}\n".format(self.ticket, self.date, self.avg_price)
