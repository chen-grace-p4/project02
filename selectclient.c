
#include "networking.h"

int main() {
   struct timeval tv;
   fd_set read_fds;
   fd_set full_fds;

   FD_ZERO(&read_fds);
   FD_SET(STDIN, &full_fds);

   int server;
   server = client_setup(TEST_IP);
   char buffer[BUFFER_SIZE];

   while (1) {
      read_fds = full_fds;
      //1.5 seconds
      tv.tv_sec = 1;
      tv.tv_usec = 500000;
      //printf("Input: ");
      int i = select(STDIN+1, &read_fds, NULL, NULL, &tv);
      //error_check_sock(i, "select", STDIN);
      //printf("select ready: %d\n", i);


      //something is entered on stdin
      if (FD_ISSET(STDIN, &read_fds)) {
         //read(STDIN, buffer, sizeof(buffer));
         fgets(buffer, sizeof(buffer), stdin);
         //printf("your input: %s", buffer);
         *strchr(buffer, '\n') = 0;

         char temp[BUFFER_SIZE];
         strcpy(temp, buffer);
         //char *temp = buffer;

         //separate buffer into array and check first word for command
         char * first = strtok(buffer, " ");
         //char * first = strsep(temp, " ");
         if (strcasecmp(first, "-message") == 0 ||strcasecmp(first, "-m") == 0  ) {
            //IF THERES EXTRA TIME, FIND WAY TO TRIM WHITE SPACE AT THE END
            char temp2[BUFFER_SIZE] = "";
            first = strtok(NULL, " ");
            while (first != NULL) {
               strcat(temp2, first);
               strcat(temp2, " ");
               first = strtok(NULL," ");
            }
            printf("\033[A\r||me||: ' %s'\n", temp2);
            write(server, temp2, sizeof(temp2));
         }
         else if (strcasecmp(first, "-history") == 0 ||strcasecmp(first, "-h") == 0  ) {

           int open_call = open(CHATLOG, O_RDONLY, 0);

           struct log history;

           /*** open the accounts file ***/
           FILE *read_call = fopen (CHATLOG, "r");
           // if (infile == NULL) {
           //   fprintf(stderr, "\nError opening %s\n\n", CHATLOG);
           //   exit(1);
           // }

           printf("Chatlog Opened!!!!\n");

           while (fread(&history, sizeof(struct log), 1, read_call)) {

             sprintf(temp, "user%d", history.userid);
             printf("[%s] ", temp);

           }

           // while (fread (&history, sizeof(struct log), 1, infile)) {

             // sprintf(temp, "user%d", history.userid);
             // printf("[%s] ", temp);

             // char chatlog_time[80];
             // strftime(chatlog_time, sizeof(chatlog_time), "%a %Y-%m-%d %H:%M:%S %Z", &history.time);
             // printf("%s\n", chatlog_time);


             // printf ("Name = %10s %10s   Acct Num = %8d   Balance = %8.2f\n", input.fname, input.lname, input.acct_num, input.acct_balance);
           // }


           time_t now;
           struct tm ts;
           time(&now);
           ts = *localtime(&now);
           char readable[80];
           strftime(readable, sizeof(readable), "%a %Y-%m-%d %H:%M:%S %Z", &ts);
           printf("Now: %s\n", readable);

           close(open_call);


            //CLIENT FINDS INFORMATION ABOUT HISTROY FILE AND OPENS THE FILE TO VIEW
            //clear terminal
            //somehow display data of history.csv and let users scroll through it...?
         }
         else {
            printf("///ALERT///\n");
            printf("Please enter valid command:\n");
            printf("\t '-m yourmessagehere' OR '-message yourmessagehere' to send a regular message.\n");
            printf("\t '-h' OR '-history' to view chatlog history.\n");
            printf("\t ctrl+c to disconnect and let others know and mark in chatlog.\n");
            printf("\t ctrl+/ to disconnect without letting others know.\n");
            printf("///ALERT///\n");
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
       		//printf("||received||: -%s-\n", buffer);
            printf("%s'\n", buffer);
         }
      }
   }
   return 0;
}
