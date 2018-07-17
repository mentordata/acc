#include "stdafx.h"
#include <iostream>
#include <string>
#include <time.h>

using namespace std;
#include "acc.h"
#include "lola.h"
#include "acc_functions.h"


int load_config() 
{
	FILE *_file;
	char _idx;
	char _path[200];
	_idx = 0;

	// symbol digits p_open
	sprintf(_path, "%s%s", acc_path, "acc_server.cfg");
	_file = fopen(_path, "r");
	printf("acc_cfg file: %s\n", (_file) ? "OK" : "NG");

	fseek(_file, 0L, SEEK_SET);
	while (fscanf(_file, "%s %lf %d %d %d\n", SymbolBoard[_idx].name, &SymbolBoard[_idx].point, &SymbolBoard[_idx].digits, &SymbolBoard[_idx].p_open, &SymbolBoard[_idx].p_close) != EOF)
	{
		printf("id: %-2d name:%s digits:%d point:%-10.6f p_open:%d p_close:%d\n", _idx, SymbolBoard[_idx].name, SymbolBoard[_idx].digits, SymbolBoard[_idx].point, SymbolBoard[_idx].p_open, SymbolBoard[_idx].p_close);

		DealBoard[_idx].buy_broker = -1;
		DealBoard[_idx].sell_broker = -1;
		DealBoard[_idx].status = ACC_STATUS_NONE;
		_idx++;

	};
	
	
	ACC_SYMBOL_LIMIT = _idx;
	
	fclose(_file);

	return _idx;

}

// return double from string
double get_value(char* _str, int _offset)
{
	double _ret;
	char _buf[ACC_AMOUNT_LEN];

	strncpy(_buf, _str+_offset, ACC_AMOUNT_LEN);

	sscanf(_buf, "%lf", &_ret);

	return _ret;
};

// [
//Ma1.123030  1.123060  1.301270  1.301360  111.145000111.1520003.744100  3.744300  274.710000274.8000000.971680  0.971780  0.749880  0.749950  1.347630  1.347670  0.702830  0.702920  3.564470  3.565670  ]
// updates market table for the board
int update_market(char* _msg, char _broker)
{
	unsigned char  _symbol;


//	_broker = _msg[ACC_MARKET_BROKER] - ACC_LETTER;
	for (_symbol = 0; _symbol < ACC_SYMBOL_LIMIT; _symbol++)
	{
		MarketBoard[_broker][_symbol].active = (get_value(_msg, ACC_MARKET_ASK + (_symbol*ACC_AMOUNT_LEN * 2)) > 0.00001);

		push_tick(get_value(_msg, ACC_MARKET_ASK + (_symbol*ACC_AMOUNT_LEN * 2)), MarketBoard[_broker][_symbol].ask); // = get_value(_msg, ACC_MARKET_ASK + (_symbol*ACC_AMOUNT_LEN * 2));
		push_tick(get_value(_msg, ACC_MARKET_BID + (_symbol*ACC_AMOUNT_LEN * 2)), MarketBoard[_broker][_symbol].bid);
	
	};



	 return _symbol;
};


