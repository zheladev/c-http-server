#include "network/socket.h"

int init_socket(struct addrinfo hints, char *port_str) {

    struct addrinfo *res;
    int sockfd, status;

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((status = getaddrinfo(NULL, port_str, &hints, &res)) != 0) {
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

    return sockfd;
}

int bind_socket(int sockfd, struct addrinfo *addr_info) {
    int b;
    b = bind(sockfd, addr_info->ai_addr, addr_info->ai_addrlen);

    if (b == -1) {
        perror("bind");
        return 1;
    }

    return 0;
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