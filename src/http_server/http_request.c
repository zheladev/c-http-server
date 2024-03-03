#include "http_server/http_request.h"

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