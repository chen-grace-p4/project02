#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <ifaddrs.h>
#include <time.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>

#include <fcntl.h> // open
#include <unistd.h> // close

#include <sys/ioctl.h>

#ifndef NETWORKING_H
#define NETWORKING_H

#define BUFFER_SIZE 256
#define PORT "9002"
#define TEST_IP "127.0.0.1"

#define STDIN 0

#define MAX_CLIENTS 10
#define LISTEN 0

#define CHATLOG "chat.log"

void error_check( int i, char *s );
void error_check_sock( int i, char *s, int sd );

int server_setup();
int server_connect(int sd);

int client_setup(char * server);

//int udp_server_setup();
//struct addrinfo *  udp_client_setup(char * server, int *sock);

struct log {
  int userid; // 1, 2, 3, ... --> prints out as user1, user2, user3, ...
  struct tm time; // epoch time --> prints out as DAY-OF-WEEK YYYY-MM-DD HH:MM:SS TIME-ZONE
  char message[1000];
};

#endif
