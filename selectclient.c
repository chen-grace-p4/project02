
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

           // open chat.log for reading
           FILE *read_call;
           read_call = fopen (CHATLOG, "r");
           if (read_call == NULL) {
             fprintf(stderr, "\nError opening %s\n\n", CHATLOG);
             exit(1);
           }

           printf("\033[A\r==================================================================================\n");
           printf("[CHATLOG]\n\n");

           // struct - stores userid, time, and message
           struct log history;

           // reads and prints the chatlog
           while (fread (&history, sizeof(history), 1, read_call)) {

             // char chatlog_time[80];
             // strftime(chatlog_time, sizeof(chatlog_time), "%a %Y-%m-%d %H:%M:%S %Z", &history.time);


             time_t now;
             struct tm ts;
             time(&now);
             ts = *localtime(&now);
             char chatlog_time[80];
             // strftime(chatlog_time, sizeof(chatlog_time), "%a %Y-%m-%d %H:%M:%S %Z", &history.time);

             // printf("[ %s ] ", chatlog_time);

             char *foo = asctime(&history.time);
             foo[strlen(foo) - 1] = 0;

             printf("[ %s ] ", foo);



             sprintf(temp, "user%d", history.userid);
             printf("[ %s ] ", temp);

             printf("[ %s]\n", history.message);
           }


           printf("==================================================================================\n");
           // time_t now;
           // struct tm ts;
           // time(&now);
           // ts = *localtime(&now);
           // char readable[80];
           // strftime(readable, sizeof(readable), "%a %Y-%m-%d %H:%M:%S %Z", &ts);
           // printf("Current Time: %s\n", readable);
           // printf("=========================================\n");

           fclose(read_call);
           // close(open_call);


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
