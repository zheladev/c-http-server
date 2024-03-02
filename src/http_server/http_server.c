#include "http_server/http_server.h"
#include "network/socket_utils.h"


int initialize_server(char *port_str, int *sockfd, struct addrinfo *hints) {
    struct addrinfo *res;
    int status;

    hints->ai_family = AF_UNSPEC;
    hints->ai_socktype = SOCK_STREAM;
    hints->ai_flags = AI_PASSIVE;

    if ((getaddrinfo(NULL, port_str, hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 1;
    }

    if ((*sockfd = init_socket(hints, port_str)) == -1) {
        perror(("init_socket"));
        return 1;
    }

    if (bind_socket(*sockfd, hints) == -1) {
        perror("bind");
        return 1;
    }
    return 0;
}

void start_server(int *sockfd) {
    int incoming_sockfd;
    struct sockaddr_storage incoming_addr;
    socklen_t addr_size;
    if ((listen_on_socket(*sockfd, LISTEN_BACKLOG)) == -1) {
        perror("listen on socket.");
        return;
    }

    addr_size = sizeof incoming_addr;
    while ((incoming_sockfd = accept(*sockfd, (struct sockaddr *)&incoming_addr,
                                     NULL)) != -1) {
        handle_request(incoming_sockfd);
    }
}

void shutdown_server(int *sockfd) {
    // server shutdown logic, close sockets, and release resources
}

void handle_request(int client_socket) {
    char *outgoing_msg = "lol";
    char *incoming_msg;
    
    int msg_size;
}
