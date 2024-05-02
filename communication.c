//
// Created by Đức Bùi on 1/5/24.
//

#include "communication.h"

char* get_imap_tag(){
    return "A121";
}
/**
 * Return connection descriptor
 * @param emailServer
 * @param port
 * @return -1 if the connection failed, otherwise connection descriptor
 */
int create_connection(char* emailServer, char* port, struct addrinfo** res){
    // Create a socket
    int connfd = 0, re = 1, s, n;
    struct addrinfo hints, *rp;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_STREAM;
    s = getaddrinfo(emailServer, port, &hints, res);

    if (0 != s){
        handle_error(E_CONN_INIT);
        return -1;
    }

    // Search for a socket
    int canConnect = 0;
    for (rp = *res; rp != NULL; (rp = rp->ai_next)){
        connfd = socket(rp->ai_family,
                        rp->ai_socktype,
                        rp->ai_protocol);
        if (connfd == -1)
            continue;
        if (connect(connfd,
                    rp->ai_addr,
                    rp->ai_addrlen) != -1) {
            canConnect++;
            break;
        }
        close(connfd);
    }

    // Cannot connect to server
    if (0 == canConnect) return -1;

    // Free pointers
    freeaddrinfo(rp);

    return connfd;
}

void handle_error(int errCode) {
    fprintf(stderr, "SOMETHING WENT WRONG");
    exit(errCode);
}

void check_response(int connfd) {
    char *recvBuff = calloc(2048, sizeof(char));
    ssize_t remainingBuffer = 2048;

    ssize_t byteReceived = recv(connfd, recvBuff, remainingBuffer, 0);
    if (byteReceived > 0) {
        char* token = strtok(recvBuff, " ");
        if (NULL != token){
            token = strtok(NULL, " ");
            if (0 == strcmp(token, "OK")) {
                printf("OK");
                return;
            }
        } else {
            fprintf(stderr, "AGHHHHH");
        }
    }
}