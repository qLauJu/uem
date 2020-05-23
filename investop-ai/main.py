#!/usr/bin/python
# -*- coding: utf-8 -*-

import csv

import helpers as helpers
import guilherme as guilherme
import puglia as puglia
import vendrame as vendrame
from stocks import STOCKS
import oracle
import sys


COTHIST2014_FILENAME = 'bases/COTAHIST_A2014.TXT'
COTHIST2015_FILENAME = 'bases/COTAHIST_A2015.TXT'
COTHIST2016_FILENAME = 'bases/COTAHIST_A2016.TXT'


def print_result(result):
    print("-" * 93)
    print("| {:^20s} | {:^20s} | {:^20s} | {:^20s} |".format(
        'TICKER', 'INITIAL', 'FINAL', 'GAIN/LOSS'))
    print("-" * 93)
    initial = 1.0 / len(STOCKS)

    sum = 0
    for ticker in STOCKS:
        final = result[ticker]['weight']
        sum += final
        print("| {:^20s} | {:^20.3f} | {:^20.3f} | {:^20.3} |".format(
            ticker, initial, final, final - initial))

    print("-" * 93)

    print("| {:^20}   {:^20}   {:^20}   {:^20.3f} |".format(
        " ", " ", "TOTAL GAIN/LOSS", result['total']))

    print("-" * 93)


def main():
    base2014 = helpers.read_file(COTHIST2014_FILENAME)
    base2015 = helpers.read_file(COTHIST2015_FILENAME)
    base2016 = helpers.read_file(COTHIST2016_FILENAME)

    print("""
Simulador de Investimentos

1) Guilherme
2) Leonardo Puglia
3) Leonardo Vendrame
4) Oráculo

0) Sair
    """)

    sys.stdout.write("=> ")
    sys.stdout.flush()
    option = int(raw_input())

    if option == 0:
        print("Saindo.")
    elif option == 1:
        print_result(guilherme.simulate(base2014, base2015, base2016))
    elif option == 2:
        print_result(puglia.simulate(base2014, base2015, base2016))
    elif option == 3:
        vendrame.simulate(base2014, base2015, base2016)
    elif option == 4:
        print_result(oracle.futute(base2016, STOCKS))
    else:
        print("Opção inválida.")


if __name__ == "__main__":
    main()
