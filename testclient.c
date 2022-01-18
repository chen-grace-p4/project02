#include "networking.h"
//static void sighandler(int signo);

int main() {
  //signal(SIGINT, sighandler);
  //USE SELECT beej.us/guide/bgnet/html/#select
  int server;
  char buffer[BUFFER_SIZE];
  server = client_setup(TEST_IP);

  while (1) {
    int count;
    ioctl(server, FIONREAD, &count);
    //printf("status: %d\n", count);
    //means server socket has something to send
    if (count != 0) {
      read(server, buffer, sizeof(buffer));
  		printf("received: -%s-\n", buffer);
      //printf("status2: %d\n", count);
    }
    //server socket is empty so you can send something
    else {
      printf("Input: ");
      //IS THERE A WAY TO CANCEL FGETS SO THAT IT DOESN'T STALL
      //RECEIVING MESSAGES?
      //PROBLEM: CLIENT ONLY SEES RECIEVED MESSAGES IF THEY SEND THEIR OWN MESSAGE
      //RIGHT BEFORE IT
  		fgets(buffer, sizeof(buffer), stdin);

  		//replace newline with terminating null
  		*strchr(buffer, '\n') = 0;

      char temp[BUFFER_SIZE];
      strcpy(temp, buffer);

      //separate buffer into array and check first word for command
      char * first = strtok(buffer, " ");
      if (strcasecmp(first, "update") == 0) {
        printf("updating to most recent messages\n");
      } else {
        //printf("sending: -%s-\n", buffer);
    		//write(server, buffer, sizeof(buffer));
        write(server, temp, sizeof(temp));
      }
    }
		// read(server, buffer, sizeof(buffer));
    //
		// printf("received: -%s-\n", buffer);
  }
  return 0;
}

// static void sighandler(int signo) {
//   if (signo == SIGINT) {
//     remove(WKP);
//     exit(EXIT_SUCCESS);
//   }
// }
