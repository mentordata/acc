//#pragma once
#ifndef _LOLA_H_     // equivalently, #if !defined HEADER_H_  
#define _LOLA_H_  
#include "acc.h"

int lola_open(char _symbol);
char lola_hair(char _symbol);
int lola_trade(char *_msg, char _broker);

#endif
