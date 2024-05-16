/**
 * Library contains API for communicating with IMAP server
*/
#define _POSIX_C_SOURCE 200112L
#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_
#include <errno.h>
#include <getopt.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "ERROR_CODE.h"
#include "IMAP_RESPONSE.h"
#include "dynamic_string.h"
#include "utils.h"

#define CREATE_CONNECTION_ERR (-1)
// Forward Declaration
struct addrinfo;
int asprintf(char **ret, const char *format, ...);

// ===== Base operation for communicating with the server ====
/**
 * @fn create_connection
 * Create connection and return file descriptor if success
 * @param emailServer: Server name
 * @param port: Server port
 * @param res: saving address info dest
 * @return -1 if the connection initialisation failed, otherwise connection file descriptor
*/
int create_connection(char* emailServer, char* port, struct addrinfo **res);
/**
 * @fn check_response
 * Check for server's greeting response
 * @param connfd: connection file descriptor
 * @return -1 if the server return unexpected greeting, 1 if the expected greeting returned
*/
int check_greeting(int connfd);
/**
 * @fn recv_from_server
 * Recieve message from server
 * @param connfd: connection file descriptor
 * @param tag: The tag was used for previous command
 * @return message recieved from server
*/
string *recv_from_server(int connfd, char* tag);
/**
 * @fn send_to_server
 * send command to server
 * @param connfd: connection file descriptor
 * @param command: The command that wished to be sent (SHOULD BE TERMINATED BY CRLF)
 * @param nbytes: Number of bytes to be sent (Usually the size of command without \0);
 * @return the number of bytes recieved from server, -1 indicates the server failed to send
*/
ssize_t send_to_server(int connfd, char* command, size_t nbytes);
/**
 * @fn get_imap_tag
 * generate and allocate tag to be used for IMAP command
 * @return the allocated tag
*/
char* get_imap_tag();

// ===== Routines for client =====
/**
 * @fn login_to_server
 * login to server using username and password
 * @param connfd
 * @param username
 * @param password
 * @return 1 if login successful, -1 otherwise
*/
int login_to_server(int connfd, char *username, char *password);
/**
 * @fn select_folder
 * Select folder after login
 * @param connfd
 * @param folderDirectory: if NULL, Inbox is automatically selected 
 * @return 1 if selection successful, -1 otherwise
*/
int select_folder(int connfd, const char *folderDirectory);
/**
 * @fn retrieve_email
 * Retrieve raw email from server
 * @param connfd
 * @param num_message: message number, if -1 get the most recent message number
*/
string* retrieve_email(int connfd, long long num_message);
#endif //_COMMUNICATION_H_
