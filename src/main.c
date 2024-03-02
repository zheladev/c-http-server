#include "http_server/http_server.h"

int main(int argc, char *argv[]) {
    struct addrinfo hints;
    int sockfd;
    char *port_str = "8080";

    if (argc > 1) {
        port_str = argv[1];
    }

    memset(&hints, 0, sizeof hints);

    if (initialize_server(port_str, &sockfd, &hints) == 1) {
        return 1;
    }

    start_server(&sockfd);
    shutdown_server(&sockfd);
    return 0;
}