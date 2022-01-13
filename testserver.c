
#include "networking.h"
//static void sighandler(int signo);

int main() {
  //signal(SIGINT, sighandler);
  int client;
  int sd = server_setup();

  while(1) {
    printf("\nStarting up or a client joined/exited...\n");
    client = server_connect(sd);

    int f;
    f = fork();

    //subserver
    if (f == 0) {
      //char buff[BUFFER_SIZE];
      //to_client = server_connect(from_client);

      while(1) {
        //recieve data
        char buff[BUFFER_SIZE];
        int r = read(client, buff, sizeof(buff));
        if (!r) break;

        //process
        buff[strlen(buff)] = '\0';
        strcat(buff, "manipulated");

        //sends back
        write(client, buff, sizeof(buff));
      }
    } else { //server
      //remove(WKP);
      close(client);
    }
  }
  return 0;
}

// static void sighandler(int signo) {
//   if (signo == SIGINT) {
//     remove(WKP);
//     exit(EXIT_SUCCESS);
//   }
// }
