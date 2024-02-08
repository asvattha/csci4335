#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_PORT 12345
#define BUFFER_SIZE 1024

int main() {
    int server_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_len = sizeof(client_address);
    char buffer[BUFFER_SIZE];

    // Create UDP socket
    if ((server_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Prepare the server address structure
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(SERVER_PORT);

    // Bind the socket to the server address
    if (bind(server_socket, (const struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening...\n");

    while (1) {
        // Receive data from client
        int bytes_received = recvfrom(server_socket, (char *)buffer, BUFFER_SIZE, 0,
                                      (struct sockaddr *)&client_address, &client_address_len);
        buffer[bytes_received] = '\0';

        // Display received data
        printf("Received message from %s:%d: %s\n", inet_ntoa(client_address.sin_addr),
               ntohs(client_address.sin_port), buffer);

        // Echo back the received data to the client
        sendto(server_socket, (const char *)buffer, strlen(buffer), 0,
               (const struct sockaddr *)&client_address, client_address_len);
    }

    close(server_socket);
    return 0;
}

