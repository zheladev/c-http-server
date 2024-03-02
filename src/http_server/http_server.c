#include "http_server/http_server.h"

int initialize_server(char *port_str, int *sockfd) {
    struct addrinfo hints, *res;
    int status;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((getaddrinfo(NULL, port_str, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 1;
    }

    *sockfd = init_socket(hints, port_str);
    
    return 0;
}

void start_server(int *sockfd) {
    // server start logic, listen for incoming connections, and handle requests
}

void shutdown_server(int *sockfd) {
    // server shutdown logic, close sockets, and release resources
}

void handle_request(int client_socket) {
    // handle HTTP requests from clients
    // Parse incoming request, etc
}
