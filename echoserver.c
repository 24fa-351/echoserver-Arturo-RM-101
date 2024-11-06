#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

#define PORT 46645
#define LISTEN_BACKLOG 5

// We get a vairable to see which port we're using with "-p"
// Another vairable to send the message in the server
// Utilizing socket() to create a server we can communicate with
// We will need a way to listen
// We will also need a way to accept

void* handleConnection(void* ptr)
{
    int* a_client_ptr = (int*)ptr;
    int a_client = *a_client_ptr;
    free(a_client_ptr);

    char buffer[1024];

    int bytes_read = read(a_client, buffer, sizeof(buffer));
    while (bytes_read != -1 && bytes_read != 0) {
        printf("Received: %s\n", buffer);
        write(a_client, buffer, bytes_read);

        bytes_read = read(a_client, buffer, sizeof(buffer));
    }
    printf("Connection terminated\n");
    return NULL;
}

int main(int argc, char* argv[])
{
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in socket_address;
    memset(&socket_address, '\0', sizeof(socket_address));
    socket_address.sin_family = AF_INET;
    socket_address.sin_addr.s_addr = htonl(INADDR_ANY);
    socket_address.sin_port = htons(PORT);

    int returnval;

    returnval = bind(
        socket_fd, (struct sockaddr*)&socket_address, sizeof(socket_address));

    returnval = listen(socket_fd, LISTEN_BACKLOG);

    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);



    while (1) {

        int client_fd = accept(socket_fd, (struct sockaddr*)&client_address, &client_address_len);
        if (client_fd == -1) {
            break;
        }

        int* client_fd_location = malloc(sizeof(int));
        *client_fd_location = client_fd;

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handleConnection, client_fd_location);
        //handleConnection(client_fd);
        
    }
    return 0;
}