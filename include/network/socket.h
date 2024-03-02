#ifndef SOCKET_UTILS_H
#define SOCKET_UTILS_H

#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

int init_socket(struct addrinfo *addr_info, char *port_str);
int bind_socket(int sockfd, struct addrinfo *addr_info);
int listen_on_socket(int sockfd, int backlog);

#endif
