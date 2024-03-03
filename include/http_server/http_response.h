#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

int send_http_msg(int sockfd, char *msg);
#endif