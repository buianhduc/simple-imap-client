//
// Created by Đức Bùi on 29/4/24.
//
#define _POSIX_C_SOURCE 200112L
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <getopt.h>
#include <netdb.h>
#include "communication.h"

#define PORT "143"
#define PERROR(MSG) fprintf(stderr, MSG);

extern int asprintf(char**, const char*, ...);
int main(int argc, char *argv[]) {
    int opt, messageNum = -1, ind = 0;
    char *username, *password, *dir, *command = NULL, *server_name = NULL;
    // Arguments for the program:
    // fetchmail
    //        -u <username> -p <password> [-f <folder>] [-n <messageNum>]
    //        [-t] <command> <server_name>

    // Parse arguments for the program
    // Referenced from: https://stackoverflow.com/a/18079491
    while ((opt = getopt(argc, argv, "u:p:f:n:t:")) != -1) {
        switch (opt) {
        case 'u':
            username = malloc(strlen(optarg));
            strcpy(username, optarg);
            break;
        case 'p':
            password = malloc(strlen(optarg));
            strcpy(password, optarg);
            break;
        case 'f':
            dir = malloc(strlen(optarg));
            strcpy(dir, optarg);
            break;
        case 'n':
            messageNum = atoi(optarg);
            break;
        case 't':
            break;
        default:
            fprintf(stderr, "%s", "Unknown arguments!\n");
            return E_INVALID_ARGS;
        }
    }
    for (int idx = optind; idx < argc; idx++){
        if (command == NULL && argv[idx] != NULL){
            command = malloc(strlen(argv[idx]));
            strcpy(command, argv[idx]);
        } else if (server_name == NULL && argv[idx] != NULL){
            server_name = malloc(strlen(argv[idx]));
            strcpy(server_name, argv[idx]);
        }
    }


    // Validate program arguments
    if (command == NULL || server_name == NULL) exit(E_INVALID_ARGS);

    // Create a socket
    struct addrinfo *addrSocket = NULL;
    int connfd = create_connection(server_name, PORT, &addrSocket);

    // Connection successful
    if (-1 != connfd){
        check_response(connfd);
    }
    close(connfd);

    // Free pointers
    free(username);
    free(password);
    if (dir != NULL) free(dir);
    free(command);
    free(server_name);
    freeaddrinfo(addrSocket);
    return EXIT_SUCCESS;
}
