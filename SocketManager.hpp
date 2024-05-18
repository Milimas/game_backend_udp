#ifndef SOCKETMANAGER_HPP
#define SOCKETMANAGER_HPP

#include "Player.hpp"
#include "Game.hpp"
#include "MatchMaking.hpp"
#include <iostream>
#include <stdint.h>
#include <map>
#include <cstdio>
#include <unistd.h>
#include <cstring>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class SocketManager
{
public:
	static int sockfd; /* socket */
	static int portno; /* port to listen on */
	static socklen_t clientlen; /* byte size of client's address */
	static struct sockaddr_in serveraddr; /* server's addr */
	static struct sockaddr_in clientaddr; /* client addr */
	static struct hostent *hostp; /* client host info */
	static char buf[BUFSIZE]; /* message buf */
	static char *hostaddrp; /* dotted decimal host addr string */
	static int n; /* message byte size */

    /*
     * error - wrapper for perror
     */
    static void error( std::string msg ) {
        perror(msg.c_str());
        exit(1);
    }

    static void print_sockaddr_in(const struct sockaddr_in *addr)
    {
        char ip_address[INET_ADDRSTRLEN] ;

        // Address family
        printf("Address Family: ") ;
        switch (addr->sin_family)
        {
            case AF_INET:
                printf("AF_INET (IPv4)\n") ;
                break;
            case AF_INET6:
                printf("AF_INET6 (IPv6)\n") ;
                break;
            default:
                printf("Unknown address family: %d\n", addr->sin_family) ;
        }

        // Port (in host byte order)
        printf("Port: %d\n", ntohs(addr->sin_port)) ;

        // IP Address (converted to string)
        if (inet_ntop(AF_INET, &addr->sin_addr, ip_address, INET_ADDRSTRLEN) != NULL)
            printf("IP Address: %s\n", ip_address);
        else
            perror("inet_ntop");
    }

    static int createSocket( void )
    {
        /* 
         * socket: create the parent socket 
         */
        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd < 0) 
            error("ERROR opening socket");
        std::cerr << __PRETTY_FUNCTION__ << ": SUCCESS" << std::endl ;
        return (sockfd) ;
    }

    static void  setSocketOpt( void )
    {
	    int optval; /* flag value for setsockopt */

        /* setsockopt: Handy debugging trick that lets 
         * us rerun the server immediately after we kill it; 
         * otherwise we have to wait about 20 secs. 
         * Eliminates "ERROR on binding: Address already in use" error. 
         */
        optval = 1;
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval , sizeof(int));
        std::cerr << __PRETTY_FUNCTION__ << ": SUCCESS" << std::endl ;
    }

    static struct sockaddr_in buildSockaddr( int portno )
    {
        /*
         * build the server's Internet address
         */
        bzero((char *) &serveraddr, sizeof(serveraddr));
        serveraddr.sin_family = AF_INET;
        serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
        serveraddr.sin_port = htons((unsigned short)portno);
        std::cerr << __PRETTY_FUNCTION__ << ": SUCCESS" << std::endl ;
        return (serveraddr) ;
    }

    static void bindSocket( void )
    {
        /* 
         * bind: associate the parent socket with a port 
         */
        if (bind(sockfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0) 
            error("ERROR on binding");
        std::cerr << __PRETTY_FUNCTION__ << ": SUCCESS" << std::endl ;
    }

    static void startServer( int portno )
    {
        createSocket() ;
        setSocketOpt() ;
        buildSockaddr(portno) ;
        bindSocket() ;
        std::cerr << __PRETTY_FUNCTION__ << ": SERVER IS RUNNING ON PORT: " << portno << std::endl ;
        /* 
         * main loop: wait for a datagram, then echo it
         */
        clientlen = sizeof(clientaddr);
        MatchMaking matchMaking ;
        while (1) {
            /*
             * recvfrom: receive a UDP datagram from a client
             */
            bzero(buf, BUFSIZE);
            n = recvfrom(sockfd, buf, BUFSIZE, 0,
                    (struct sockaddr *) &clientaddr, &clientlen);
            if (n < 0)
                error("ERROR in recvfrom");
            if (n == 0)
                exit(2) ;

            /* 
             * gethostbyaddr: determine who sent the datagram
             */
            // hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, sizeof(clientaddr.sin_addr.s_addr), AF_INET);
            // if (hostp == NULL)
            // 	error("ERROR on gethostbyaddr");
            // hostaddrp = inet_ntoa(clientaddr.sin_addr);
            // if (hostaddrp == NULL)
            // 	error("ERROR on inet_ntoa\n");
            // printf("server received datagram from %s (%s)\n", 
            // 	hostp->h_name, hostaddrp);
            // printf("server received %ld/%d bytes: %s\n", strlen(buf), n, buf);

            matchMaking.createNewPlayer(clientaddr) ;
            if ( matchMaking.isPlayerInGame(clientaddr) )
            {
                /* 
                 * sendto: echo the input back to the other player 
                 */
                struct sockaddr_in otheraddr = matchMaking.getOpponentSockaddr(clientaddr) ;
                n = sendto(sockfd, buf, BUFSIZE, 0, (struct sockaddr *) &otheraddr, sizeof(otheraddr)) ;
                if (n < 0) 
                    error("ERROR in sendto");
            }
        }
    }

};

bool operator <(const struct sockaddr_in &addr1, const struct sockaddr_in &addr2) {
  if (addr1.sin_family != addr2.sin_family) {
    return addr1.sin_family < addr2.sin_family; // Order by address family
  }
  if (addr1.sin_port != addr2.sin_port) {
    return (ntohs(addr1.sin_port) < ntohs(addr2.sin_port)); // Compare ports in host byte order
  }
  return (0 > memcmp(&addr1.sin_addr, &addr2.sin_addr, sizeof(addr1.sin_addr))); // Compare IP addresses
}

int SocketManager::sockfd; /* socket */
socklen_t SocketManager::clientlen; /* byte size of client's address */
struct sockaddr_in SocketManager::serveraddr; /* server's addr */
struct sockaddr_in SocketManager::clientaddr; /* client addr */
struct hostent *SocketManager::hostp; /* client host info */
char SocketManager::buf[BUFSIZE]; /* message buf */
char *SocketManager::hostaddrp; /* dotted decimal host addr string */
int SocketManager::n; /* message byte size */

#endif
