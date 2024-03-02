#include "network/socket_utils.h"

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

int recv_entire_msg(int fd, char **buffer) {
    int size_recv, total_size = 0;
    int buffer_size = CHUNK_SIZE;
    char chunk[CHUNK_SIZE];

    // Allocate memory for the buffer
    *buffer = calloc(1, buffer_size);

    while ((size_recv = recv(fd, chunk, CHUNK_SIZE, MSG_PEEK)) > 0) {
        if (size_recv == -1) {
            perror("Error receiving msg.");
            return -1;
        }
        memset(chunk, 0, CHUNK_SIZE);
        recv(fd, chunk, CHUNK_SIZE, 0);

        if (total_size + size_recv > buffer_size) {
            buffer_size *= 2;
            // Update the pointer to the buffer
            *buffer = realloc(*buffer, buffer_size);
        }
        memcpy(*buffer + total_size, chunk, size_recv);

        total_size += size_recv;
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