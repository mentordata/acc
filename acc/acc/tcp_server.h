#pragma once
#ifndef TCP_SERVER_H_
#define TCP_SERVER_H_

SOCKET tcp_server_init(int _port);
//int tcp_server_listen(SOCKET ClientSocket);
int tcp_server_close(SOCKET ClientSocket);

#define TCP_SERVER_BUFLEN 4096
#define TCP_SERVER_CONSOLE 27099
#define TCP_SERVER_PORT 27000


#endif