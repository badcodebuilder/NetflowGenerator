/**
 * @file srv.c
 * @author BadCodeBuilder
 * @brief 
 * @version 0.1
 * @date 2020-11-07
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
sem_t mutex;            // A packet to one thread

/**
 * @brief 
 * 
 * @param arg 
 * @return void* 
 */
void* recver(void* arg) {
    clock_t clk;

    redisContext* conn;
    conn = redisConnect(REDIS_SRV_ADDR, REDIS_SRV_PORT);
    
    while (1) {
        sem_wait(&mutex);
        clk = clock();
        redisCommand(conn, "rpush packet %ld", clk);
    }

    redisFree(conn);
    return NULL;
}

/**
 * @brief 
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char* argv[]) {
    sem_init(&mutex, 0, 0);
    // TODO: Create UDP server
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

    // Create all recording threads
    pthread_t recvers[MAX_RECVER_THREAD_CNT];
    for (int i = 0; i < MAX_RECVER_THREAD_CNT; ++i) {
        res = pthread_create(recvers+i, NULL, recver, NULL);
        if (res != 0) {
            printf("Thread%02d error: cannot create thread, errno=%d.\n", i, res);
            --i;
        }
    }

    char buff[BUFF_SIZE];
    int lenth;
    struct sockaddr cliaddr;
    socklen_t cliaddrLen;
    while (1) {
        // TODO: Receive packet, actually I do not care about client address
        lenth = recvfrom(sockfd, buff, BUFF_SIZE, 0, &cliaddr, &cliaddrLen);
        // lenth = recvfrom(sockfd, buff, BUFF_SIZE, 0, NULL, NULL);
        // XXX: Not a good solution, the responsing thread cannot get packet
        sem_post(&mutex);
    }
    return 0;
}