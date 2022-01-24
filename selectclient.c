
#include "networking.h"

int main() {
   struct timeval tv;
   fd_set read_fds;
   fd_set full_fds;

   tv.tv_sec = 2;
   tv.tv_usec = 500000;

   FD_ZERO(&read_fds);
   FD_SET(STDIN, &full_fds);

   int server;
   server = client_setup(TEST_IP);
   char buffer[BUFFER_SIZE];

   while (1) {
      read_fds = full_fds;
      tv.tv_sec = 1;
      tv.tv_usec = 500000;
      //printf("Input: ");
      int i = select(STDIN+1, &read_fds, NULL, NULL, &tv);
      //error_check_sock(i, "select", STDIN);
      //printf("select ready: %d\n", i);


      //something is entered on stdin
      if (FD_ISSET(STDIN, &read_fds)) {
         read(STDIN, buffer, sizeof(buffer));
         //printf("your input: %s", buffer);
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
           printf("\033[A\rsending: -%s-\n", temp);
           write(server, temp, sizeof(temp));
         }
      }
      //nothing is entered on stdin
      else {
         //printf("Timed out.\n");
         int count;
         ioctl(server, FIONREAD, &count);

         //means server socket has something to send
         if (count != 0) {
            read(server, buffer, sizeof(buffer));
       		printf("received: -%s-\n", buffer);
         }
      }
   }
   return 0;
}
