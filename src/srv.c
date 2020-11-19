/**
 * @file srv.c
 * @author BadCodeBuilder
 * @brief 
 * @version 0.1
 * @date 2020-11-19
 * 
 */

#include "hiredis/hiredis.h"
#include "pthread.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <semaphore.h>

#include "config.h"

/********************* Global Variable Definition *********************/
int count = 0;          // Number of task

/**
 * @brief 
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char* argv[]) {
    // Create Redis connection
    redisContext* conn;
    conn = redisConnect(REDIS_SRV_ADDR, REDIS_SRV_PORT);

    // Create UDP server
    int res;
    int sockfd;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        printf("Socket error: cannot create socket.\n");
        exit(1);
    }

    struct sockaddr_in srvaddr;
    socklen_t srvaddrLen;
    memset(&srvaddr, 0, sizeof(srvaddr));
    srvaddr.sin_family      = AF_INET;
    srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    srvaddr.sin_port        = htons(SRV_PORT);
    srvaddrLen = sizeof(srvaddr);
    res = bind(sockfd, (struct sockaddr*)&srvaddr, srvaddrLen);
    if (res != 0) {
        printf("Socket error: cannot bind address or port to socket");
        return 1;
    }

    char buff[BUFF_SIZE];
    int lenth;
    struct sockaddr cliaddr;
    socklen_t cliaddrLen;
    time_t baseSec = 0;
    struct timeval clk;
    long myClk;
    while (1) {
        // Receive packet, actually I do not care about client address
        lenth = recvfrom(sockfd, buff, BUFF_SIZE, 0, &cliaddr, &cliaddrLen);

        // Get current time, use us is enough
        gettimeofday(&clk, NULL);
        if (baseSec == 0) {
            baseSec = clk.tv_sec;
        }
        myClk = (clk.tv_sec-baseSec)*1000000 + clk.tv_usec;

        // Record current time
        redisCommand(conn, "rpush packet %ld", myClk);
    }

    return 0;
}