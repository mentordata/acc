// acc_server.cpp: Okreœla punkt wejœcia dla aplikacji konsoli.
//

#include "stdafx.h"
#include "time.h"
#include "acc.h"
#include "acc_functions.h"
//#include <WinBase.h>
#include <Windows.h>
#include <process.h>
#include <conio.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include "tcp_server.h"

struct ACC_MARKET MarketBoard[DEF_ACC_BROKER_LIMIT][DEF_ACC_SYMBOL_LIMIT];

//static structure. Only one opened position per broker per symbol
struct ACC_POSITION PositionBoard[DEF_ACC_BROKER_LIMIT][DEF_ACC_SYMBOL_LIMIT];

struct ACC_BROKER BrokerBoard[DEF_ACC_BROKER_LIMIT];
struct ACC_SYMBOL SymbolBoard[DEF_ACC_SYMBOL_LIMIT];
struct ACC_DEAL   DealBoard[DEF_ACC_SYMBOL_LIMIT];
struct ACC_GIRL   GirlBoard;

unsigned long AccDigits[10] = { 1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000 };

unsigned char ACC_BROKER_LIMIT;
unsigned char ACC_SYMBOL_LIMIT;
unsigned char ACC_MODE;
unsigned char ACC_STATE;	// what is a state of server C-onnect, M-arket, T-rade, F-inish
FILE *acc_file_msg;			// incomming message log
FILE *acc_file_log;			// acc log
FILE *acc_file_pos;			// positions to backtesting
FILE *acc_file_deal;			// positions to backtesting
FILE *acc_server_cfg;			// symbols
char *acc_path;

unsigned long long tick_seq;
char threads_stop;
char test_open, test_close;

void broker_thread(void *_arg)
{
	char _broker;

	int _iresult, _msg_idx,_recvbuflen, _len_send,_idx2;
	char _recvbuf[10000];
	char _sendbuf[200];
	char(*_ptab)[100][1000];
	char _mtab[100][1000];
	char _info[200];
	unsigned long long begin_time, end_time, process_time, recv_time;
	double _tmp;



	
	_broker = (*(char*)_arg);

	_recvbuflen = 10000;
	_ptab = &_mtab;


	send(BrokerBoard[_broker].client_socket, "OK", 2, 0);		// Im ready
	BrokerBoard[_broker].status = ACC_STATUS_ACTIVE;			// active
	BrokerBoard[_broker].broker_tick = 0;
	sprintf(_info, "OK. id: %d - %s connected.", _broker, BrokerBoard[_broker].name);
	BrokerBoard[_broker].output=_info;
	
	begin_time = GetTickCount();

	// Activity to be timed


	do
	{
		_iresult = recv(BrokerBoard[_broker].client_socket, _recvbuf, _recvbuflen, 0);				// receive message
		if (_iresult > 0)		// received
		{
			tick_seq++;
			BrokerBoard[_broker].broker_tick++;
			_msg_idx = get_msg(_recvbuf, _ptab);
			_recvbuf[_iresult] = '\0';
			if (ACC_MODE == 'D')
				fprintf(acc_file_msg, "%-10u %s\n", tick_seq, _recvbuf);


			for (_idx2 = 0; _idx2 < _msg_idx; _idx2++)				// execute updates and positions
			{
				switch (_mtab[_idx2][ACC_HEADER])
				{
				case ACC_HEADER_MARKET: update_market(_mtab[_idx2], _broker); break;
				case ACC_HEADER_POSITION: update_position(_mtab[_idx2], _broker); break;
				case ACC_HEADER_FINISH:   threads_stop = 1; break;			// immediate stop
				default: break;
				}
			};

			_len_send = lola_trade(_sendbuf, _broker);		// prepare trades for client

			_iresult = send(BrokerBoard[_broker].client_socket, _sendbuf, _len_send, 0);					// send message

			if (_iresult == SOCKET_ERROR) {
				printf("send failed with error: %d\n", WSAGetLastError());
				closesocket(BrokerBoard[_broker].client_socket);
				WSACleanup();
				threads_stop = 1;
			};

			

		};	// received

		end_time = GetTickCount();


		BrokerBoard[_broker].frequency = (BrokerBoard[_broker].broker_tick * 1000) / (end_time - begin_time+1);

		Sleep(1);

	} while (!threads_stop && (BrokerBoard[_broker].status==ACC_STATUS_ACTIVE));	// error and NONE - switch it off

	send(BrokerBoard[_broker].client_socket, "[F]", 3, 0);					// send message STOP

	BrokerBoard[_broker].status = ACC_STATUS_NONE;

	return ;
};



