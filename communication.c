//
// Created by Đức Bùi on 1/5/24.
//

#include "communication.h"

char* get_imap_tag(){
    return "COMMAND5MAY2024AASDNWHEBR";
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
        hints.ai_family = AF_INET;
        s = getaddrinfo(emailServer, port, &hints, res);
        if (0 != s) handle_error(E_CONN_INIT);
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


    return connfd;
}

void handle_error(int errCode) {
    fprintf(stderr, "SOMETHING WENT WRONG\n");
    
}

void check_response(int connfd) {
    char *recvBuff = calloc(2048, sizeof(char));
    ssize_t remainingBuffer = 2048;

    ssize_t byteReceived = recv(connfd, recvBuff, remainingBuffer, 0);
    if (byteReceived > 0) {
        char* token = strtok(recvBuff, " ");
        if (NULL != token){
            token = strtok(NULL, " ");
            if (0 == strcmp(token, IMAP_OK)) {
                fprintf(stderr,"Connection OK\n");
            }
        } else {
            fprintf(stderr, "Connection Broken\n");
        }
        free(recvBuff);
        return;
    }
    free(recvBuff);
    exit(E_SERVER_RESPONSE);
}

string *recv_from_server(int connfd, char* tag){
    char* recvBuff = calloc(BUFFER_DEFAULT_SIZE + 2, sizeof(char));
    string *result = create_string(BUFFER_DEFAULT_SIZE + 1);
    do{
        ssize_t receivedBytes = recv(connfd, recvBuff, BUFFER_DEFAULT_SIZE, 0);
        if (receivedBytes <= 0){
            perror("Error occurred while reading from server");
            free(recvBuff);
            return NULL;
        }
        recvBuff[receivedBytes] = 0;
        result = append(result, recvBuff);
    } while (strstr(recvBuff, tag) == NULL);
    return result;
}

ssize_t send_to_server(int connfd, char* command, size_t nbytes){
    ssize_t byteSent = write(connfd, command, nbytes);
    if (byteSent < 0) {
        fprintf(stderr, "Error sending command to server\n");
        return -1;
    }

    if (byteSent == 0){
        fprintf(stderr, "The server is offline\n");
        return -1;
    }

    return byteSent;
}

int login_to_server(int connfd, char *username, char *password) {
    char* command = NULL;
    char* tag = get_imap_tag();
    (void) asprintf(&command, "%s LOGIN %s %s\r\n", tag, username, password);
    ssize_t byteSent = send_to_server(connfd, command, get_strlen(command));
    if (byteSent > 0){
        string* response = recv_from_server(connfd, tag);
        if (response != NULL){
            if (strstr(response->str, IMAP_OK) != NULL){
                return 1;
            }
            if (strstr(response->str, IMAP_NO) != NULL){
                fprintf(stderr, "Wrong username or password\n");
            }
        }
        free_string(response);
    }
    free(command);
    return -1;
}
int select_folder(int connfd, const char *folderDirectory) {
    char* command = NULL;
    char* tag = get_imap_tag();
    if (folderDirectory == NULL){
        folderDirectory = "INBOX";
    }
    (void) asprintf(&command, "%s SELECT %s\r\n", tag, folderDirectory);
    ssize_t byteSent = send_to_server(connfd, command, get_strlen(command));
    if (byteSent <= 0) return -1;
    string* response = recv_from_server(connfd, tag);

    if (strstr(response->str, IMAP_OK)){
        free(response);
        return 1;
    }
    if (strstr(response->str, IMAP_NO)) {
        fprintf(stderr, "The mailbox doesn't exist\n");
    }
    if (strstr(response->str, IMAP_BAD)){
        fprintf(stderr, "Unknown args");
    }

    free(response);
    return -1;
}
string parse_response(string s, char* tag){
    size_t i = 0;
    char* lastLine = strstr(s.str, tag);
    if (lastLine == NULL){
        fprintf(stderr, "Error parsing message");
        exit(E_PARSE);
    }
    s.str[lastLine-s.str - 1] = '\0';
    s.len = strlen(s.str);
    for (i = 0; i < s.len && s.str[i] != '\n'; i++);
    s.str += (i+1);
    s.len = strlen(s.str);
    return s;
}
int retrieve_email(int connfd, int num_message){
    char* command = NULL;
    char* tag = get_imap_tag();
    asprintf(&command, "%s FETCH %d BODY.PEEK[]\r\n", tag, num_message);
    if(send_to_server(connfd, command, get_strlen(command)) <= 0){
        free(command);
        return -1;
    }
    string* buff = recv_from_server(connfd, tag);
    if (strstr(buff->str, IMAP_OK) != NULL){
        string content = parse_response(*buff, tag);
        printf("%s", content.str);
        return 1;
    }
    free_string(buff);
    return -1;
}
