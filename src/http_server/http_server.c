#include "http_server/http_server.h"

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

    if ((*sockfd = init_socket(res, port_str)) == -1) {
        perror(("init_socket"));
        return 1;
    }

    if (bind_socket(*sockfd, res) == -1) {
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
                                     &addr_size)) != -1) {
        handle_request(incoming_sockfd);
    }
}

void shutdown_server(int *sockfd) {
    // server shutdown logic, close sockets, and release resources
}

void handle_request(int client_socket) {
    char *incoming_msg, *outgoing_msg = "lol";
    int msg_size;

    msg_size = recv_http_request(client_socket, &incoming_msg);

    if (msg_size == -1) {
        perror("Error receiving message.");
        return;
    }

    if (DEBUG)
        printf("%s", incoming_msg);

    // TODO: parse message, check which resource it's trying to acces, etc

    // TODO: retrieve file, send response etc

    send_msg(client_socket, outgoing_msg);
    shutdown(client_socket, SHUT_RDWR);
}
