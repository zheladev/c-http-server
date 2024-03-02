#include "network/socket_utils.h"
#include <stdio.h>

int init_socket(struct addrinfo *addr_info, char *port_str) {
    int sockfd;

    sockfd = socket(addr_info->ai_family, addr_info->ai_socktype,
                    addr_info->ai_protocol);
    int yes = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == 1) {
        return -1;
    }

    return sockfd;
}

int bind_socket(int sockfd, struct addrinfo *addr_info) {
    int b;
    b = bind(sockfd, addr_info->ai_addr, addr_info->ai_addrlen);

    return b;
}

int listen_on_socket(int sockfd, int backlog) {
    int l;
    l = listen(sockfd, backlog);

    if (l == -1) {
        perror("listen");
        return 1;
    }

    return 0;
}

int recv_http_request(int fd, char **buffer) {
    int size_recv, total_size = 0;
    int buffer_size = CHUNK_SIZE;
    char chunk[CHUNK_SIZE];

    // Allocate memory for the buffer
    *buffer = calloc(1, buffer_size);
    if (*buffer == NULL) {
        perror("Error allocating memory.");
        return -1;
    }

    while (1) {
        size_recv = recv(fd, chunk, CHUNK_SIZE, 0);

        if (size_recv == -1) {
            perror("Error receiving data.");
            free(*buffer);
            return -1;
        }

        if (size_recv == 0) {
            // HTTP0.9 requests are terminated by closing the connection.
            printf("Connection closed by the peer.\n");
            break;
        }

        if (total_size + size_recv > buffer_size) {
            buffer_size *= 2;
            char *temp_buffer = realloc(*buffer, buffer_size);
            if (temp_buffer == NULL) {
                perror("Error reallocating memory.");
                free(*buffer);
                return -1;
            }
            *buffer = temp_buffer;
        }
        memcpy(*buffer + total_size, chunk, size_recv);
        total_size += size_recv;

        // HTTP1.0/1.1 requests are terminated by 0x0D 0x0A 0x0D 0x0A byte
        // sequence.
        char *header_end = strstr(*buffer, "\r\n\r\n");
        if (header_end != NULL) {
            break;
        }
    }

    return total_size;
}
int send_msg(int sockfd, char *msg) {
    int len, bytes_sent = 0, bytes_sent_buffer;

    len = strlen(msg);

    while (bytes_sent < len || bytes_sent == -1) {
        bytes_sent_buffer = send(sockfd, msg, len, 0);
        if (bytes_sent_buffer == -1) {
            perror("Error sending msg.");
            return -1;
        }
        bytes_sent += bytes_sent_buffer;
    }

    return bytes_sent;
}