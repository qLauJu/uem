#!/usr/bin/python
# -*- coding: utf-8 -*-

import helpers
import order
from stocks import STOCKS

MMA_SHORT = 3
MMA_LONG = 30

def mma(closing_prices, n):
    '''Calcula a Media Movel Aritmética ou MMA.
    
    Parâmetros:
    closing_prices - lista de preços de fechamento.
    n - período de tempo das médias.
    Retorno:
    result - média móvel aritmética. '''
    
    result = 0
    i = len(closing_prices) - n

    while i < len(closing_prices):
        price = closing_prices[i]
        result = price + result
        i += 1
    
    result = result/n
    
    return result
    
def choice(short, long):
    '''Computa a decisão do algoritmo entre comprar, vender ou manter uma ação.

    Parâmetros:
    short - média móvel aritmética de curto prazo.
    long - média móvel aritmética de longo prazo.
    Retorno:
    0 - manter ação
    1 - comprar ação
    2 - vender ação '''

    n = len(short) - 1
    if n == 0:
        return 1

    last_diff = long[n-1] - short[n-1]
    curr_diff = long[n] - short[n]

    if last_diff > 0 and curr_diff < 0:
        return 1

    last_diff = short[n-1] - long[n-1]
    curr_diff = short[n] - long[n]

    if last_diff > 0 and curr_diff < 0:
        return 2

    return 0

def compute_gain_or_lost(buy, sell):
    '''Computa o ganho ou perda 
    
    Parâmetros:
    buy - dados da ação na compra
    sell - dados da ação na venda
    Retorno:
    valor do ganho ou perda
    '''

    return (sell.avg_price - buy.avg_price) / buy.avg_price

def simulate(base2014, base2015, base2016):
    ''' Define a simulação do algoritmo, simula o ano de 2016 por completo dia-a-dia. 
    
    Parâmetros:
    base2014 - base de dados de 2014 das empresas selecionadas.
    base2015 - base de dados de 2015 das empresas selecionadas.
    base2016 - base de dados de 2016 das empresas selecionadas.
    Retorno:
    result - resultado da simulação (final do ano).
    Obs:
    Note que a base de dados de 2016 é utilizada para controle das datas de operação.
    '''

    old_base = helpers.merge(base2014, base2015)
    old_data = {ticker: data_old for ticker, data_old in old_base.items()}
    main_data = {ticker: data2016 for ticker, data2016 in base2016.items()}

    ticker_list = main_data.keys()
    days = len(main_data[ticker_list[0]])

    result = {stock: {'orders': [], 'weight': 1.0/len(STOCKS)} for stock in STOCKS}
    daily_choice = {ticker: None for ticker in ticker_list}
    last_ops = {ticker: {"buy": None, "sell": None} for ticker in ticker_list}

    short_averages = {ticker: [] for ticker in ticker_list}
    long_averages = {ticker: [] for ticker in ticker_list}
    closing_prices = {ticker: [] for ticker in ticker_list}

    for ticker in ticker_list:
        for day in range(len(old_data[ticker_list[0]])):
            closing_prices[ticker] = map(lambda x: x.closing_price, old_base[ticker])
    
    # Begin Simulation -------------------------------------------------------------------------
    for day in range(days):
        print("\nDay {0} {1} {2}" .format(day+1, 43*"--", main_data[ticker_list[0]][day].date)) #-----------------

        for ticker in ticker_list:
            short_averages[ticker].append(mma(closing_prices[ticker], MMA_SHORT))
            long_averages[ticker].append(mma(closing_prices[ticker], MMA_LONG))
            
            closing_prices[ticker].append(main_data[ticker][day].closing_price)

            decision = choice(short_averages[ticker], long_averages[ticker])
            
            daily_choice[ticker] = decision              

            # print("{0}: closing_price: {1:6} | avg_price: {2:6} | mma_short: {3:7.6} | mma_long: {4:7.6} | Choice: {5} |" .format(ticker,
            #                                                                                                                 main_data[ticker][day].closing_price,
            #                                                                                                                 main_data[ticker][day].avg_price,
            #                                                                                                                 short_averages[ticker][day],
            #                                                                                                                 long_averages[ticker][day],
            #                                                                                                                 decision))
        
        # print("{0}" .format(105 * "-")) #----------------------------
        print("Decisions Taken")
        total_daily_result = 0
        for ticker in ticker_list:
            if daily_choice[ticker] == 2:
                last_ops[ticker]["sell"] = main_data[ticker][day]
                weight_balance = result[ticker]["weight"] * compute_gain_or_lost(last_ops[ticker]["buy"], last_ops[ticker]["sell"])
                result[ticker]["weight"] = result[ticker]["weight"] + weight_balance
                total_daily_result += weight_balance
                print("Sell {0} | Gain/Loss  = {1}" .format(ticker, weight_balance))
                last_ops[ticker]["buy"] = None
            
            elif daily_choice[ticker] == 1:
                print("Buy {0}" .format(ticker))
                last_ops[ticker]["buy"] = main_data[ticker][day]
                last_ops[ticker]["sell"] = None

            elif daily_choice[ticker] == 0: 
                print("Keep {0}" .format(ticker))            

        print("{0}" .format(105 * "-")) #-----------------------------
        print("Daily Total = {0}" .format(total_daily_result))
        print("{0}" .format(105 * "-")) #-----------------------------
        
        sum = 0
        for ticker in ticker_list:
            sum += result[ticker]["weight"]
        
        print("Current Total Weight: {0}" .format(sum))
        print("{0}\n" .format(105 * "-")) #-----------------------------

    sum = 0
    for ticker in ticker_list:
        sum += result[ticker]["weight"]

    result["total"] = sum      

    return result
