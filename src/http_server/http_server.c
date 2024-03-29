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
    while ((incoming_sockfd =
                accept(*sockfd, (struct sockaddr *)&incoming_addr, &addr_size)) != -1) {
        handle_request(incoming_sockfd);
    }
}

void shutdown_server(int *sockfd) { shutdown_socket(*sockfd, SHUT_RDWR); }

void handle_request(int client_socket) {
    HttpRequest *incoming_request;
    char *outgoing_msg = "lol";
    int msg_size;

    malloc_http_request(&incoming_request);
    msg_size = recv_http_request(client_socket, incoming_request);

    if (msg_size < 0) {
        perror("Error receiving message.");
        return;
    }

    if (DEBUG) {
        printf("New request received.\n"
               "Version int: %d\n"
               "Method int: %d\n"
               "Resource: %s\n"
               "END\n",
               incoming_request->http_version, incoming_request->http_method,
               incoming_request->resource);
    }

    // TODO: parse message, check which resource it's trying to acces, etc

    // TODO: retrieve file, send response etc

    send_http_msg(client_socket, outgoing_msg);
    shutdown(client_socket, SHUT_RDWR);
}
