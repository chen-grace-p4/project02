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
#define ACTIVITYLOG "activity.log"

void error_check( int i, char *s );
void error_check_sock( int i, char *s, int sd );

int server_setup();
int server_connect(int sd);

int client_setup(char * server);

unsigned long file_size(char *file);

// void add_activity(int id, int type);
void add_activity(int type);


//int udp_server_setup();
//struct addrinfo *  udp_client_setup(char * server, int *sock);

struct chatlog {
  int userid;
  struct tm time;
  char message[1000];
};

struct activitylog {
  // int userid;
  struct tm time;
  int activity;
};

#endif
