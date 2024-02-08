#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345
#define BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_address;
    char buffer[BUFFER_SIZE];

    // Create UDP socket
    if ((client_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Prepare the server address structure
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_address.sin_port = htons(SERVER_PORT);

    while (1) {
        // Get user input
        printf("Enter message to send: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        // Send message to the server
        sendto(client_socket, (const char *)buffer, strlen(buffer), 0,
               (const struct sockaddr *)&server_address, sizeof(server_address));

        // Receive the echoed message from the server
        int bytes_received = recvfrom(client_socket, (char *)buffer, BUFFER_SIZE, 0, NULL, NULL);
        buffer[bytes_received] = '\0';

        // Display the echoed message
        printf("Echoed message from server: %s\n", buffer);
    }

    close(client_socket);
    return 0;
}

