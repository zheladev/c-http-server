#ifndef SOCKET_UTILS_H
#define SOCKET_UTILS_H

#include "constants.h"
#include <arpa/inet.h>
#include <bits/types/struct_timeval.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

int init_socket(struct addrinfo *addr_info, char *port_str);
int bind_socket(int sockfd, struct addrinfo *addr_info);
int shutdown_socket(int sockfd, int how);
int listen_on_socket(int sockfd, int backlog);

#endif