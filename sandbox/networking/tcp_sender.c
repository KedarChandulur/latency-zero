#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <errno.h>

#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>


// Needed for both TCP and UDP

// int getaddrinfo(const char *node,   // e.g. "www.example.com" or IP
//                 const char *service,  // e.g. "http" or port number
//                 const struct addrinfo *hints,
//                 struct addrinfo **res);

// void freeaddrinfo (struct addrinfo *__ai)

// int socket(int domain, int type, int protocol);

// int setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

// int bind(int sockfd, struct sockaddr *my_addr, int addrlen);


// Below are the required for TCP

// int listen(int sockfd, int backlog);

// int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

// int send(int sockfd, const void *msg, int len, int flags);

// int recv(int sockfd, void *buf, int len, int flags);


// Below are for UDP

// int sendto(int sockfd, const void *msg, int len, unsigned int flags,
//            const struct sockaddr *to, socklen_t tolen);

// int recvfrom(int sockfd, void *buf, int len, unsigned int flags,
//              struct sockaddr *from, int *fromlen);


int main()
{
    printf("\ntcp_server initializing...\n");

    //const char* ipaddr = NULL; // need to change this to the IP address of the server.
    const char* ipaddr = "127.0.0.1"; // loopback addr.
    const char* port = "8080";

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; // Either IPv4 or IPv6
    //hints.ai_flags = AI_PASSIVE; // By using the AI_PASSIVE flag, I’m telling the program to bind to the IP of the host i am running on
    hints.ai_socktype = SOCK_STREAM; // TCP Stream socket

    struct addrinfo* res = NULL;

    const int status = getaddrinfo(ipaddr, port, &hints, &res);
    
    // Error with getaddrinfo.
    if (status != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return EXIT_FAILURE;
    }

    const int domain = res->ai_family; // Family of socket, IPv4 or IPv6.
    const int type = res->ai_socktype; // Type of socket, Datagram or Stream.
    const int protocol = res->ai_protocol; // Protocol used by socket, TCP or UDP

    const int sockfd = socket(domain, type, protocol);

    // Error with socket creation
    if(sockfd < 0)
    {
        printf("\nError: socket() creation failed! error status: %s", strerror(errno));
        return EXIT_FAILURE;
    }

    const int opt = 1;
    const int sockoptstatus = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if(sockoptstatus < 0)
    {
        printf("\nError: setsockopt() failed! error status: %s", strerror(errno));
        return EXIT_FAILURE;
    }

    struct sockaddr* server_addr = (struct sockaddr*)res->ai_addr; // This Server address.
    socklen_t server_addr_len = res->ai_addrlen; // Length of the server address struct

    const int bindstatus = bind(sockfd, server_addr, server_addr_len);

    if(bindstatus < 0)
    {
        printf("Error: bind() failed! error status: %s", strerror(errno));

        return EXIT_FAILURE;
    }

    const int backlog = 1; // Maximum number of pending connections.
    const int listenstatus = listen(sockfd, backlog);
    
    if(listenstatus < 0)
    {
        printf("\nError: listen() failed! error status: %s", strerror(errno));
        return EXIT_FAILURE;
    }
    else
    {
        printf("\nServer listening on port\n");
    }

    struct sockaddr_storage their_addr;
    socklen_t their_addr_len = sizeof(their_addr);

    const int newfd = accept(sockfd, (struct sockaddr*)&their_addr, &their_addr_len);

    if(newfd < 0)
    {
        printf("\nError: accept() failed! error status: %s", strerror(errno));
        return EXIT_FAILURE;
    }
    else
    {
        printf("\nConnection accepted!\n");
    }

    const char* message = "Hello from the TCP Server!";
    const int message_len = strlen(message);
    const int bytes_sent = send(newfd, message, message_len, 0);

    if(bytes_sent < 0)
    {
        printf("\nError: Server failed to send the message! error status: %s", strerror(errno));
    }
    else
    {
        printf("\nSuccess: Message from server sent successfully\n");
    }

    close(sockfd);

    close(newfd);

    freeaddrinfo(res); // freeing the linked list.

    exit(EXIT_SUCCESS);
}