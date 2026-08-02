#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


// Needed for both TCP and UDP

// int getaddrinfo(const char *node,   // e.g. "www.example.com" or IP
//                 const char *service,  // e.g. "http" or port number
//                 const struct addrinfo *hints,
//                 struct addrinfo **res);

// void freeaddrinfo (struct addrinfo *__ai)

// int socket(int domain, int type, int protocol);


// Below are the required for UDP

// int bind(int sockfd, struct sockaddr *my_addr, int addrlen);

// int sendto(int sockfd, const void *msg, int len, unsigned int flags,
//            const struct sockaddr *to, socklen_t tolen);

// int recvfrom(int sockfd, void *buf, int len, unsigned int flags,
//              struct sockaddr *from, int *fromlen);


// Below are for TCP

// int listen(int sockfd, int backlog);

// int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);


int main()
{
    printf("udp_sender init\n");

    //const char* ipaddr = NULL; // need to change this.
    const char* ipaddr = "127.0.0.1"; // loopback addr.
    const char* port = "8080";

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; // Either IPv4 or IPv6
    //hints.ai_flags = AI_PASSIVE; // By using the AI_PASSIVE flag, I’m telling the program to bind to the IP of the host i am running on
    hints.ai_socktype = SOCK_DGRAM; // UDP Datagram socket

    struct addrinfo* res = NULL;

    const int status = getaddrinfo(ipaddr, port, &hints, &res);

    // Error with getaddrinfo.
    if(status != 0)
    {
        printf("Error: getaddrinfo() failed! status: %s", gai_strerror(status));
        return EXIT_FAILURE;
    }

    const int domain = res->ai_family; // Family of socket, IPv4 or IPv6.
    const int type = res->ai_socktype; // Type of socket, Datagram or Stream.
    const int protocol = res->ai_protocol; // Protocol used by socket, TCP or UDP

    const int sockfd = socket(domain, type, protocol);

    // Error with socket creation
    if(sockfd < 0)
    {
        //printf("\nError: socket() creation failed! error status: %s", strerror(errno));
        perror("\nError: socket() creation failed");

        return EXIT_FAILURE;
    }

    struct sockaddr* server_addr = (struct sockaddr*)res->ai_addr; // This Server address.
    socklen_t server_addr_len = res->ai_addrlen; // Length of server address struct

    const int bindstatus = bind(sockfd, server_addr, server_addr_len);

    // Error with binding
    if(bindstatus < 0)
    {
        printf("\nError: bind() failed! error status: %s", strerror(errno));
        //perror("\nError: bind() failed");

        return EXIT_FAILURE;
    }
    
    char buffer[1024];
    const int bytes_received = recvfrom(sockfd, buffer, sizeof(buffer), 0, server_addr, &server_addr_len);
    
    if (bytes_received < 0)
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