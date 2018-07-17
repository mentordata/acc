#include <stdio.h>
#include <math.h>
#include "stdafx.h"
#include "acc.h"
#include "acc_functions.h"

// LOLA is 2 brokers arbitrage algo. 3 diff closure algoritms Green, Red, Blonde. Single symbol per broker. Multiple positions per broker

int lola_prepare(char* _cmd)
{
	


	return 0;
}

int lola_open(char _symbol)
{
	// dokonuje otwarcia dwoch stron na podstawie market dla _symbolu
	//
	char _idx, _idx2;
	char _idx_brk_sell, _idx_brk_buy;
	char _found;
	double _maxbid, _minask, _profit, _last_profit;
	char _size_sell, _size_buy;
	char _bid_broker, _ask_broker;
	double *_bidtab, *_asktab;
	double _sell_bid, _sell_ask, _buy_bid, _buy_ask;
	_profit = 1; //to change or calculate

	if (test_open && (_symbol==0))	// simulation
	{
		MarketBoard[1][_symbol].ask[0] = MarketBoard[0][_symbol].bid[0] - 0.0003;		// oportunity
		MarketBoard[1][_symbol].bid[0] = MarketBoard[0][_symbol].bid[0] - 0.0010;
		MarketBoard[1][_symbol].ask[1] = MarketBoard[0][_symbol].bid[0] - 0.0003;		// oportunity
		MarketBoard[1][_symbol].bid[1] = MarketBoard[0][_symbol].bid[0] - 0.0010;
		MarketBoard[1][_symbol].active = 1;
		test_open = 0;
		//		ACC_BROKER_LIMIT = 2;
	};


	// sprawdz czy jest ju¿ pozycja otwarta dla symbolu - w sensie ¿e cokolwiek jest ju¿ zrobione
	_found = 0;
	for (_idx = 0; _idx<ACC_BROKER_LIMIT && !_found; _idx++)
		_found = (PositionBoard[_idx][_symbol].status != 'N');          // is not none

	if (_found)
		return -1;      // position already found

	_minask = 9999999;		// MarketBoard[0][_symbol].ask[0];
	_maxbid = 0;	// MarketBoard[0][_symbol].bid[0];
	_bidtab = MarketBoard[0][_symbol].bid;
	_asktab = MarketBoard[0][_symbol].ask;
	_last_profit = 0;
	_ask_broker = _bid_broker = -1;


// profit oriented selection
	for (_idx_brk_sell = 0; _idx_brk_sell<ACC_BROKER_LIMIT; _idx_brk_sell++)
		for (_idx_brk_buy = 0; _idx_brk_buy<ACC_BROKER_LIMIT; _idx_brk_buy++)
			if ((_idx_brk_buy != _idx_brk_sell) && (BrokerBoard[_idx_brk_buy].status == ACC_STATUS_ACTIVE) && (BrokerBoard[_idx_brk_sell].status == ACC_STATUS_ACTIVE) && MarketBoard[_idx_brk_sell][_symbol].active && MarketBoard[_idx_brk_buy][_symbol].active)
			{
			
				// _profit in double EUR
				_profit = (((MarketBoard[_idx_brk_sell][_symbol].bid[0] - MarketBoard[_idx_brk_buy][_symbol].ask[0]) -
					(MarketBoard[_idx_brk_buy][_symbol].ask[0] - MarketBoard[_idx_brk_buy][_symbol].bid[0])) *
					AccDigits[SymbolBoard[_symbol].digits] * SymbolBoard[_symbol].point)
					+ BrokerBoard[_idx_brk_sell].commission[_symbol] + BrokerBoard[_idx_brk_buy].commission[_symbol];
							

				if (_profit > (SymbolBoard[_symbol].p_open*SymbolBoard[_symbol].point))			// profitable or not
					if(_profit>_last_profit)
					{
						_ask_broker = _idx_brk_buy;
						_bid_broker = _idx_brk_sell;
						_last_profit = _profit;
					};
			};

	if ((_ask_broker == -1) || (_bid_broker == -1))			// no trade allowed - no pair instruments
		return -2;

	if (_bid_broker == _ask_broker)
		return -3;						// the same broker - suspicious


	// wylicz size na podstawie bid miedzy 0.1 a 10 lotow
	_size_buy = 1;
	_size_sell = 1;




	// utworz pozycje
	// SELL position
	PositionBoard[_bid_broker][_symbol].seq = tick_seq;
	PositionBoard[_bid_broker][_symbol].side = 'S';
	PositionBoard[_bid_broker][_symbol].size = _size_sell;
	PositionBoard[_bid_broker][_symbol].price_open = MarketBoard[_bid_broker][_symbol].bid[0];
	PositionBoard[_bid_broker][_symbol].price_close = 0.0;
	PositionBoard[_bid_broker][_symbol].opened = 0.0;
	PositionBoard[_bid_broker][_symbol].closed = 0.0;
	PositionBoard[_bid_broker][_symbol].slip = 0;          
	PositionBoard[_bid_broker][_symbol].profit = 0.0;
	PositionBoard[_bid_broker][_symbol].commission = 0.0;
	PositionBoard[_bid_broker][_symbol].swap = 0.0;
	PositionBoard[_bid_broker][_symbol].status = ACC_POS_STATUS_FILLED;       // filled


															// BUY position
	PositionBoard[_ask_broker][_symbol].seq = tick_seq;
	PositionBoard[_ask_broker][_symbol].side = 'B';
	PositionBoard[_ask_broker][_symbol].size = _size_buy;
	PositionBoard[_ask_broker][_symbol].price_open = MarketBoard[_ask_broker][_symbol].ask[0];
	PositionBoard[_ask_broker][_symbol].price_close = 0.0;
	PositionBoard[_ask_broker][_symbol].opened = 0.0;
	PositionBoard[_ask_broker][_symbol].closed = 0.0;
	PositionBoard[_ask_broker][_symbol].slip = 0;          
	PositionBoard[_ask_broker][_symbol].profit = 0.0;
	PositionBoard[_ask_broker][_symbol].commission = 0.0;
	PositionBoard[_ask_broker][_symbol].swap = 0.0;
	PositionBoard[_ask_broker][_symbol].status = ACC_POS_STATUS_FILLED;       // filled


	DealBoard[_symbol].buy_broker  = _ask_broker;
	DealBoard[_symbol].sell_broker = _bid_broker;
	DealBoard[_symbol].status = ACC_POS_STATUS_FILLED;
	DealBoard[_symbol].seq = tick_seq;
	DealBoard[_symbol].sell_profit = 0.0;
	DealBoard[_symbol].sell_commission = 0.0;
	DealBoard[_symbol].buy_profit = 0.0;
	DealBoard[_symbol].buy_commission = 0.0;
	DealBoard[_symbol].result = 0.0;




	if ((ACC_MODE == 'D') || (ACC_MODE == 'S'))
	{
		position_show(_bid_broker, _symbol);
		position_show(_ask_broker, _symbol);
	};

	return 0;
};


