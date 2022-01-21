
#include "networking.h"

int main() {
  int fd, listen_socket, maxfd, client;
  //this is the fd_set that will be modified by select
  fd_set read_fds;
  //this is the full fd_set to keep a stable record
  fd_set full_fds;
  char buffer[BUFFER_SIZE];

  //make sure clients is totally empty
  FD_ZERO( &clients );
  server_socket = client_setup();
  printf("server socket: %d\n", server_socket);

  //add the listening socket to the master set
  FD_SET(server_socket, &clients);
  //we have to keep track of the largest file descriptor for select
  //currently, listen_socket is the only, and therefore largest
  maxfd = server_socket;


  while (1) {
    //dont need this because we wont be removing any file descriptors unless server closes
    read_fds = full_fds;

  	// puts ready descriptors into read_fds
    int i = select(maxfd + 1, &read_fds, NULL, NULL, NULL);
    error_check_sock(i, "select", listen_socket);
    printf("select ready: %d\n", i);

    //all we know is a file descriptor is ready, we don't know which one
    //loop through read_fds for any available socket
    for (fd = 0; fd <= maxfd; fd++) {

      //will return true if the provided file descriptor is in read_fds
      if (FD_ISSET(fd, &read_fds)) {

        //listen socket ready means new client
        if (fd == listen_socket) {
          client = server_connect(listen_socket);
          FD_SET(client, &clients);

          //make sure to update maxfd
          if (client > maxfd) maxfd = client;
        }//new connections

        //listen socket not ready so existing client
        else {
          if (read(fd, buffer, sizeof(buffer)) ) {
            
            }

            //write(fd, buffer, sizeof(buffer));
          }//data to be read
          else {
            //remove this descriptor from the full set
            FD_CLR(fd, &clients);
            close(fd);
          }//close the connection
        }//existing socket
      }//descriptor is readhy
    }//loop through sockets
  }//forever
  close(server_socket);
  return 0;
}
