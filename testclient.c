
#include "networking.h"
//static void sighandler(int signo);

int main() {
  //signal(SIGINT, sighandler);

  int server;
  char buffer[BUFFER_SIZE];
  server = client_handshake();

  while (1) {
    printf("Input: ");
		fgets(buffer, sizeof(buffer), stdin);

		//replace newline with terminating null
		*strchr(buffer, '\n') = 0;

		//using sizeof is better because strlen could easily change
		write(server, buffer, sizeof(buffer));
		read(server, buffer, sizeof(buffer));

		printf("received: -%s-\n", buffer);
  }
  return 0;
}

// static void sighandler(int signo) {
//   if (signo == SIGINT) {
//     remove(WKP);
//     exit(EXIT_SUCCESS);
//   }
// }
