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

// int sendto(int sockfd, const void *msg, int len, unsigned int flags,
//            const struct sockaddr *to, socklen_t tolen);

// int recvfrom(int sockfd, void *buf, int len, unsigned int flags,
//              struct sockaddr *from, int *fromlen);


// Below are for TCP

// int connect(int sockfd, struct sockaddr *serv_addr, int addrlen);

// int send(int sockfd, const void *msg, int len, int flags);

// int recv(int sockfd, void *buf, int len, int flags);


int main()
{
    printf("\nudp_receiver init\n");

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
        printf("\nError: getaddrinfo() failed! error status: %s", gai_strerror(status));
        exit(EXIT_FAILURE);
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

        exit(EXIT_FAILURE);
    }

    const struct sockaddr* server_addr = (struct sockaddr*)res->ai_addr; // Destination Server address.
    const socklen_t server_addr_len = res->ai_addrlen; // Length of server address struct

    // const int connectstatus = connect(sockfd, server_addr, server_addr_len);

    // if(connectstatus < 0)
    // {
    //     printf("\nError: connect() failed! error status: %s", strerror(errno));
    //     //perror("\nError: connect() failed");

    //     exit(EXIT_FAILURE);
    // }

    const char* message = "Hello, UDP Server!";
    const int message_len = strlen(message);
    const int bytes_sent = sendto(sockfd, message, message_len, 0, server_addr, server_addr_len);

    if(bytes_sent != message_len)
    {
        printf("\nError: receiver send() failed!");
    }
    else
    {
        printf("\nSuccess: receiver send() succeeded!");
    }

    struct sockaddr* receiver_addr;
    socklen_t* receiver_addr_len;

    char buffer[1024];

    const int bytes_received = recvfrom(sockfd, buffer, sizeof(buffer), 0, receiver_addr, receiver_addr_len);
    
    if (bytes_received < 0)
    {
        printf("\nError: receiver recv() failed!");
        exit(EXIT_FAILURE);
    }

    buffer[bytes_received] = '\0';
    printf("\nreceiver Received: %s\n", buffer);

    close(sockfd);

    freeaddrinfo(res); // freeing the linked list.

    exit(EXIT_SUCCESS);
}