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

int shutdown_socket(int sockfd, int how) {
    int s;
    s = shutdown(sockfd, how);

    return s;
}