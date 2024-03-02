#include "network/socket_utils.h"

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