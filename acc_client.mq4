//+------------------------------------------------------------------+
//|                                                   acc_client.mq4 |
//|                                                       Jack Green |
//|                                                    apicarbon.com |
//+------------------------------------------------------------------+
#define ACC_VERSION "1.83"
#include <stderror.mqh> 
#include <stdlib.mqh> 
#property copyright "Jack Green"
#property link      "apicarbon.com"
#property version   ACC_VERSION
#property strict


//+------------------------------------------------------------------+
//| Expert initialization function                                   |
//+------------------------------------------------------------------+
#import "acc_client.dll"
int acc_client_init(string _host, int _port);
int acc_client_send(string _str, char& _buffer[]);
#import


string host="localhost";   // ACC system host                  
int    port=27000;         // ACC system port                  
int    id=0;               // Broker no.                       
int    timer=10;            // delay in seconds to start
#define DEF_ACC_SYMBOL_LIMIT 32
#define ACC_LETTER 'a'
#define ACC_ID 0
#define ACC_ID_DEAL 1
#define ACC_ID_POSITION 2
#define ACC_ID_BROKER 3
#define ACC_TRADE 4
#define ACC_ID_SYMBOL 5
#define ACC_SYMBOL 6
#define ACC_PRICE 7
#define ACC_SIZE 8
#define ACC_SLIPPAGE 9
#define ACC_PROFIT 10
#define ACC_STATUS 11
#define ACC_DTIME 12
#define ACC_CDTIME 13

#define ACC_STATS_A 1000
#define ACC_STATS_B 500// time in us 

#define ACC_TRADE_SELL 'S'
#define ACC_TRADE_BUY 'B'
#define ACC_TRADE_CLOSE 'C'
#define ACC_TRADE_LEAVE 'L'


unsigned long main_timer=0;       // every single milisecond
double account_rate=1;            // used to conversion profits, commisions and swaps into EUR
bool c_section=false;
string symbols[DEF_ACC_SYMBOL_LIMIT];
double symbols_commission[DEF_ACC_SYMBOL_LIMIT];
string broker_name;
string pos_tick;
unsigned char ACC_SYMBOL_LIMIT;  // current limit of symbols
// used in stats
unsigned long timer_tick;
long utc_shift=0;

bool is_end, started;

struct acc_pos
{
int ticket;
uchar side; // SELL, BUY
uchar status;
};


acc_pos position[DEF_ACC_SYMBOL_LIMIT];

