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
#include <semaphore.h>

#include "config.h"

#define     SINE_STEP       1e-1
#define     SQUARE_CYCLE    50
#define     SQUARE_DUTY     5e-1

/********************* Global Variable Definition *********************/
int myIndex = 0;            // 
int mode = 0;               // Wave form mode
sem_t sendSignal;           // Send signal, use semaphore is more easy

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
    // TODO: Create socket
    int sockfd;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        printf("Thread-Sender error: cannot create socket\n");
        return NULL;
    }

    struct sockaddr_in srvaddr;
    socklen_t srvaddrLen;
    memset(&srvaddr, 0, sizeof(srvaddr));
    srvaddr.sin_family  = AF_INET;
    inet_pton(AF_INET, SRV_ADDR, (void*)&srvaddr.sin_addr);
    srvaddr.sin_port    = htons(SRV_PORT);
    srvaddrLen = sizeof(srvaddr);

    char buff[BUFF_SIZE] = "hello, world";
    
    while (1) {
        sem_wait(&sendSignal);
        // Send packet
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
 * @brief 
 * 
 * @param arg 
 * @return void* 
 */
void* tick(void* arg) {
    while (1) {
        int ctrl = control();
        for (int i = 0; i < MAX_SEND_PACKET_PER_SLICE; ++i) {
            if (ctrl > 0) {
                sem_post(&sendSignal);
                --ctrl;
            }
            usleep(SEND_INTERVAL);
        }
    }
    return NULL;
}

/**
 * @brief 
 * 
 * @return int 
 */
int control() {
    int ans = 0;
    switch (mode)
    {
    case 1: {
        double x = myIndex * SINE_STEP;
        ans = (int)(MAX_SEND_PACKET_PER_SLICE * (0.5 + sin(x)/2));
        break;
    }
    case 2: {
        myIndex %= SQUARE_CYCLE;
        ans = myIndex < (int)(SQUARE_CYCLE*SQUARE_DUTY) ? 0 : MAX_SEND_PACKET_PER_SLICE;
        break;
    }
    default:
        break;
    }
    ++myIndex;
    return ans;
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
    mode = atoi(argv[1]);
    if (mode != 1 && mode != 2) {
        printf("Argument Error: controller must be set as 1 or 2\n");
        exit(1);
    }

    // Initialize semaphore
    sem_init(&sendSignal, 0, 0);

    // Create sender and ticker thread
    pthread_t sender;
    pthread_t ticker;
    if (pthread_create(&sender, NULL, sender, NULL) < 0) {
        printf("Thread-Sender Error: cannot create thread.\n");
        exit(1);
    }
    if (pthread_create(&ticker, NULL, tick, NULL) < 0) {
        printf("Thread-Tick Error: cannot create thread.\n");
        exit(1);
    }

    return 0;
}