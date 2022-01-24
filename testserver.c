
#include "networking.h"

int main() {
   printf("INSTRUCTIONS FOR CLIENTS:\n");
   printf("\t - all clients should be connected at once in the beginning to see all messages.\n");
   printf("\t  clients connected afterwards will not be able to see or recieve past messages.\n");
   printf("\n");
   printf("\t - begin inputs with -m or -message to send a normal text message.\n");
   printf("\t - send -h or -history to view chatlog history.\n");
   printf("\n");
   printf("in order to disconnect...\n");
   printf("\t - ctrl+c to disconnect and let others know + mark it in chatlog history\n");
   printf("\t - ctrl+/ to disconnect you without letting other users know\n");

  int fd, listen_socket, maxfd, client;
  //this is the fd_set that will be modified by select
  fd_set read_fds;
  //this is the full fd_set to keep a stable record
  fd_set clients;
  char buffer[BUFFER_SIZE];

  //make sure clients is totally empty
  FD_ZERO( &clients );
  listen_socket = server_setup();
  printf("listen socket: %d\n", listen_socket);

  //add the listening socket to the master set
  FD_SET(listen_socket, &clients);
  //we have to keep track of the largest file descriptor for select
  //currently, listen_socket is the only, and therefore largest
  maxfd = listen_socket;


  while (1) {
    //must reset read_fds after each time you process select
    read_fds = clients;

    /*
      magic time!
      the first argument is 1 more than the largest file descriptor
      select  will be looking at
      This will block until at least one of the descriptors in read_fds
      is ready.
      Returns the number of fds that are ready.
      read_fds will contain only the descriptors that are ready.
    */
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
           //printf("ready fd: %d\n", fd);
          //if the socket got closed, select will trigger
          //if read returns 0 bytes, then we should close the
          //connection, otherwise process it.
          if (read(fd, buffer, sizeof(buffer)) ) {
             int id = fd-3;
             char temp[BUFFER_SIZE];
             sprintf(temp, "||user%d||: '", id);
             //printf("%s", userid);

             strcat(temp, buffer);
             printf("%s\n", temp);

             //  SAVE TEMP IN SOME FILE HERE FOR history
             //CONVERT INFOMRATION INTO A STRUCT TO BE STORED
            for(i = 0; i <= maxfd; i++) {
              //sending message to everyone except listen and ourself
              if (FD_ISSET(i, &clients)) {
                if (i != listen_socket && i != fd) {
                   //*strchr(temp, '\n') = 0;
                   //write(i, buffer, sizeof(buffer));
                   write(i, temp, sizeof(temp));
                }
              }
            }
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
  close(listen_socket);
  return 0;
}