int OnInit()
  {
//---
int _ret, _idx;
char _msg[1000];
int _cfg_file;
string _item, _value[], _broker_tick;
string _symbol_tmp[10];


_cfg_file=FileOpen("acc_client.cfg",FILE_READ|FILE_CSV);

if(_cfg_file==INVALID_HANDLE)
{
  _ret=GetLastError();
   Print("Problem with acc_client.cfg. Error:"+IntegerToString(_ret));
}
else
{
ACC_SYMBOL_LIMIT=0;
      while(!FileIsEnding(_cfg_file)) 
        { 
         //--- read the string 
         _item=StringTrimLeft(StringTrimRight(FileReadString(_cfg_file))); 
         
         _ret=StringSplit(_item,StringGetCharacter("=",0),_value);
         
         if(StringSplit(_item,StringGetCharacter("=",0),_value)==2)
            {
            if(_value[0]=="host")
               host=_value[1];

            if(_value[0]=="port")
               port= StrToInteger(_value[1]);
               
            if(_value[0]=="name")
               broker_name=_value[1];
               
            if(_value[0]=="id")
               id=(unsigned char)(StringToInteger(_value[1]));
               
            if(_value[0]=="timer")
               timer = StrToInteger(_value[1]);       // delay from start in seconds
               
            if(_value[0]=="utc_shift")
               utc_shift = StrToInteger(_value[1]);   // how many minutes utc_shift = trade_time-UTC_time 

           if(_value[0]=="account_rate")
               account_rate = StrToDouble(_value[1]);

            if(_value[0]=="symbol")
            {
               _ret=StringSplit(_value[1],StringGetCharacter(" ",0),_symbol_tmp);
               symbols[ACC_SYMBOL_LIMIT]=_symbol_tmp[0];
               symbols_commission[ACC_SYMBOL_LIMIT]=StringToDouble(_symbol_tmp[1]);
               ACC_SYMBOL_LIMIT++;
            };
               
            };         
         PrintFormat(_item); 
        } 
      //--- close the file 
      FileClose(_cfg_file); 
      Print("No Symbols: "+IntegerToString(ACC_SYMBOL_LIMIT));
      
};   


Print("acc_client v."+ACC_VERSION+" "+AccountInfoString(ACCOUNT_COMPANY)+" account currency "+ AccountInfoString(ACCOUNT_CURRENCY)+" host:"+AccountInfoString(ACCOUNT_SERVER)+" login: "+IntegerToString(AccountInfoInteger(ACCOUNT_LOGIN)));
//Print("acc_client.cfg broker="+broker_name+" host="+host+" port="+port+" timer="+timer+" id="+id);


_ret=TimeCurrent() - TimeGMT() - (utc_shift*60);

if(_ret<(-60))
   {
   Print("Trade Server time error. [TimeCurrent() - TimeGMT() - (utc_shift*60)<60]=",IntegerToString(_ret));
//   return(INIT_FAILED);
   };
   

_ret = acc_client_init(host,port);

if(_ret)
   {
   Print("ACC server connection error :",IntegerToString(_ret));
   return(INIT_FAILED);
   };

// B, broker, account_rate, name[20]
_broker_tick=StringFormat("[B%c%-10.6f%-10s",'a'+id,account_rate,broker_name);

for(_idx=0;_idx<ACC_SYMBOL_LIMIT && symbols[_idx]!="";_idx++)
{
_broker_tick=_broker_tick+StringFormat("%-10.6f",NormalizeDouble(symbols_commission[_idx]/account_rate,2));

};

_broker_tick = _broker_tick+"]";

if(!_ret)      // only if connected
   {
   _ret = acc_client_send(_broker_tick, _msg);

   started = EventSetTimer(1);

   is_end=false;

  };
  
   return(INIT_SUCCEEDED);
  
};
//+------------------------------------------------------------------+
//| Expert deinitialization function                                 |
//+------------------------------------------------------------------+

void OnTimer()
{

if(started)
   {
   started=false;
   while(!is_end)
   {
      SendTick(); 
   };

if(is_end)
   ExpertRemove();
  
};
 

};


void OnDeinit(const int reason)
  {
//---
string _broker_tick;
char _msg[100];

is_end = true;
_broker_tick=StringFormat("[F%c%-3d]",'a'+id,reason);

acc_client_send(_broker_tick, _msg);
   
  }
//+------------------------------------------------------------------+
//| Expert tick function                                             |
//+------------------------------------------------------------------+
void OnTick()
  {
//---

  }
  