void girl_thread(void *_arg)
{
	char _symbol;
	unsigned long long begin_time, end_time, process_time, recv_time;

	begin_time = GetTickCount();
	GirlBoard.girl_tick = 0;

	if (GirlBoard.status == ACC_STATUS_ACTIVE)
		return;	// is already opened

	GirlBoard.status = ACC_STATUS_ACTIVE;

	do
	{

		GirlBoard.girl_tick++;

		for (_symbol = 0; _symbol < ACC_SYMBOL_LIMIT; _symbol++) {

			lola_open(_symbol);
			lola_hair(_symbol);
		};

		Sleep(1);
		end_time = GetTickCount();

		GirlBoard.frequency = (GirlBoard.girl_tick * 1000) / (end_time - begin_time + 1);

	} while (!threads_stop && (GirlBoard.status == ACC_STATUS_ACTIVE));

	return;
};



void console_thread(void *_arg)
{
	char _sendbuf[100000];
	char _recvbuf[1000];
	char _tmpstr[1000];
	int _iresult, _idx;
	char _stop;

	SOCKET _socket;

	
	do
	{
	
		_stop = 0;

		_socket = tcp_server_init(TCP_SERVER_CONSOLE);		// connection
		command("help", _sendbuf);
		sprintf(_tmpstr, "acc [%llu] :", tick_seq);
		strcat(_sendbuf, _tmpstr);
		send(_socket, _sendbuf, strlen(_sendbuf), 0);
		do {
			_iresult = recv(_socket, _recvbuf, 1000, 0);	// wait for command
 			if (_iresult != SOCKET_ERROR)
			{
				if (isalpha(_recvbuf[0]))
				{ //do command
					for (_idx = 0; _recvbuf[_idx]>='!' && _recvbuf[_idx]<='}'; _idx++);

					_recvbuf[_idx] = '\0';
					command(_recvbuf, _sendbuf);
					sprintf(_tmpstr, "acc [%llu] :", tick_seq);
					strcat(_sendbuf, _tmpstr);
					send(_socket, _sendbuf, strlen(_sendbuf), 0);
				};
			}
			else
			{
				_stop = 1;
			};


		} while (!_stop);



	} while (!threads_stop);

	return;
};



