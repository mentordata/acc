#pragma once
#ifndef ACC_FUNCTIONS_H_
#define ACC_FUNCTIONS_H_
#include "acc.h"
#include "lola.h"




double get_value(char* _str, int _offset);
int update_market(char* _msg, char _broker);
int update_position(char* _msg, char _broker);
char* command(char* _cmd, char* _ret);
int get_msg(char* _msg, char (*_mtab)[100][1000]);
char get_sell_broker(char _symbol);
char get_buy_broker(char _symbol);
int position_show(char _broker, char _symbol);
int deal_show(char _symbol);
int push_tick(double _value, double *_tab);
int load_config();
char* timestamp(char*);

#endif