int update_position(char* _msg, char _broker)
{
	unsigned char  _symbol;
	char _opposite_broker;

//	_broker = _msg[ACC_POS_BROKER] - ACC_LETTER;
	_symbol = _msg[ACC_POS_SYMBOL] - ACC_LETTER;

	PositionBoard[_broker][_symbol].opened = get_value(_msg, ACC_POS_OPENED);
	PositionBoard[_broker][_symbol].closed = get_value(_msg, ACC_POS_CLOSED);

	PositionBoard[_broker][_symbol].profit = get_value(_msg, ACC_POS_PROFIT);
	PositionBoard[_broker][_symbol].commission = get_value(_msg, ACC_POS_COMMISSION);

	PositionBoard[_broker][_symbol].swap = get_value(_msg, ACC_POS_SWAP);
	PositionBoard[_broker][_symbol].status = _msg[ACC_POS_STATUS];

	PositionBoard[_broker][_symbol].point_value = _msg[ACC_POS_POINT_VALUE];

	BrokerBoard[_broker].balance = get_value(_msg, ACC_POS_BALANCE);
	BrokerBoard[_broker].equity = get_value(_msg, ACC_POS_EQUITY);


	if (PositionBoard[_broker][_symbol].status == ACC_POS_STATUS_OPENED)
		position_show(_broker, _symbol);


	if (PositionBoard[_broker][_symbol].status == ACC_POS_STATUS_ERROR)
	{
		position_show(_broker, _symbol);
		PositionBoard[_broker][_symbol].status = ACC_POS_STATUS_NONE;
		DealBoard[_symbol].status = ACC_POS_STATUS_NONE;

		_opposite_broker = (DealBoard[_symbol].buy_broker == _broker) ? DealBoard[_symbol].sell_broker : DealBoard[_symbol].buy_broker;

		BrokerBoard[_broker].errors++; // increase error number

		if (BrokerBoard[_broker].errors > DEF_ACC_ERROR_LIMIT)
			BrokerBoard[_broker].status = ACC_POS_STATUS_ERROR; // disable broker - it is unreliable

		PositionBoard[_opposite_broker][_symbol].status = ACC_POS_STATUS_LEAVE; // leave any positive in 10 secs or close to success
	};

	if (PositionBoard[_broker][_symbol].status == ACC_POS_STATUS_DONE)
	{
		PositionBoard[_broker][_symbol].status = ACC_POS_STATUS_NONE;
		BrokerBoard[_broker].result += (PositionBoard[_broker][_symbol].commission + PositionBoard[_broker][_symbol].profit + PositionBoard[_broker][_symbol].swap);
		DealBoard[_symbol].status = ACC_POS_STATUS_NONE;

		if ((PositionBoard[DealBoard[_symbol].sell_broker][_symbol].status == ACC_POS_STATUS_NONE) && (PositionBoard[DealBoard[_symbol].buy_broker][_symbol].status == ACC_POS_STATUS_NONE))
		{
			DealBoard[_symbol].sell_profit = PositionBoard[DealBoard[_symbol].sell_broker][_symbol].profit;
			DealBoard[_symbol].buy_profit  = PositionBoard[DealBoard[_symbol].buy_broker][_symbol].profit;
			DealBoard[_symbol].sell_commission = PositionBoard[DealBoard[_symbol].sell_broker][_symbol].commission;
			DealBoard[_symbol].buy_commission = PositionBoard[DealBoard[_symbol].buy_broker][_symbol].commission;

			DealBoard[_symbol].result = DealBoard[_symbol].buy_commission + DealBoard[_symbol].buy_profit + DealBoard[_symbol].sell_commission + DealBoard[_symbol].sell_profit;

			deal_show(_symbol);

		};

			position_show(_broker, _symbol);
	};





	return 0;
};


int deal_show(char _symbol)
{
	char _tmpstr[30];
	char _path[200];

	sprintf(_path, "%s%s", acc_path, "acc_server.pos.log");
	acc_file_deal = fopen(_path, "a");
	fprintf(acc_file_deal, "%u,%26s,%c,%s,%s,%s,%.2lf,%.2lf,%.2lf,%.2lf,%.2lf\n",
		DealBoard[_symbol].seq,
		timestamp(_tmpstr),
		DealBoard[_symbol].status,
		BrokerBoard[DealBoard[_symbol].sell_broker].name,
		BrokerBoard[DealBoard[_symbol].buy_broker].name,
		SymbolBoard[_symbol].name,
		DealBoard[_symbol].sell_profit,
		DealBoard[_symbol].sell_commission,
		DealBoard[_symbol].buy_profit,
		DealBoard[_symbol].buy_commission,
		DealBoard[_symbol].result
	);



	fclose(acc_file_deal);
	return 0;
}

