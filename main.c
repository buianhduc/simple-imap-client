//
// Created by Đức Bùi on 29/4/24.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <getopt.h>
#include <netdb.h>

#define PORT "143"
#define ADDR "unimelb-comp30023-2024.cloud.edu.au"
int main(int argc, char *argv[]) {
    int opt, messageNum;
    char *username, *password, *dir, *command, *server_name;
    // Arguments for the program:
    // fetchmail
    //        -u <username> -p <password> [-f <folder>] [-n <messageNum>]
    //        [-t] <command> <server_name>

    // Parse arguments for the program
    // Referenced from: https://stackoverflow.com/a/18079491
    while (optind < argc){
        if ((opt = getopt(argc, argv, "u:p:f:n:t:")) != -1){
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
                return EXIT_FAILURE;
            }
        } else {
            if (argv[optind] != NULL && argv[optind+1] != NULL){
                command = malloc(strlen(argv[optind]));
                strcpy(command, argv[optind]);
                server_name = malloc(strlen(argv[optind+1]));
                strcpy(command, argv[optind+1]);
            }
            break;
        }
    }

    // Create a socket
    int connfd = 0, re = 1, s ,n;
    struct addrinfo hints, *res, *rp;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_STREAM;

    s = getaddrinfo(ADDR, PORT, &hints, &res);
    for (rp = res; rp != NULL; (rp = rp->ai_next)){
        connfd = socket(rp->ai_family,
                        rp->ai_socktype,
                        rp->ai_protocol);
        if (connfd == -1)
            continue;
        if (connect(connfd,
                    rp->ai_addr,
                    rp->ai_addrlen) != -1) break;
        close(connfd);
    }
    char* Acommand;
    asprintf(&Acommand, "a001 LOGIN %s %s\r\n", username, password);
    write(connfd, Acommand, strlen(Acommand));
        char *recvBuff = malloc(768);

        while (read(connfd, recvBuff, sizeof recvBuff) != 0){
            printf("%s\n", recvBuff);
        }
    close(connfd);

    // Free pointers
    free(username);
    free(password);
//    free(dir);
    free(command);
    free(server_name);
    return EXIT_SUCCESS;
}