int main(int argc, char *argv[])
{
	int _idx, _idx2;
	char _broker, _symbol;
	SOCKET _socket;
	char _stop;
	char _command[100];
	char _recvbuf[10000];
	char _broker_tick[1000];
	char _retbuff[100000];
	char _sendbuf[1000];
	unsigned long _tick_idx;
	char _mtab[100][1000];
	char (*_ptab)[100][1000];
	int _recvbuflen = 10000;
	int _iresult;
	int _msg_idx;
	int _len_send;
	char _sidx;
	char _str[10];
	int _p_open;
	int _p_close;
	int _digits;
	char _path[200];


	ACC_MODE = argv[1][0];						// D-ebug, R-untime, S-imulate, B-acktesting

	acc_path = argv[2];							// path

	timeBeginPeriod(1);
	Sleep(500);

	_tick_idx = 0;

	printf("ACC server v. %s \n", ACC_VERSION);
	printf("usage:\n acc.exe option config_path\n  options:\nD - debug \n");
	printf("S - simulation \n");
	printf("R - production run\n");
	printf("B - backtest \n");



	sprintf(_path,"%s%s",acc_path, "acc_server.pos.log");
	acc_file_pos = fopen(_path, "a");
	printf("acc_pos file: %s\n", (acc_file_pos) ? "OK" : "NG");

	sprintf(_path, "%s%s", acc_path, "acc_server.deal.log");
	acc_file_deal = fopen(_path, "a");
	printf("acc_deal file: %s\n", (acc_file_deal) ? "OK" : "NG");

	load_config();

	command("clear", _retbuff);

	fprintf(acc_file_pos, "seq,timestamp,sts,side,hair,broker,symbol,open,opened,close,closed,comm,profit,swap,result\n");
	fprintf(acc_file_deal, "seq,timestamp,sts,sell,buy,symbol,sell_profit,sell_comm,buy_profit,buy_comm,result\n");

	fclose(acc_file_pos);
	fclose(acc_file_deal);

	

	tick_seq = 0;
	_broker = 0;
	ACC_BROKER_LIMIT = 0;
	threads_stop = 0;
	_beginthread(console_thread, 0, NULL);

	Sleep(500);
	printf("\n==============================\n");


	do
	{
		printf("acc [%llu] :",tick_seq);
		scanf("%s",_command);


		if (!strcmp(_command, "connect") || !strcmp(_command, "c"))
		{
			if (_broker < DEF_ACC_BROKER_LIMIT)
			{
				_socket = tcp_server_init(TCP_SERVER_PORT);		// connection
				_iresult = recv(_socket, _broker_tick, 1000, 0);	// wait for broker tick
				if (_iresult > 0)
					if (_broker_tick[1] == ACC_HEADER_BROKER)
					{
															// _broker_tick[2] - ACC_LETTER;		// broker ID is identyfied by order of connections
						BrokerBoard[_broker].client_socket = _socket;
						BrokerBoard[_broker].account_rate = get_value(_broker_tick, 3);		// account rate
						strncpy(BrokerBoard[_broker].name, _broker_tick + 13, 10);
						for (_sidx = 0; ((_sidx * 10) + 3+10+10+1) < _iresult; _sidx++)
							BrokerBoard[_broker].commission[_sidx] = get_value(_broker_tick, 23 + (_sidx * 10));

						_beginthread(broker_thread, 0, &_broker);
						while (BrokerBoard[_broker].status != ACC_STATUS_ACTIVE)
							Sleep(10);													// wait for broker to prepare
						printf("%s\n", BrokerBoard[_broker].output);

						ACC_BROKER_LIMIT = _broker + 1;
						_broker++;
					};
			}
			else
			{
				printf("Broker limit achieved [%d]\n", DEF_ACC_BROKER_LIMIT);
			};
		};

		if (!strcmp(_command, "lola") || !strcmp(_command, "r"))
		{
			if (GirlBoard.status != ACC_STATUS_ACTIVE)
			{
				threads_stop = 0;
				_beginthread(girl_thread, 0, NULL);
				printf("Girl (LOLA) started.\n");
			}
			else
			{
				printf("Girl (LOLA) already started");
			};
		};


		if (!strcmp(_command, "stop") || !strcmp(_command, "s"))
		{
			printf(" wait till all brokers are down...");
			threads_stop = 1;

			for (_idx = 0; _idx < ACC_BROKER_LIMIT;)
			{
				if (BrokerBoard[_idx].status == ACC_POS_STATUS_DONE)
				{
					closesocket(BrokerBoard[_broker].client_socket);
					_idx++;
				};
			};
			printf(" Brokers' down. That's it\n");

		};


		printf("%s\n", command(_command, _retbuff));


	} while (!threads_stop);




		// CLOSE
		tcp_server_close(BrokerBoard[_idx].client_socket);
		printf("==================================================\n");
		printf("ACC server finished at tick_seq= %u\n", tick_seq);
		
		//		tcp_server_close(_console_socket);

    return 0;
}

