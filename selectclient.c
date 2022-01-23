
#include "networking.h"

int main() {
  int fd, server_socket, maxfd, server;
  //this is the fd_set that will be modified by select
  fd_set read_fds;
  //this is the full fd_set to keep a stable record
  fd_set full_fds;
  char buffer[BUFFER_SIZE];

  //make sure clients is totally empty
  FD_ZERO( &full_fds );
  server_socket = client_setup(TEST_IP);
  printf("server socket: %d\n", server_socket);

  //add the listening socket to the master set
  FD_SET(server_socket, &full_fds);
  //FD_SET(STDIN, &full_fds);
  //we have to keep track of the largest file descriptor for select
  //currently, listen_socket is the only, and therefore largest
  maxfd = server_socket;


  while (1) {
    //dont need this because we wont be removing any file descriptors unless server closes
    read_fds = full_fds;

  	// puts ready descriptors into read_fds
    int i = select(maxfd + 1, &read_fds, NULL, NULL, NULL);
    error_check_sock(i, "select", server_socket);
    printf("select ready: %d\n", i);

    //all we know is a file descriptor is ready, we don't know which one
    //loop through read_fds for any available socket
    for (fd = 0; fd <= maxfd; fd++) {
      //will return true if the provided file descriptor is in read_fds
      if (FD_ISSET(fd, &read_fds)) {
           //server socket ready means new message
           printf("ready socket: %d\n", fd);
           printf("server socket: %d\n", server_socket);
           if (fd == server_socket) {
              //DISPLAY NEW MESSAGE
              read(server, buffer, sizeof(buffer));
              printf("received: -%s-\n", buffer);

           }
           //server socket not ready so go to stdin
           else {
             printf("Input: ");
             fgets(buffer, sizeof(buffer), stdin);

             //replace newline with terminating null
             *strchr(buffer, '\n') = 0;

             char temp[BUFFER_SIZE];
             strcpy(temp, buffer);

             //strcpy(temp, strcat(username, ": "));
             //strcpy(temp, strcat(temp, buffer));

             //separate buffer into array and check first word for command
             char * first = strtok(buffer, " ");
             if (strcasecmp(first, "update") == 0) {
               printf("updating to most recent messages\n");
             } else {
               //printf("sending: -%s-\n", buffer);
          		//write(server, buffer, sizeof(buffer));
               write(server, temp, sizeof(temp));
             }

             //removes stdin from read_fds
             FD_CLR(fd, &read_fds);
             //close(fd);
          }//existing socket
      }//file descriptor is readhy
      else {

      }
    }//loop through sockets
  }//forever
  close(server_socket);
  return 0;
}