void SendTick()
{
//static int id_position=-1;        // just single ID

long _timer=0;
char xreturn[];    // tutaj byl blad !!!! [] a nie [10] !!!
char _buffer[1000];
string _symbol;
char cpost[];
int _test; 
MqlDateTime cTimestamp; 
int _idx;
unsigned long _curr_time;// time to collect

string acc_msg;
string acc_tick;
long _diff_time;

acc_msg=StringFormat("[M%c",'a'+id);   // beginning

for(_idx=0;_idx<ACC_SYMBOL_LIMIT && symbols[_idx]!="";_idx++)
{
_symbol=symbols[_idx];


_diff_time= MarketInfo(_symbol,MODE_TIME) - TimeGMT() - (utc_shift*60);

if(_diff_time > (-120))
{
   acc_tick=StringFormat("%-10.6f%-10.6f",MarketInfo(_symbol,MODE_BID),MarketInfo(_symbol,MODE_ASK));
}
else
{
// if trade is not allowed then send zeros
   acc_tick=StringFormat("%-10.6f%-10.6f", 0.0, 0.0);
};

acc_msg=acc_msg+acc_tick;

};

acc_msg+= pos_tick;

pos_tick="";


acc_msg=acc_msg+"]";
 
if(!c_section){
c_section=true;

_test=acc_client_send(acc_msg, _buffer);

if(_buffer[1]=='T')    // trade
{
   _curr_time=GetMicrosecondCount(); // time to print

   executeTrade(CharArrayToString(_buffer));
   Print("Filled in "+IntegerToString(GetMicrosecondCount()-_curr_time)+" [us]");

};


if(_buffer[1]=='F')    // trade
{
   Print("STOP ");
   is_end=true;
   
};


}
else
{
Print("critical section conflict");

};
c_section=false;
   
};  
 

// if position is between F and D then report every state of position
// open the file with broker_broker_symbol_starttimestamp and write log for every position/broker update

// mql4 exec_trade
// trade with no price inside. Always with bid or ask
//[TabSbd^TagBbd^TacBbd^TahSbd]
// T:broker:symbol:Trade:size:slip
// T:broker:symbol:Trade
// [TabS^TagB^TacS]



