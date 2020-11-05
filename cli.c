/**
 * @file cli.c
 * @author BadCodeBuilder
 * @brief 
 * @version 0.1
 * @date 2020-11-03
 * 
 */
#include "pthread.h"
#include <math.h>
#include <signal.h>
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

#include "config.h"

/********************* Global Variable Definition *********************/
int controller = 0;         // Controller

/**
 * @brief Thread function, if the controller is greater than threshold, 
 * it will send UDP packet to the server. Sending a UDP packet should 
 * be very quick and not too strict(I mean that sending one more or less
 * packet occasionally dose no harm to the program), so we can ignore
 * the read-write conflict about the controller changing. 
 * 
 * @param arg 
 * @return void* 
 */
void* sender(void* arg) {
    // BUG: The arg is wrong. An independent arg-array is a must.
    int threshold = *((int*)arg);
    // TODO: Create socket
    int sockfd;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        printf("Thread%02d error: cannot create socket\n", threshold);
        return NULL;
    }

    struct sockaddr_in srvaddr;
    socklen_t srvaddrLen;
    memset(&srvaddr, 0, sizeof(srvaddr));
    srvaddr.sin_family      = AF_INET;
    inet_pton(AF_INET, SRV_ADDR, (void*)&srvaddr.sin_addr);
    srvaddr.sin_port        = htons(SRV_PORT);
    srvaddrLen = sizeof(srvaddr);

    char buff[BUFF_SIZE]    = "hello, world";
    
    while (1) {
        while (controller <= threshold);
        // TODO: Send packet
        sendto(
            sockfd,
            buff,
            strlen(buff),
            0,
            (struct sockaddr*)&srvaddr,
            srvaddrLen
        );
    }
    return NULL;
}

/**
 * @brief Control the global variable: controller, to control all the 
 * threads whether send UDP packages or not. The value of the wave should
 * be related to the number of working thread. And use different mode to
 * make different kinds of wave
 * 
 * @param mode 
 * @return int 
 */
int control(int mode) {
    if (mode == 1) {
        // Sine wave
        double x = 0;
        double interval = 1e-1;
        while (1) {
            x += interval;
            controller = (int)(MAX_SENDER_THREAD_CNT * (1 + sin(x)));
            usleep(SEND_INTERVAL);
        }
    } else if (mode == 2) {
        // Square wave
        int x = 0;
        int cycle = 100;
        int duty = 50;
        while (1) {
            x = (x+1) % cycle;
            controller = x < duty? 0 : MAX_SENDER_THREAD_CNT;
            usleep(SEND_INTERVAL);
        }
    }
    return 0;
}

/**
 * @brief 
 * 
 * @param sig 
 */
void sigHandler(int sig) {
    if (sig == SIGINT) {
        // TODO: terminal all thread
    }
}

/**
 * @brief 
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char* argv[]) {
    // Parse arguments
    if (argc != 2) {
        printf("Argument Error: too many or few arguments\n");
        exit(1);
    }
    int mode = atoi(argv[1]);
    if (mode != 1 && mode != 2) {
        printf("Argument Error: controller must be set as 1 or 2\n");
        exit(1);
    }

    // Create sender threads
    int threadCnt;
    pthread_t senders[MAX_SENDER_THREAD_CNT];
    for (threadCnt = 0; threadCnt < MAX_SENDER_THREAD_CNT; ++threadCnt) {
        int threshold = threadCnt;
        int res = pthread_create(
            senders+threadCnt,
            NULL,
            sender,
            (void*)&threshold
        );
        if (res < 0) {
            printf("Thread main Error: cannot create Thread%02d", threadCnt);
        }
    }

    control(mode);

    return 0;
}