#include "http_server/http_request.h"

void malloc_http_request(HttpRequest **request) {
    *request = malloc(sizeof(HttpRequest));
    (*request)->http_headers = NULL;
    (*request)->num_http_headers = 0;
}

void single_split(const char *delimiter, const char *source, char **firstPart,
                  char **secondPart) {
    char *delimiterPosition = strchr(source, *delimiter);

    if (delimiterPosition != NULL) {
        size_t firstPartLength = delimiterPosition - source;
        *firstPart = malloc(firstPartLength + 1);
        strncpy(*firstPart, source, firstPartLength);
        (*firstPart)[firstPartLength] = '\0';
        while (isspace(*delimiterPosition++))
            ;
        *secondPart = strdup(delimiterPosition + 1);
    } else {
        *firstPart = strdup(source);
        *secondPart = NULL;
    }
}

void add_request_header(HttpRequest *request, char *header) {
    char *name = NULL, *value = NULL;
    single_split(": ", header, &name, &value);

    request->http_headers = realloc(
        request->http_headers, (request->num_http_headers + 1) * sizeof(HttpHeader *));

    request->http_headers[request->num_http_headers] = malloc(sizeof(HttpHeader));
    request->http_headers[request->num_http_headers]->name = strdup(name);
    request->http_headers[request->num_http_headers]->value = strdup(value);

    request->num_http_headers++;
}

void add_request_resource(HttpRequest *request, char *resource) {
    request->resource = strdup(resource);
}

void add_request_body(HttpRequest *request, char *body) {
    request->body = strdup(body);
}

void free_http_request(HttpRequest *request) {
    for (size_t i = 0; i < request->num_http_headers; ++i) {
        free(request->http_headers[i]->name);
        free(request->http_headers[i]->value);
        free(request->http_headers[i]);
    }

    free(request->http_headers);
}

HttpMethod get_http_method_from_str(const char *str) {
    if (strcmp(str, "GET") == 0) {
        return HTTP_GET;
    } else if (strcmp(str, "POST") == 0) {
        return HTTP_POST;
    } else if (strcmp(str, "PUT") == 0) {
        return HTTP_PUT;
    } else if (strcmp(str, "DELETE") == 0) {
        return HTTP_DELETE;
    } else if (strcmp(str, "CONNECT") == 0) {
        return HTTP_CONNECT;
    } else if (strcmp(str, "OPTIONS") == 0) {
        return HTTP_OPTIONS;
    } else if (strcmp(str, "TRACE") == 0) {
        return HTTP_TRACE;
    } else if (strcmp(str, "PATCH") == 0) {
        return HTTP_PATCH;
    } else {
        return HTTP_VERB_UNKNOWN;
    }
}

HttpVersion get_http_version_from_str(char *str) {
    if (strcmp(str, "HTTP/1.0") == 0) {
        return HTTP_1_0;
    } else if (strcmp(str, "HTTP/1.1") == 0) {
        return HTTP_1_1;
    } else if (strcmp(str, "HTTP/2.0") == 0) {
        return HTTP_2_0;
    } else if (strcmp(str, "HTTP/3.0") == 0) {
        return HTTP_3_0;
    } else {
        return HTTP_VERSION_UNKNOWN;
    }
}

int extract_http_ver_res_verb(HttpRequest *request_buffer, char *req_header) {
    char method[MAX_HTTP_VERB_SIZE], resource[MAX_HTTP_RESOURCE_SIZE],
        version[MAX_HTTP_VERSION_SIZE];
    if (sscanf(req_header, "%s %s %s", method, resource, version) != 3) {
        perror("sscanf extract ver, res, verb");
        return 1;
    }

    request_buffer->http_version = get_http_version_from_str(version);
    request_buffer->resource = strdup(resource);
    request_buffer->http_method = get_http_method_from_str(method);

    return 0;
}

int recv_http_request(int fd, HttpRequest *http_request_buffer) {
    int size_recv, total_size = 0;
    int buffer_size = MAX_HEADER_SIZE;
    int header_fully_parsed = 0;
    int body_length = 0;
    char *buffer;

    buffer = malloc(MAX_HEADER_SIZE);
    // Allocate memory for the buffer
    if (http_request_buffer == NULL) {
        perror("Error allocating memory.");
        return -1;
    }

    while (1) {
        size_recv = recv(fd, buffer, MAX_HEADER_SIZE, 0);
        if (size_recv == -1) {
            perror("Error receiving data.");
            free(buffer);
            return -1;
        }

        if (size_recv == 0) {
            // HTTP0.9 requests are terminated by closing the connection.
            printf("Connection closed by the peer.\n");
            break;
        }
        // if copying recv data would overflow buffer, increase allocated memory
        // before memcpy
        if (total_size + size_recv > buffer_size) {
            buffer_size *= 2;
            char *temp_buffer = realloc(buffer, buffer_size);
            if (temp_buffer == NULL) {
                perror("Error reallocating memory.");
                free(temp_buffer);
                free(buffer);
                return -1;
            }
            buffer = temp_buffer;
        }
        memcpy(buffer + total_size, buffer, size_recv);

        total_size += size_recv;

        // HTTP1.0/1.1 requests are terminated by 0x0D 0x0A 0x0D 0x0A byte
        // sequence.
        if (!header_fully_parsed) {
            char *header_end = strstr(buffer, "\r\n\r\n");
            if (header_end != NULL) {
                char *buffer_copy = strdup(buffer);
                header_fully_parsed = 1;
                char *token = strtok(buffer_copy, "\n");

                // extract method, resource, version
                if (extract_http_ver_res_verb(http_request_buffer, token) != 0) {
                    return -1;
                }

                // extract other headers
                while ((token = strtok(NULL, "\n")) != NULL &&
                       strcmp(token, "\r") != 0) {

                    add_request_header(http_request_buffer, token);

                    char *header_name =
                        http_request_buffer
                            ->http_headers[http_request_buffer->num_http_headers - 1]
                            ->name;

                    if (strcmp(header_name, "Content-Length") == 0) {
                        body_length = atoi(
                            http_request_buffer
                                ->http_headers[http_request_buffer->num_http_headers -
                                               1]
                                ->value);
                    }
                }
                // TODO: Retrieve body if it exists
                break;
            }
        }
    }

    return total_size;
}