int position_show(char _broker, char _symbol)
{
	char _tmpstr[30];
	char _path[200];

	sprintf(_path, "%s%s", acc_path, "acc_server.pos.log");
	acc_file_pos = fopen(_path, "a");


	// status, broker, symbol, side, open, opened, close, closed, commission, profit
//	Sts Broker Symbol Side PriceOpen Opened PriceClose Closed Commission Profit
//		F, mBank, EURUSD, S, 1.121600, 0.000000, 0.000000, 0.00, 0.00, 0.00
//		F, , EURUSD, B, 1.121400, 0.000000, 0.000000, 0.00, 0.00, 0.00
	printf("%10u %26s %3c %3c  %3s %-10s %-10s %-10.6lf %-10.6lf %-10.6lf %-10.6lf %-8.2lf %-8.2lf %-8.2lf %-8.2lf\n",
		PositionBoard[_broker][_symbol].seq,
		timestamp(_tmpstr),
		PositionBoard[_broker][_symbol].status,
		PositionBoard[_broker][_symbol].side,
		PositionBoard[_broker]->reason,
		BrokerBoard[_broker].name,
		SymbolBoard[_symbol].name,
		PositionBoard[_broker][_symbol].price_open,
		PositionBoard[_broker][_symbol].opened,
		PositionBoard[_broker][_symbol].price_close,
		PositionBoard[_broker][_symbol].closed,
		PositionBoard[_broker][_symbol].commission,
		PositionBoard[_broker][_symbol].profit,
		PositionBoard[_broker][_symbol].swap,
		PositionBoard[_broker][_symbol].commission + PositionBoard[_broker][_symbol].profit + PositionBoard[_broker][_symbol].swap
		);


	fprintf(acc_file_pos, "%u,%s,%c,%c,%s,%s,%s,%.6lf,%.6lf,%.6lf,%.6lf,%.2lf,%.2lf,%.2lf,%.2lf\n",
		PositionBoard[_broker][_symbol].seq,
		timestamp(_tmpstr),
		PositionBoard[_broker][_symbol].status,
		PositionBoard[_broker][_symbol].side,
		PositionBoard[_broker]->reason,
		BrokerBoard[_broker].name,
		SymbolBoard[_symbol].name,
		PositionBoard[_broker][_symbol].price_open,
		PositionBoard[_broker][_symbol].opened,
		PositionBoard[_broker][_symbol].price_close,
		PositionBoard[_broker][_symbol].closed,
		PositionBoard[_broker][_symbol].commission,
		PositionBoard[_broker][_symbol].profit,
		PositionBoard[_broker][_symbol].swap,
		PositionBoard[_broker][_symbol].commission + PositionBoard[_broker][_symbol].profit + PositionBoard[_broker][_symbol].swap
		);

	fclose(acc_file_pos);

	return 0;
};

// [TabSab1.03453^TabBac1.03453^TabBac1.03453]
// TabSab1.03453^TabBac1.03453^TabBac1.03453
int get_msg(char* _msg, char (*_mtab)[100][1000]) {
	int _idx; 
	int _midx; 
	int _new_idx;

	char _buf[50]; 
	_idx = 1; // [
	_new_idx = 0;

	_midx=0;
	while(_msg[_idx] && _msg[_idx]!=ACC_MSG_END)
	{ 
		if(_msg[_idx]==ACC_MSG_SEPARATOR) 
		{ 
			(*_mtab)[_midx][_new_idx]='\0'; // thats end  
			_midx++;  
			_new_idx = 0;	// from beginning
		} 
		else 
		{ 
			(*_mtab)[_midx][_new_idx] = _msg[_idx];
			_new_idx++;
		};
		_idx++;
	};

	(*_mtab)[_midx][_new_idx]='\0'; // thats end 

	return _midx+1;
};


char* timestamp(char* _ret)
{

	time_t current_time;
	tm *_curr_time;

	current_time = time(NULL);

	_curr_time = localtime(&current_time);



	sprintf(_ret, "%d%02d%02d_%02d:%02d:%02d", _curr_time->tm_year+1900, _curr_time->tm_mon+1, _curr_time->tm_mday, _curr_time->tm_hour, _curr_time->tm_min, _curr_time->tm_sec);

	return _ret;
}

