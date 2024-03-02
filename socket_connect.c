#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define BACKLOG    10
#define CHUNK_SIZE 1024

int recv_whole_message(int fd, char **buffer) {
    int size_recv, total_size = 0;
    int buffer_size = CHUNK_SIZE;
    char chunk[CHUNK_SIZE];

    *buffer = calloc(1, buffer_size); // Allocate memory for the buffer

    while (1) {
        memset(chunk, 0, CHUNK_SIZE);
        if ((size_recv = recv(fd, chunk, CHUNK_SIZE, 0)) < 0) {
            break;
        } else {
            printf("Im here");
            printf("%s", chunk);
            if (total_size + size_recv > buffer_size) {
                buffer_size *= 2;
                printf("Im here");
                *buffer = realloc(*buffer, buffer_size); // Update the pointer to the buffer
                printf("Im here");
            }
            memcpy(*buffer + total_size, chunk, size_recv);
            printf("Im there");
            total_size += size_recv;
            printf("Im asdhere");
        }
    }

    return total_size;
}

int main(int argc, char *argv[]) {
    struct sockaddr_storage their_addr;
    int status, l, b;
    socklen_t addr_size;
    struct addrinfo hints, *res;
    char *port_str;
    int sockfd, new_fd;

    if (argc != 2) {
        fprintf(stderr, "usage: socket_connect port\n");
        return 1;
    }

    port_str = argv[1];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((getaddrinfo(NULL, port_str, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 1;
    }

    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    if (sockfd == -1) {
        perror("socket");
        return 1;
    }

    int yes = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == 1) {
        perror("setsockopt");
        return 1;
    }

    b = bind(sockfd, res->ai_addr, res->ai_addrlen);

    if (b == -1) {
        perror("bind");
        return 1;
    }

    l = listen(sockfd, BACKLOG);

    if (l == -1) {
        perror("listen");
        return 1;
    }

    addr_size = sizeof their_addr;
    while ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr,
                            &addr_size)) != 0) {
        char *outgoing_msg = "Stop pinging me.\n";

        char *incoming_msg;
        int len, bytes_sent;

        int msg_size = recv_whole_message(new_fd, &incoming_msg);

        printf("msg size: %d\n\n", msg_size);
        printf("Received: %s\n", incoming_msg);

        len = strlen(outgoing_msg);
        bytes_sent = send(new_fd, outgoing_msg, len, 0);

        if (bytes_sent == -1) {
            perror("send");
        }

        shutdown(new_fd, 2);
    }

    shutdown(sockfd, 2);
}