
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

         else if (strcasecmp(first, "-chatlog") == 0 ||strcasecmp(first, "-c") == 0  ) {

           // open chat.log for reading
           FILE *read_call;
           read_call = fopen (CHATLOG, "r");
           if (read_call == NULL) {
             fprintf(stderr, "\nError Opening %s\n\n", CHATLOG);
             exit(1);
           }

           // erases -c/-chatlog
           printf("\033[0;33m"); //Set the text to the color red
           printf("\033[A\r==================================================================================\n");
           printf("\033[0m"); //Resets the text to default color

           // heading
           // includes size of chatlog and uses that number to calculate the total number of messages
           printf("\033[0;35m"); //Set the text to the color red
           printf("[ CHATLOG ]  ");
           printf("\033[0m"); //Resets the text to default color


           printf("[ CHATLOG SIZE: %lu BYTES ]  [ TOTAL NUMBER OF MESSAGES: %lu ]\n\n", file_size(CHATLOG), ((file_size(CHATLOG) / sizeof(struct chatlog))));

           printf("\033[0;35m"); //Set the text to the color red
           printf(" [ TOTAL NUMBER OF MESSAGES: %lu ] ");
           printf("\033[0m"); //Resets the text to default color





           // struct - stores userid, time, and message
           struct chatlog log;

           // reads and prints the chatlog
           while (fread (&log, sizeof(log), 1, read_call)) {

             // [TIME]
             char *foo = asctime(&log.time);
             foo[strlen(foo) - 1] = '\0';
             printf("[ %s ] ", foo);

             // [USERID]
             sprintf(temp, "user%d", log.userid);
             printf("[ %s ] ", temp);

             // [MESSAGE]
             printf("[ %s]\n", log.message);

           }

           printf("\033[0;33m"); //Set the text to the color red
           printf("==================================================================================\n");
           printf("\033[0m"); //Resets the text to default color

           fclose(read_call);
         }

         else if (strcasecmp(first, "-activitylog") == 0 ||strcasecmp(first, "-a") == 0  ) {

           // open activity.log for reading
           FILE *read_call;
           read_call = fopen(ACTIVITYLOG, "r");
           if (read_call == NULL) {
             fprintf(stderr, "\nError Opening %s\n\n", ACTIVITYLOG);
             exit(1);
           }

           // erases -h/-history
           printf("\033[A\r==================================================================================\n");

           // heading
           // includes size of chatlog and uses that number to calculate the total number of entries
           printf("[ ACTIVITYLOG ] [ ACTIVITYLOG SIZE: %lu BYTES ] [ TOTAL NUMBER OF ENTRIES: %lu ]\n\n", file_size(ACTIVITYLOG), ((file_size(ACTIVITYLOG) / sizeof(struct activitylog))));

           // struct - stores userid, time, and message
           struct activitylog log;

           // reads and prints the activitylog
           while (fread (&log, sizeof(log), 1, read_call)) {

             // [ TIME ]
             char *foo = asctime(&log.time);
             foo[strlen(foo) - 1] = 0;
             printf("[ %s ] ", foo);

             // [ USERID ]
             sprintf(temp, "user%d", log.userid);
             printf("[ %s ] ", temp);

             // [ ACTIVITY ]
             if      (log.activity == 1) printf("[ connected ]");
             else if (log.activity == 2) printf("[ disconnected ]");
             else if (log.activity == 3) printf("[ sent a message ]");
             else if (log.activity == 4) printf("[ viewed the chatlog ]");
             else if (log.activity == 5) printf("[ viewed the activitylog ]");
             printf("\n");
           }

           printf("==================================================================================\n");
           fclose(read_call);
         }

         else {
            printf("///ALERT///\n");
            printf("Please enter valid command:\n");
            printf("\t '-m yourmessagehere' OR '-message yourmessagehere' to send a regular message.\n");
            printf("\t '-c' OR '-chatlog' to view chatlog.\n");
            printf("\t '-a' OR '-activitylog' to view activitylog.\n");
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


// returns file size in bytes
unsigned long file_size(char *file) {
  struct stat sb;
  stat(file, &sb);
  return sb.st_size;
}