char* command(char* _cmd, char* _ret)
{
	// returns data for operator
	// market
	// pos
	char _buf[10000];
	char _bufall[100000];
	char _idx, _idx2, _broker, _symbol, _ask_broker, _bid_broker, _buy_brk, _sell_brk;
	double _minask, _maxbid, _diff, _portfolio;
	struct ACC_POSITION _sell_pos, _buy_pos;
	char _hair[4];
	int _ret_flag;

	_bufall[0] = '\0';  // empty


	_ret_flag = 0;
	// MARKET information
	if (!strcmp(_cmd, "market"))
	{
		sprintf(_bufall, "Brk  Sym    Ask       Bid        Timestamp\n");


		for (_idx = 0; _idx<ACC_BROKER_LIMIT; _idx++)
			for (_idx2 = 0; _idx2<ACC_SYMBOL_LIMIT; _idx2++)
			{
				sprintf(_buf, "%c    %c   %10.6lf %10.6lf %s\n", _idx + ACC_LETTER, _idx2 + ACC_LETTER, MarketBoard[_idx][_idx2].ask[0], MarketBoard[_idx][_idx2].bid[0], MarketBoard[_idx][_idx2].timestamp);
				strcat(_bufall, _buf);
			};

	};

	if (!strcmp(_cmd, "help"))
	{
		sprintf(_bufall,"\nACC server v.%s\n",ACC_VERSION);
		strcat(_bufall, "Help \n================\n");
		strcat(_bufall, "help       - this help\n");
		strcat(_bufall, "connect (c)- connect single broker\n");
		strcat(_bufall, "lola (r)   - Run Lola Run\n");
		strcat(_bufall, "info       - general info about brokers and symbols\n");
		strcat(_bufall, "stop       - kill them all. That's it\n");
		strcat(_bufall, "clear_pos  - clear all positions\n");
		strcat(_bufall, "clear_brk  - clear all brokers\n");

		strcat(_bufall, "test_open  - open position on broker\n");
		strcat(_bufall, "test_close - close position on broker\n");

		strcat(_bufall, "market     - market info\n");
		strcat(_bufall, "reload     - reloads config\n");
		strcat(_bufall, "pos        - position information\n");
		strcat(_bufall, "result     - result information\n");
		strcat(_bufall, "diff       - differences in market\n");
		strcat(_bufall, "deal       - current deal board\n");
	};


	if (!strcmp(_cmd, "reload"))
	{
		load_config();
	};

	if (!strcmp(_cmd, "clear_pos"))
	{
		for (_idx = 0; _idx < DEF_ACC_BROKER_LIMIT; _idx++)
			for (_idx2 = 0; _idx2 < DEF_ACC_SYMBOL_LIMIT; _idx2++)
			{
				PositionBoard[_idx][_idx2].status = ACC_POS_STATUS_NONE;
				PositionBoard[_idx][_idx2].price_open = 0;
				PositionBoard[_idx][_idx2].opened = 0;
				PositionBoard[_idx][_idx2].price_close = 0;
				PositionBoard[_idx][_idx2].closed = 0;
				PositionBoard[_idx][_idx2].profit = 0;
				PositionBoard[_idx][_idx2].commission = 0;

			};

		sprintf(_bufall, "PositionBoard cleared (status, prices, profit, comm)\n");
	};

	if (!strcmp(_cmd, "clear_brk"))
	{
		for (_idx = 0; _idx < DEF_ACC_BROKER_LIMIT; _idx++)
		{
			BrokerBoard[_idx].errors = 0;
			BrokerBoard[_idx].status = ACC_STATUS_ACTIVE;
		};
		sprintf(_bufall, "BrokerBoard cleared (status, errors)\n");
	};


	if (!strcmp(_cmd, "info"))
	{
		sprintf(_bufall, "Brokers : %d\n================\n", ACC_BROKER_LIMIT);
		_portfolio = 0;

		for (_idx = 0; _idx<ACC_BROKER_LIMIT; _idx++)
		{
			sprintf(_buf, "id: %-2d name:%-10s balance:%-10.2lf equity: %-10.2lf result: %-10.2lf tick:%-10d freq:%-4d status:%c errors:%d\n", 
				_idx, BrokerBoard[_idx].name, BrokerBoard[_idx].balance, BrokerBoard[_idx].equity, BrokerBoard[_idx].result, BrokerBoard[_idx].broker_tick,
				BrokerBoard[_idx].frequency, BrokerBoard[_idx].status, BrokerBoard[_idx].errors );
			_portfolio += BrokerBoard[_idx].result;

			strcat(_bufall, _buf);
		};
		sprintf(_buf, "Summary : %.2lf\n================\n", _portfolio);
		strcat(_bufall, _buf);

		sprintf(_buf, "Symbols : %d\n================\n", ACC_SYMBOL_LIMIT);
		strcat(_bufall, _buf);

		for (_idx = 0; _idx<ACC_SYMBOL_LIMIT; _idx++)
		{
			sprintf(_buf, "id: %-2d (%c) name:%-10s digits:%d point:%0.6lf p_open: %-3d p_close: %-3d\n", _idx, _idx+ACC_LETTER, SymbolBoard[_idx].name, SymbolBoard[_idx].digits, SymbolBoard[_idx].point, SymbolBoard[_idx].p_open, SymbolBoard[_idx].p_close);
			strcat(_bufall, _buf);
		};

		sprintf(_buf, "Deals : %d\n================\n", ACC_SYMBOL_LIMIT);
		strcat(_bufall, _buf);

		for (_idx = 0; _idx<ACC_SYMBOL_LIMIT; _idx++)
		{
			sprintf(_buf, "seq:%d name:%-10s buy:%-10s sell:%-10s status:%c\n", DealBoard[_idx].seq, SymbolBoard[_idx].name,
				(DealBoard[_idx].buy_broker>=0)?BrokerBoard[DealBoard[_idx].buy_broker].name :"",
				(DealBoard[_idx].sell_broker >= 0) ? BrokerBoard[DealBoard[_idx].sell_broker].name :"", DealBoard[_idx].status);
			strcat(_bufall, _buf);
		};


		sprintf(_buf, "\nGirl\n================\n tick:%d freq:%d status:%c", GirlBoard.girl_tick, GirlBoard.frequency, GirlBoard.status);
		strcat(_bufall, _buf);

	};

	if (!strcmp(_cmd, "position")|| !strcmp(_cmd, "pos"))
	{
		sprintf(_bufall, "%-10s %-10s %4s %4s %10s %10s %10s %10s %10s\n","Broker","Symbol","Sts","Side","Commission","Profit","Result","slip_open","slip_close");

		
		for (_idx = 0; _idx<ACC_BROKER_LIMIT; _idx++)
			for (_idx2 = 0; _idx2<ACC_SYMBOL_LIMIT; _idx2++)
			{
				_buf[0] = '\0'; 
				if (PositionBoard[_idx][_idx2].status != ACC_POS_STATUS_NONE)
					{
					sprintf(_buf, "%-10s %-10s %4c %4c %-8.2lf %-8.2lf %-8.2lf %-8.6lf %-8.6lf\n",
						BrokerBoard[_idx].name, SymbolBoard[_idx2].name, PositionBoard[_idx][_idx2].status, PositionBoard[_idx][_idx2].side,
						PositionBoard[_idx][_idx2].commission, PositionBoard[_idx][_idx2].profit, PositionBoard[_idx][_idx2].commission + PositionBoard[_idx][_idx2].profit,
						((PositionBoard[_idx][_idx2].side == 'B') ? (PositionBoard[_idx][_idx2].price_open - PositionBoard[_idx][_idx2].opened) : (PositionBoard[_idx][_idx2].opened - PositionBoard[_idx][_idx2].price_open)),
						((PositionBoard[_idx][_idx2].side == 'B') ? (PositionBoard[_idx][_idx2].closed - PositionBoard[_idx][_idx2].price_close) : (PositionBoard[_idx][_idx2].price_close - PositionBoard[_idx][_idx2].closed))
					);
					};
				strcat(_bufall, _buf);
			};

	};


	if (!strcmp(_cmd, "result"))
	{
		sprintf(_bufall,"broker_sell broker_buy symbol     price_sell   price_buy   opened_sell   opened_buy   close_sell   close_buy  closed_sell  closed_buy sell_commission buy_commission sell_profit   buy_profit   sell_result buy_result\n");

		for (_symbol = 0; _symbol < ACC_SYMBOL_LIMIT; _symbol++)
		{
			_buy_brk = get_buy_broker(_symbol);
			_sell_brk = get_sell_broker(_symbol);
			_buy_pos = PositionBoard[_buy_brk][_symbol];
			_sell_pos = PositionBoard[_sell_brk][_symbol];
			if (PositionBoard[_buy_brk][_symbol].status != ACC_POS_STATUS_NONE)
			{
				sprintf(_bufall + strlen(_bufall), "%-10s, %-10s, %-10c, %-10.6lf, %-10.6lf, %-10.6lf, %-10.6lf, %-10.6lf, %-10.6lf, %-10.6lf,%-10.6lf, %-10.6lf, %-10.6lf, %-10.6lf, %-10.6lf, %-10.6lf, %-10.6lf, %-10.6lf, %-10.6lf\n", \
					BrokerBoard[_buy_brk].name, \
					BrokerBoard[_sell_brk].name, \
					_symbol + ACC_LETTER, \
					_sell_pos.price_open, \
					_buy_pos.price_open, \
					_sell_pos.opened, \
					_buy_pos.opened, \
					_sell_pos.price_close, \
					_buy_pos.price_close, \
					_sell_pos.closed, \
					_buy_pos.closed, \
					_sell_pos.commission, \
					_buy_pos.commission, \
					_sell_pos.profit, \
					_buy_pos.profit, \
					_sell_pos.swap, \
					_buy_pos.swap, \
					_sell_pos.profit + _sell_pos.swap + _sell_pos.commission, \
					_buy_pos.profit + _buy_pos.swap + _buy_pos.commission);		// 19 arguments
			};

		};

	};
	if (!strcmp(_cmd, "test_open"))
	{
		test_open = 1;
		sprintf(_bufall,"test_open  - start\n");
	};
	if (!strcmp(_cmd, "test_close"))
	{
		test_close = 1;
		sprintf(_bufall, "test_close  - start\n");
	};

	if (!strcmp(_cmd, "diff")) 
	{

		sprintf(_bufall, "sym   bid        ask        diff     \n");
		for (_symbol = 0; _symbol < ACC_SYMBOL_LIMIT; _symbol++)
		{

		_minask = MarketBoard[0][_symbol].ask[0];
		_maxbid = MarketBoard[0][_symbol].bid[0];

		_ask_broker = _bid_broker = 0;

		for (_idx = 1; _idx<ACC_BROKER_LIMIT; _idx++)
		{
			if (_minask>MarketBoard[_idx][_symbol].ask[0])
			{
				_minask = MarketBoard[_idx][_symbol].ask[0];         // new broker has lower ask
				_ask_broker = _idx;
			};

			if (_maxbid<MarketBoard[_idx][_symbol].bid[0])
			{
				_maxbid = MarketBoard[_idx][_symbol].bid[0];
				_bid_broker = _idx;
			};
		};


		_diff = _maxbid - _minask;

		if (_diff > 0.0)		// there is something
		{
			sprintf(_buf, "%c  %c_%10.6lf   %c_%10.6lf   %1.6lf\n", _symbol + ACC_LETTER, _bid_broker + ACC_LETTER, _maxbid, _ask_broker + ACC_LETTER, _minask, _maxbid - _minask);
			strcat(_bufall, _buf);
			_ret_flag = 1;
		};

		};

	};

	//printf("%d %d\n",_idx, _idx2);
	sprintf(_ret,"%s\n", _bufall);

	return _ret;
};

