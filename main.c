//
// Created by Đức Bùi on 29/4/24.
//
#define _POSIX_C_SOURCE 200112L
#define _GNU_SOURCE
#include "communication.h"
#include "email_handle.h"
#include "utils.h"
#include <errno.h>
#include <getopt.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT "143"

extern int asprintf(char**, const char*, ...);

extern int errno;
int main(int argc, char *argv[]) {
    int opt, messageNum = -1;
    char *username=NULL, *password = NULL, *dir = NULL, *command = NULL, *server_name = NULL;
    // Arguments for the program:
    // fetchmail
    //        -u <username> -p <password> [-f <folder>] [-n <messageNum>]
    //        [-t] <command> <server_name>

    // Parse arguments for the program
    // Referenced from: https://stackoverflow.com/a/18079491
    while ((opt = getopt(argc, argv, "u:p:f:n:t:")) != -1) {
        switch (opt) {
        case 'u':
            username = malloc(strlen(optarg) + 1);
            strcpy(username, optarg);
            break;
        case 'p':
            password = malloc(strlen(optarg) + 1);
            strcpy(password, optarg);
            break;
        case 'f':
            dir = malloc(strlen(optarg) + 1);
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
            command = malloc(strlen(argv[idx]) + 1);
            strcpy(command, argv[idx]);
        } else if (server_name == NULL && argv[idx] != NULL){
            server_name = malloc(strlen(argv[idx]) + 1);
            strcpy(server_name, argv[idx]);
        }
    }


    // Validate program arguments
    if (NULL == command || NULL == server_name) exit(E_INVALID_ARGS);

    // Create a socket
    struct addrinfo *addrSocket = NULL;
    int connfd = create_connection(server_name, PORT, &addrSocket);

    // Connection successful
    if (-1 != connfd){
        check_response(connfd);
        if (1 == login_to_server(connfd, username, password)){
            fprintf(stderr, "LOGIN OK\n");
        } else {
            printf("Login failure\n");
            exit(E_SERVER_RESPONSE);
        }
        if (1 == select_folder(connfd, dir)){
            fprintf(stderr, "SELECT OK\n");
        } else {
            printf("Folder not found\n");
            exit(E_SERVER_RESPONSE);
        }

        if (retrieve_email(connfd, messageNum) == -1){
            printf("Message not found\n");
            exit(E_SERVER_RESPONSE);
        }
    }
    if (-1 == close(connfd)) {
        fprintf(stderr, "Error closing connection");
    }

    // Free pointers
    free(username);
    free(password);
    if (NULL != dir) free(dir);
    free(command);
    free(server_name);
    if (addrSocket != NULL) freeaddrinfo(addrSocket);
    return EXIT_SUCCESS;
}
