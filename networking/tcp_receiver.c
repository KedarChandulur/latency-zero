#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#include <netdb.h>
#include <string.h>
#include <sys/socket.h>


// Needed for both TCP and UDP

// int getaddrinfo(const char *node,   // e.g. "www.example.com" or IP
//                 const char *service,  // e.g. "http" or port number
//                 const struct addrinfo *hints,
//                 struct addrinfo **res);

// void freeaddrinfo (struct addrinfo *__ai)

// int socket(int domain, int type, int protocol);

// int setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));


// Below are the required for TCP

// int connect(int sockfd, struct sockaddr *serv_addr, int addrlen);

// int send(int sockfd, const void *msg, int len, int flags);

// int recv(int sockfd, void *buf, int len, int flags);


// Below are for UDP

// int sendto(int sockfd, const void *msg, int len, unsigned int flags,
//            const struct sockaddr *to, socklen_t tolen);

// int recvfrom(int sockfd, void *buf, int len, unsigned int flags,
//              struct sockaddr *from, int *fromlen);


int main()
{
    printf("\ntcp_client initializing...\n");

    //const char* ipaddr = NULL; // need to change this to the IP address of the server.
    const char* ipaddr = "127.0.0.1"; // loopback addr
    const char* port = "8080";

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; // Either IPv4 or IPv6
    //hints.ai_flags = AI_PASSIVE; // By using the AI_PASSIVE flag, I’m telling the program to bind to the IP of the host i am running on
    hints.ai_socktype = SOCK_STREAM; // TCP Stream socket

    struct addrinfo* res = NULL;
    const int status = getaddrinfo(ipaddr, port, &hints, &res);
    
    // Error with getaddrinfo
    if (status != 0)
    {
        printf("Error: getaddrinfo() failed! status: %s", gai_strerror(status));
        return EXIT_FAILURE;
    }

    const int domain = res->ai_family; // Family of socket, IPv4 or IPv6.
    const int type = res->ai_socktype; // Type of socket, Datagram or stream.
    const int protocol = res->ai_protocol; // Protocol used by socket, TCP or UDP.

    const int sockfd = socket(domain, type, protocol);

    // Error with socket creation
    if(sockfd < 0)
    {
        perror("\nError: socket() creation failed");
        return EXIT_FAILURE;
    }

    const int opt = 1;
    const int sockoptstatus = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if(sockoptstatus < 0)
    {
        printf("\nError: setsockopt() failed! error status: %s", strerror(errno));
        return EXIT_FAILURE;
    }

    const int connectstatus = connect(sockfd, res->ai_addr, res->ai_addrlen); // Listen for incoming connections

    if(connectstatus < 0)
    {
        printf("Error: connect() failed! error status: %s", strerror(errno));
        return EXIT_FAILURE;
    }

    char buffer[1024]; // Buffer to store received data
    const int bytes_received = recv(sockfd, buffer, sizeof(buffer), 0); // Receive data from the server

    if(bytes_received < 0)
    {
        printf("\nError: Failed to receive the data from server! error status: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    buffer[bytes_received] = '\0';
    printf("\nMessage from server arrived: %s\n", buffer);

    close(sockfd);

    freeaddrinfo(res); // freeing the linked list.

    return EXIT_SUCCESS;
}