int push_tick(double _value, double *_tab)
{// _tab[0] - most actual
 // _tab[_len-1] - oldest

	int _len;
	
	_len = DEF_ACC_MARKET_LIMIT;

	while (--_len)
		*(_tab+_len) = *(_tab+_len - 1); // moves 

	*_tab = _value;
	return 0;
};

char get_buy_broker(char _symbol)
{// returns two symetric positions for the _symbol; F-illed, S-ent, O-pened C-losed// and FSO not C// only two first positions
	char _ask_brk, _bid_brk;

	for(_ask_brk=0; _ask_brk<ACC_BROKER_LIMIT && (!PositionBoard[_ask_brk][_symbol].status) && (!(PositionBoard[_ask_brk][_symbol].side==ACC_SIDE_BUY));_ask_brk++);
	
	return _ask_brk;
};

char get_sell_broker(char _symbol)
{// returns two symetric positions for the _symbol; F-illed, S-ent, O-pened C-losed// and FSO not C// only two first positions
	char _ask_brk, _bid_brk;

	for (_bid_brk = 0; _bid_brk<ACC_BROKER_LIMIT && (!PositionBoard[_bid_brk][_symbol].status) && (!(PositionBoard[_bid_brk][_symbol].side == ACC_SIDE_SELL)); _bid_brk++);

	return _bid_brk;
};


int close_odds(ACC_POSITION PositionBoard[DEF_ACC_BROKER_LIMIT][DEF_ACC_SYMBOL_LIMIT])
{
	char _broker, _symbol;// single opened leg ?// negative profit// 
	int _idx, _idx2;
	ACC_POSITION _pos;
	for(_idx=0; _idx<ACC_BROKER_LIMIT; _idx++)
		for(_idx2=0; _idx2<ACC_BROKER_LIMIT; _idx2++) 
		{ 
			_pos=PositionBoard[_idx][_idx2];  
		};
return 0;
};

