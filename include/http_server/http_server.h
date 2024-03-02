#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include "network/socket.h"
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include "constants.h"

int initialize_server(char *port_str, int *sockfd, struct addrinfo *hints);
void start_server(int *sockfd);
void shutdown_server(int *sockfd);
void handle_request(int client_socket);
#endif