int executeTrade(string _msg)
{
string _trades[DEF_ACC_SYMBOL_LIMIT];
uchar _trade[DEF_ACC_SYMBOL_LIMIT];
int _no_trades;
uchar _separator;
int _ticket;
string _symbol_name;
uchar _symbol;
uchar _size;

_separator='^';


_no_trades=StringSplit(StringSubstr(_msg,1,StringLen(_msg)-2),_separator, _trades);

while(_no_trades>0)
{
StringToCharArray(_trades[_no_trades-1],_trade); // convert to char[]

//if(_trade[1]!=(id+ACC_LETTER))
//    return -1; // inproper broker

_symbol_name=symbols[_trade[2]-ACC_LETTER];
_symbol= _trade[2]-(uchar)ACC_LETTER;
_size  = _trade[4]-(uchar)ACC_LETTER;


if(_trade[3]==ACC_TRADE_SELL)
    {
    _ticket = OrderSend(_symbol_name, OP_SELL, 0.1*_size, MarketInfo(_symbol_name, MODE_BID),0,0,0); // sell
    position[_symbol].ticket=_ticket;
    position[_symbol].side='S';

    if(_ticket==-1)
        {
        Print("Cannot sell :"+_trades[_no_trades-1]+" Error: "+ErrorDescription(GetLastError())); // sell
        position[_symbol].status='E'; // ERROR
        }
    else
        {
        position[_symbol].status='O'; // OPENED
        Print("SELL ["+IntegerToString(_ticket)+"] :"+_trades[_no_trades-1]);
        };
        pos_tick=pos_tick+"^"+pos_msg(position[_symbol].ticket,_symbol );
    };


if(_trade[3]==ACC_TRADE_BUY)
    {
    _ticket = OrderSend(_symbol_name, OP_BUY, 0.1*_size, MarketInfo(_symbol_name, MODE_ASK),0,0,0);
    position[_symbol].ticket=_ticket;
    position[_symbol].side='B';

    if(_ticket==-1)
        {
        Print("Cannot buy :"+_trades[_no_trades-1]+" Error: "+ErrorDescription(GetLastError())); // buy
        position[_symbol].status='E'; // ERROR
        }
    else
        {
        position[_symbol].status='O'; // OPENED
        Print("BUY ["+IntegerToString(_ticket)+"] :"+_trades[_no_trades-1]);
        };
        pos_tick=pos_tick+"^"+pos_msg(position[_symbol].ticket,_symbol );
    };


if (_trade[3]==ACC_TRADE_CLOSE  )
{
    if (!OrderClose(position[_symbol].ticket, 0.1*_size, MarketInfo(_symbol_name,((OrderType()==OP_SELL) ? MODE_ASK : MODE_BID)),0,clrBeige))
        {
        Print("Cannot close:["+IntegerToString(position[_symbol].ticket)+"] "+_trades[_no_trades-1]+" Error: "+ErrorDescription(GetLastError())); // buy
        position[_symbol].status='E'; // ERROR
        }
    else
        {
// report position
        position[_symbol].status='D'; // DONE
        Print("CLOSE ["+IntegerToString(position[_symbol].ticket)+"] :"+_trades[_no_trades-1]);
        };
        pos_tick=pos_tick+"^"+pos_msg(position[_symbol].ticket,_symbol );
};

if (_trade[3]==ACC_TRADE_LEAVE  )
{
    while(!OrderClose(position[_symbol].ticket, 0.1*_size, MarketInfo(_symbol_name,((OrderType()==OP_SELL) ? MODE_ASK : MODE_BID)),0,clrBeige))
        {
        Print("Cannot leave:["+IntegerToString(position[_symbol].ticket)+"] "+_trades[_no_trades-1]+" Error: "+ErrorDescription(GetLastError())); // buy
        position[_symbol].status='E'; // ERROR
        Sleep(50);
        };
       
// report position
        position[_symbol].status='D'; // DONE
        Print("LEAVE ["+IntegerToString(position[_symbol].ticket)+"] :"+_trades[_no_trades-1]);
        
        pos_tick=pos_tick+"^"+pos_msg(position[_symbol].ticket,_symbol );
};

_no_trades--;
};






return 0;
}

  
string pos_msg(int _ticket, uchar _id_symbol)
{
// function returns single position message for the ticket
string _msg;
uchar _status, _side;
double _commission, _profit, _swap, _open, _close, _balance, _equity, _point_value;
_status='S';      // SENT

if(! OrderSelect(_ticket, SELECT_BY_TICKET, MODE_TRADES))      // find opened
   if(! OrderSelect(_ticket,SELECT_BY_TICKET, MODE_HISTORY))   // if not then historical
      _status='E';                                             // if not then return error
      
  _commission=OrderCommission()/account_rate;       // if in PLN then divide by EURPLN
  _profit   =OrderProfit()/account_rate;
  _swap     =OrderSwap()/account_rate;
  _balance  = AccountBalance()/account_rate;
  _equity   = AccountEquity()/account_rate;
  _open     =OrderOpenPrice();
  _point_value = (MarketInfo(OrderSymbol(),MODE_TICKVALUE)/account_rate)/MarketInfo(OrderSymbol(),MODE_TICKSIZE);
  _close    =0;   //OrderClosePrice();
//  _symbol   =OrderSymbol();


if(OrderOpenTime()>0)
{
   _status='O';      // OPENED

};

if(OrderCloseTime()!=0)
{
   _close    =OrderClosePrice();
   _status='D';      // DONE
};   

if(OrderType()==OP_BUY)
   _side='B';
   
if(OrderType()==OP_SELL)
   _side='S';

_msg=StringFormat("P%c%c%c%-10.6f%-10.6f%-10.6f%-10.6f%-10.6f%c%-10.6f%-10.6f%-10.6f",'a'+id,_id_symbol+'a',_side,_open,_close,_profit,_commission,_swap,_status,_balance, _equity,_point_value);

return _msg;
};  
  
  
  
  
//+------------------------------------------------------------------+
//| Tester function                                                  |
//+------------------------------------------------------------------+
double OnTester()
  {
//---
   double ret=0.0;
//---

//---
   return(ret);
  }
//+------------------------------------------------------------------+
//| ChartEvent function                                              |
//+------------------------------------------------------------------+
void OnChartEvent(const int id_chart,
                  const long &lparam,
                  const double &dparam,
                  const string &sparam)
  {
//---
   
  }
//+------------------------------------------------------------------+
