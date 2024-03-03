#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include "constants.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    HTTP_0_9,
    HTTP_1_0,
    HTTP_1_1,
    HTTP_2_0,
    HTTP_3_0,
    HTTP_VERSION_UNKNOWN = -1
} HttpVersion;

typedef enum {
    HTTP_GET,
    HTTP_HEAD,
    HTTP_POST,
    HTTP_PUT,
    HTTP_DELETE,
    HTTP_CONNECT,
    HTTP_OPTIONS,
    HTTP_TRACE,
    HTTP_PATCH,
    HTTP_VERB_UNKNOWN = -1
} HttpMethod;

typedef struct {
    char *name;
    char *value;
} HttpHeader;

typedef struct {
    HttpMethod http_method;
    HttpVersion http_version;
    HttpHeader **http_headers;
    size_t num_http_headers;
    char *resource;
    char *body;
} HttpRequest;

void malloc_http_request(HttpRequest **request);
void add_request_header(HttpRequest *request, char *header);
void add_request_resource(HttpRequest *request, char *resource);
void add_request_body(HttpRequest *request, char *body);
void free_http_request(HttpRequest *request);
HttpMethod get_http_method_from_str(const char *str);
HttpVersion get_http_version_from_str(char *str);
int extract_http_ver_res_verb(HttpRequest *request_buffer, char *req_header);
int recv_http_request(int fd, HttpRequest *http_request_buffer);

#endif /* HTTP_REQUEST_H */