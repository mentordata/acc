//#pragma once
/*
// ACC headers, MentorData Henryk Poplawski 2017
// -123100.098100
// positions in message acc_pos_header[1]:broker[1]:id[3]:id_symbol[1]:side[1]:size[1](in unit):price[10]:open[10]:profit[10]:commission[10]:swap[10]:status[1]
//                         A
// ACC_POS len=50
*/
#ifndef ACC_H_     // equivalently, #if !defined HEADER_H_  
#define ACC_H_  
// Code placed here is included only once per translation unit  

#include <time.h>
#include <windows.h>

#define ACC_VERSION "1.3"
#define ACC_HEADER 0
#define ACC_POS_BROKER 1
#define ACC_POS_SYMBOL 2
#define ACC_POS_SIDE 3
#define ACC_POS_OPENED 4
#define ACC_POS_CLOSED 14
#define ACC_POS_PROFIT 24
#define ACC_POS_COMMISSION 34
#define ACC_POS_SWAP 44
#define ACC_POS_STATUS 54
#define ACC_POS_BALANCE 55
#define ACC_POS_EQUITY 65
#define ACC_POS_POINT_VALUE 75



// 2017-05-05-19.08.18.487474 
//acc_market_header[1]:broker[1]:symbol[1]:bid[10]:ask[10]:timestamp[26]
// ACC_MARKET

#define ACC_HEADER 0
#define ACC_MARKET_BROKER 1
//#define ACC_MARKET_SYMBOL 2
#define ACC_MARKET_BID 2
#define ACC_MARKET_ASK 12
//#define ACC_MARKET_TIMESTAMP 23

//
// acc_trade_header[1]:broker[1]:id[3]:symbol[1]:trade[1]:size[1](units):slip[1]:price[10]
// TRADE
#define ACC_HEADER 0
#define ACC_TRADE_BROKER 1
#define ACC_TRADE_SYMBOL 2
#define ACC_TRADE_TRADE 3
#define ACC_TRADE_SIZE 4
#define ACC_TRADE_SLIP 5
#define ACC_TRADE_PRICE 6


#define ACC_HEADER_MARKET 'M'
#define ACC_HEADER_POSITION 'P'
#define ACC_HEADER_TRADE 'T'
#define ACC_HEADER_CONSOLE 'C'
#define ACC_HEADER_BROKER 'B'
#define ACC_HEADER_SYMBOL 'S'
#define ACC_HEADER_FINISH 'F'


#define ACC_SIDE_BUY 'B'
#define ACC_SIDE_SELL 'S'


#define ACC_AMOUNT_LEN 10
#define ACC_TIMESTAMP_LEN 26
#define ACC_LETTER 'a'
#define ACC_MSG_BEGIN '['
#define ACC_MSG_END ']'
#define ACC_MSG_SEPARATOR '^'


#define DEF_ACC_BROKER_LIMIT 8
#define DEF_ACC_SYMBOL_LIMIT 32
#define DEF_ACC_MARKET_LIMIT 10
#define DEF_ACC_ERROR_LIMIT 3

// simple statuses - brokers and symbols

// active - ready to play
#define ACC_STATUS_ACTIVE 'A'

// errors occured - stop playing
#define ACC_STATUS_ERROR 'E'

// none active - dont play at all
#define ACC_STATUS_NONE 'N'



// None of position
#define ACC_POS_STATUS_NONE 'N'

// Filled to open
#define ACC_POS_STATUS_FILLED 'F'

// Sent to open
#define ACC_POS_STATUS_SENT 'S'

// Opened for business
#define ACC_POS_STATUS_OPENED 'O'

// filled to Close
#define ACC_POS_STATUS_CLOSE 'C'

// filled to Close by leave. Try to close on profit>commision . if not then close after 10 seconds anyway till success 
#define ACC_POS_STATUS_LEAVE 'L'

// Wait for close
#define ACC_POS_STATUS_WAIT 'W'

// Done. closed and profit calculated. Print fin stats and change to None
#define ACC_POS_STATUS_DONE 'D'

// Error. Trade operation couldn't be done
#define ACC_POS_STATUS_ERROR 'E'



struct ACC_MARKET {
	//      char broker;            // broker - single char
	//      char symbol;            // symbol
	double bid[DEF_ACC_MARKET_LIMIT];             // bid value
	double ask[DEF_ACC_MARKET_LIMIT];             // ask value
	char active;
	char timestamp[30];
	time_t time;
	unsigned long same_tick;        // how many ticks bid and ask are the same
};

struct ACC_GIRL {
	unsigned long long girl_tick;
	char status;
	int frequency;					// frequency of girl in hertz


};

struct ACC_POSITION {
	//      unsigned char id;
	//      unsigned long deal;
	//      char symbol;
	unsigned long long seq;			// tick sequence
	unsigned long long close_seq;			// tick sequence
	char  side;                      // B-uy S-ell C-close
	char  size;                     // multiply by standard like 0.1*1
	char  slip;                     //
	double price_open;                     // desired open price
	double price_close;                    // desired close price
	double opened;                // real opened price
	double closed;                // real close price - not used
	double profit;            // current real profit for position
	double commission;      // real commission
	double swap;                            // real swap
	char status;            // F-illed (for pickup) S-ent (to broker) O-pened (confirmed by broker) C-closed (on profit)
	char reason[10];
	double point_value;
};

struct ACC_BROKER {
	char name[10];
	double account_rate;
	SOCKET client_socket = INVALID_SOCKET;
	double balance;
	double equity;
	double result;
	char status;
	unsigned long long broker_tick;
	char* output;
	double commission[DEF_ACC_SYMBOL_LIMIT];
	int rating;		// point value from -256 to +256. It describe average slippage(negative/positive), successful rate, reliability of broker, contract advantages, guaranted stopouts, rebates etc.
	int frequency;	// frequency of broker in hertz
	long time_ms;	// 
	long process_ms;
	long recv_ms;
	int errors;		// number of pos errors
};

struct ACC_SYMBOL {
	char name[10];
	double point;			// single point value in deposit currency
	int digits;				// how many digits after dot 
	int p_open;				// what is an expected profit for open in points
	int p_close;			// what is an expected profit for close in points

};

struct ACC_DEAL {
	unsigned long long seq;
	char sell_broker;
	char buy_broker;
	double sell_profit;
	double sell_commission;
	double buy_profit;
	double buy_commission;
	double result;
	char status;
};

extern unsigned char ACC_BROKER_LIMIT;
extern unsigned char ACC_SYMBOL_LIMIT;
extern unsigned long AccDigits[10];
extern unsigned char ACC_MODE;
extern char test_open, test_close;
extern unsigned long long tick_seq;

extern struct ACC_MARKET MarketBoard[DEF_ACC_BROKER_LIMIT][DEF_ACC_SYMBOL_LIMIT];
//static structure. Only one opened position per broker per symbol
extern struct ACC_POSITION PositionBoard[DEF_ACC_BROKER_LIMIT][DEF_ACC_SYMBOL_LIMIT];
extern struct ACC_BROKER BrokerBoard[DEF_ACC_BROKER_LIMIT];
extern struct ACC_SYMBOL SymbolBoard[DEF_ACC_SYMBOL_LIMIT];
extern struct ACC_DEAL   DealBoard[DEF_ACC_SYMBOL_LIMIT];
extern struct ACC_GIRL   GirlBoard;

extern FILE *acc_file_log;
extern FILE *acc_file_msg;
extern FILE *acc_file_pos;
extern FILE *acc_file_deal;
extern FILE *acc_server_cfg;			// symbols
extern char *acc_path;

#endif // HEADER_H_  