char lola_hair(char _symbol)
{
	// podejmuje decyzje na podstawie market i position i zwraca kolor wlosow
	// R-ed         - brutalna, na podstawie realnej informacji o profit/commission
	// G-reen       - szalona, na podstawie wyliczonego profitu open-market
	// B-londe      - naiwna, na podstawie braku arbitrazu ask1, ask2 > bid1, bid2

	char _idx;
	char _sell_broker, _buy_broker;
	double _real_profit, _profit;
	double _pips_profit;
	char _green, _red, _blonde, _ret;
	// odszukanie brokerow sell i buy
	double _points;
	_ret = '-';
	_sell_broker = _buy_broker = -1;    // not found

	if  ((_symbol == 0) && test_close)	// simulation
	{
		MarketBoard[1][_symbol].ask[0] = MarketBoard[0][_symbol].ask[0] - 0.00;		// 
		MarketBoard[1][_symbol].bid[0] = MarketBoard[0][_symbol].bid[0] - 0.00;
		MarketBoard[1][_symbol].ask[1] = MarketBoard[0][_symbol].ask[0] - 0.00;		// no arbitrage - blonde
		MarketBoard[1][_symbol].bid[1] = MarketBoard[0][_symbol].bid[0] - 0.00;
		PositionBoard[0][_symbol].status = ACC_POS_STATUS_OPENED;
		PositionBoard[1][_symbol].status = ACC_POS_STATUS_OPENED;
		test_close = 0;

//		ACC_BROKER_LIMIT = 2;
	};


	for (_idx = 0; _idx<ACC_BROKER_LIMIT; _idx++)
	{												// have to be ACC_POS_STATUS_OPENED - sent is for lola diagnostic
		if ((PositionBoard[_idx][_symbol].status == ACC_POS_STATUS_OPENED) && (PositionBoard[_idx][_symbol].side == 'S'))             //  opened
			_sell_broker = _idx;

		if ((PositionBoard[_idx][_symbol].status == ACC_POS_STATUS_OPENED) && (PositionBoard[_idx][_symbol].side == 'B'))             //  opened
			_buy_broker = _idx;
	};


	// sprawdzenie czy sa poprawnie otwarte pozycje dla danego symbolu i brokera

	if ((_buy_broker == -1) || (_sell_broker == -1))
		return -1;                              // problem with positions, not correctly opened, broker not found
	// profit in double quote
	_profit = (SymbolBoard[_symbol].p_open * (double)(1.0 / AccDigits[SymbolBoard[_symbol].digits]));
	
	// porownanie wyniku realnego z (profit)
	_real_profit = PositionBoard[_buy_broker][_symbol].commission + \
		PositionBoard[_buy_broker][_symbol].profit + \
		PositionBoard[_buy_broker][_symbol].swap + \
		PositionBoard[_sell_broker][_symbol].commission + \
		PositionBoard[_sell_broker][_symbol].profit + \
		PositionBoard[_sell_broker][_symbol].swap;
	
	_red = _real_profit >= 1.0;

	_points = (double)((
				(MarketBoard[_buy_broker][_symbol].bid[0] - PositionBoard[_buy_broker][_symbol].opened) + // buy side
				(PositionBoard[_sell_broker][_symbol].opened - MarketBoard[_sell_broker][_symbol].ask[0]) // sell side
				));


	_green = (_points > _profit);           // to change calculation

// midpoints sell point almost equal to buy
	_blonde = (
				(MarketBoard[_buy_broker][_symbol].bid[0] + MarketBoard[_buy_broker][_symbol].ask[0]) >=	
				(MarketBoard[_sell_broker][_symbol].bid[0] + MarketBoard[_sell_broker][_symbol].ask[0])
				);  

	if (MarketBoard[_buy_broker][_symbol].bid[0] < MarketBoard[_buy_broker][_symbol].bid[1])	// not falling edge for open
		return -6;

	if (MarketBoard[_sell_broker][_symbol].ask[0] > MarketBoard[_sell_broker][_symbol].ask[1])	// not rising edge for sell
		return -7;


	// podjecie decyzji


	// zamkniecie pozycji
	if (_blonde||_green)
	{
		PositionBoard[_buy_broker][_symbol].price_close = MarketBoard[_buy_broker][_symbol].bid[0];
		PositionBoard[_sell_broker][_symbol].price_close = MarketBoard[_sell_broker][_symbol].ask[0];
		PositionBoard[_buy_broker][_symbol].status = ACC_POS_STATUS_CLOSE;       // Close
		PositionBoard[_sell_broker][_symbol].status = ACC_POS_STATUS_CLOSE;       // Close
		PositionBoard[_buy_broker][_symbol].reason[0] = _blonde ? 'B' : '_';       // 
		PositionBoard[_buy_broker][_symbol].reason[1] = _green ? 'G' : '_';       // 
		PositionBoard[_buy_broker][_symbol].reason[2] = _red ? 'R' : '_';       // 
		PositionBoard[_buy_broker][_symbol].reason[3] = '\0';       // 

		PositionBoard[_sell_broker][_symbol].reason[0] = _blonde ? 'B' : '_';       // 
		PositionBoard[_sell_broker][_symbol].reason[1] = _green ? 'G' : '_';       // 
		PositionBoard[_sell_broker][_symbol].reason[2] = _red ? 'R' : '_';       // 
		PositionBoard[_sell_broker][_symbol].reason[3] = '\0';       // 

//		PositionBoard[_sell_broker][_symbol].reason = (_blonde * 2 + _green) + ACC_LETTER;       // 

		DealBoard[_symbol].status = ACC_POS_STATUS_CLOSE;

		if (ACC_MODE == 'D' || ACC_MODE == 'S')
		{
			printf("LOLA hair [%s %s]: %10.6lf > %10.6lf\n",(_blonde?"blonde":""), (_green?"green":""), MarketBoard[_buy_broker][_symbol].ask[0], MarketBoard[_sell_broker][_symbol].bid[0]);
			position_show(_buy_broker, _symbol);
			position_show(_sell_broker, _symbol);
		};

	};

	return 0;
};



