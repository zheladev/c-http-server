#include "http_server/http_response.h"

int send_http_msg(int sockfd, char *msg) {
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