//
// Created by Đức Bùi on 29/4/24.
//
#define _POSIX_C_SOURCE 200112L
#include "communication.h"
#include "email_handle.h"
#include "utils.h"
#include <errno.h>
#include <getopt.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <regex.h>

#define PORT "143"

extern int asprintf(char**, const char*, ...);
char* strdup(char*);

extern int errno;

int hasAnomolies(char* field){
    char illegalChars[] = {'\r', '\n', '\"', '\'', '\\'};

    for (int i = 0; i < sizeof illegalChars; i++){
        if (strchr(field, illegalChars[i])) return 1;
    }
    return 0;
}



void validate_args(char* username, char* password, char* command, char* server_name, long long messageNum, int msgNumSpecified){
    int isArgsCorrect = 1;
    if (NULL == username) {
        isArgsCorrect = 0;
        fprintf(stderr, "No username provided\n");
    }
    if (NULL == password) {
        isArgsCorrect = 0;
        fprintf(stderr, "No password provided\n");
    }
    if (NULL == command) {
        isArgsCorrect = 0;
        fprintf(stderr, "No command provided\n");
    }
    if (NULL == server_name){
        isArgsCorrect = 0;
        fprintf(stderr, "No server name provided\n");
    }
    if ((messageNum >= __INT32_MAX__ || messageNum <= 0) && messageNum != -1) {
        fprintf(stderr, "Invalid sequence number\n");
        exit(E_INVALID_ARGS);
    }
    if (!isArgsCorrect) exit(E_INVALID_ARGS);

    // Check for anomolies in username and password
    if (hasAnomolies(username) || hasAnomolies(password)) {
        fprintf(stderr, "There are anomolies either username or password\n");
        exit(E_INVALID_ARGS);
    }
}


void freePtrs(char* username, char* password, char* dir, char* command, char* server_name, struct addrinfo* addrSocket){
    if (username != NULL) free(username);
    if (password != NULL) free(password);
    if (dir != NULL) free(dir);
    if (command != NULL) free(command);
    if (server_name != NULL) free(server_name);
    if (addrSocket != NULL)
        freeaddrinfo(addrSocket);
}


int main(int argc, char *argv[]) {
    int opt, isMsgNumSpecified = 0;
    long long messageNum = -1;
    char *username=NULL, *password = NULL, *dir = NULL, *command = NULL, *server_name = NULL;
    struct addrinfo *addrSocket = NULL;
    // Arguments for the program:
    // fetchmail
    //        -u <username> -p <password> [-f <folder>] [-n <messageNum>]
    //        [-t] <command> <server_name>

    // Parse arguments for the program
    // Referenced from: https://stackoverflow.com/a/18079491

    while ((opt = getopt(argc, argv, "u:p:f:n:t")) != -1) {
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
            if (strlen(optarg) > 0 && optarg[0] == '*') messageNum = -1;
            else {
                for (int i = 0; i < strlen(optarg); i++)
                    if (!isdigit(optarg[i])) {
                        fprintf(stderr, "Invalid sequence number\n");
                        exit(E_INVALID_ARGS);
                    }

                    else messageNum = strtoll(optarg, NULL, 10);
            }
            break;
            case 't':
                return 0;
            default:
                fprintf(stderr, "%s", "Unknown arguments!\n");
                return E_INVALID_ARGS;
        }
    }
    for (int idx = optind; idx < argc; idx++){
        if (command == NULL)  command = strdup(argv[idx]);
        else if (server_name == NULL) server_name = strdup(argv[idx]);
    }



    // Validate program arguments
    validate_args(username, password, command, server_name, messageNum, isMsgNumSpecified);

    // Create a socket

    int connfd = create_connection(server_name, PORT, &addrSocket);

    // Connection init unsuccessful or  If server doesn't return the correct greeting
    if (CREATE_CONNECTION_ERR == connfd || check_response(connfd) < 0) {
        freePtrs(username, password, dir, command, server_name, addrSocket);
        exit(E_CONN_INIT);
    }

    // If login fails
    if (0 > login_to_server(connfd, username, password)) {
        printf("Login failure\n");
//        freePtrs(username, password, dir, command, server_name, addrSocket);
        exit(E_SERVER_RESPONSE);
    }
    fprintf(stderr, "LOGIN OK\n");

    // Select folder directory failed
    if (0 > select_folder(connfd, dir)){
        printf("Folder not found\n");
        freePtrs(username, password, dir, command, server_name, addrSocket);
        exit(E_SERVER_RESPONSE);
    }
    fprintf(stderr, "SELECT OK\n");

    string* email = NULL;
    if (!strcmp(command, "retrieve")){
        email = retrieve_email(connfd, messageNum);
        if (email == NULL){
            printf("Message not found\n");
            exit(E_SERVER_RESPONSE);
        }
        printf("%s",email->str);
    } else if (!strcmp(command, "mime")) {
        email = retrieve_email(connfd, messageNum);
        string* mimeContent = get_mime_section(email->str);
        if (mimeContent == NULL){
            printf("MIME Parsing Error");
            return E_PARSE;
        } else {
            printf("%s", mimeContent->str);
            free_string(mimeContent);
        }
    } else if (!strcmp(command, "parse")){
        parse_header(connfd, messageNum);
    } else if (!strcmp(command, "list")) {
        list_email(connfd);
    }

    if (email != NULL) free_string(email);
    char *exit_command;
    asprintf(&exit_command, "%s LOGOUT\r\n", "A01");
    send_to_server(connfd, exit_command ,get_strlen((exit_command)));
    string* response = recv_from_server(connfd, "A01");
    if (strstr(response->str, "BYE") == NULL) {
        fprintf(stderr, "Error logout from server");
    }
    if (-1 == close(connfd)) {
        fprintf(stderr, "Error closing connection");
    }
    // Free pointers
    freePtrs(username, password, dir, command, server_name, addrSocket);
    return EXIT_SUCCESS;
}