int lola_trade(char *_msg, char _broker)
{

	int _idx;
	char _trade[100];
	char _symbol;
	char _first;

	_idx = 0;
	_first = 0;
	_msg[_idx++] = ACC_MSG_BEGIN;  // [ character
	
	for (_symbol = 0; _symbol<ACC_SYMBOL_LIMIT; _symbol++)
	{// [separator] T _broker _symbol side size slip price 
		if (PositionBoard[_broker][_symbol].status == ACC_POS_STATUS_FILLED)		// Filled 
		{

			if (_first)
			{
				sprintf_s(_trade, "%cT%c%c%c%c", 
					ACC_MSG_SEPARATOR, 
					_broker + ACC_LETTER, _symbol + ACC_LETTER, 
					PositionBoard[_broker][_symbol].side,
					PositionBoard[_broker][_symbol].size + ACC_LETTER
					);
			}
			else
			{
				sprintf_s(_trade, "T%c%c%c%c", \
					_broker + ACC_LETTER, _symbol + ACC_LETTER, \
					PositionBoard[_broker][_symbol].side,
					PositionBoard[_broker][_symbol].size + ACC_LETTER
					);

			};

			strncpy(_msg + _idx, _trade, strlen(_trade));
			_idx += strlen(_trade);
			_first = 1;		// first message is achived, so please add msg separator

			PositionBoard[_broker][_symbol].status = ACC_POS_STATUS_SENT;			// S-ent
			DealBoard[_symbol].status = ACC_POS_STATUS_SENT;

		};

		if (PositionBoard[_broker][_symbol].status == ACC_POS_STATUS_CLOSE)				// CLOSE 
		{

			if (_first)
			{
				sprintf_s(_trade, "%cT%c%c%c%c", \
					ACC_MSG_SEPARATOR, \
					_broker + ACC_LETTER, _symbol + ACC_LETTER, \
					ACC_POS_STATUS_CLOSE,
					PositionBoard[_broker][_symbol].size + ACC_LETTER
					);
			}
			else
			{
				sprintf_s(_trade, "T%c%c%c%c", \
					_broker + ACC_LETTER, _symbol + ACC_LETTER, \
					ACC_POS_STATUS_CLOSE,
					PositionBoard[_broker][_symbol].size + ACC_LETTER
					);
			};

			PositionBoard[_broker][_symbol].status = ACC_POS_STATUS_WAIT; // S-ent
			DealBoard[_symbol].status = ACC_POS_STATUS_WAIT;

			strncpy(_msg + _idx, _trade, strlen(_trade));
			_idx += strlen(_trade);
			_first = 1;		// first message is achived, so please add msg separator
		};

		if (PositionBoard[_broker][_symbol].status == ACC_POS_STATUS_LEAVE)				// LEAVE CLOSE 
		{

			if (_first)
			{
				sprintf_s(_trade, "%cT%c%c%c%c", \
					ACC_MSG_SEPARATOR, \
					_broker + ACC_LETTER, _symbol + ACC_LETTER, \
					ACC_POS_STATUS_LEAVE,
					PositionBoard[_broker][_symbol].size + ACC_LETTER
				);
			}
			else
			{
				sprintf_s(_trade, "T%c%c%c%c", \
					_broker + ACC_LETTER, _symbol + ACC_LETTER, \
					ACC_POS_STATUS_LEAVE,
					PositionBoard[_broker][_symbol].size + ACC_LETTER
				);
			};

			PositionBoard[_broker][_symbol].status = ACC_POS_STATUS_WAIT; // S-ent
			DealBoard[_symbol].status = ACC_POS_STATUS_WAIT;

			strncpy(_msg + _idx, _trade, strlen(_trade));
			_idx += strlen(_trade);
			_first = 1;		// first message is achived, so please add msg separator
		};

	};



	_msg[_idx++] = ACC_MSG_END;
	_msg[_idx++] = '\0';

	

	return _idx;
};