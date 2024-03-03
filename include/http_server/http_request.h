#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include "constants.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { HTTP_0_9, HTTP_1_0, HTTP_1_1 } HttpVersion;

typedef enum {
    HTTP_GET,
    HTTP_HEAD,
    HTTP_POST,
    HTTP_PUT,
    HTTP_DELETE,
    HTTP_CONNECT,
    HTTP_OPTIONS,
    HTTP_TRACE,
    HTTP_PATCH
} HttpMethod;

typedef struct {
    char *name;
    char *value;
} HttpHeader;

typedef struct {
    HttpMethod method;
    HttpVersion version;
    HttpHeader *headers;
    char *resource;
    char *body;
} HttpRequest;

int recv_http_request(int fd, char **buffer);

#endif