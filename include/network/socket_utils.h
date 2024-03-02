#ifndef SOCKET_UTILS_H
#define SOCKET_UTILS_H

#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

int send_msg(int sockfd, char *msg);
int recv_entire_msg(int fd, char **buffer);

#endif