#include "networking.h"
//static void sighandler(int signo);

int main() {
  //signal(SIGINT, sighandler);

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
  		fgets(buffer, sizeof(buffer), stdin);

  		//replace newline with terminating null
  		*strchr(buffer, '\n') = 0;

  		//using sizeof is better because strlen could easily change
  		write(server, buffer, sizeof(buffer));
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
