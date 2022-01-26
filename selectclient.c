
#include "networking.h"

static void signal_catcher(int signal) {
   if (signal == SIGINT) {
      //apending message to file noting the program exited...
      printf("\nDisconnecting...\n");
      add_activity(2);

      //exit with "0" as the status means program successfully terminated
      //without error
      exit(0);
   }
}

int main() {
   signal(SIGINT, signal_catcher);
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
            printf("\033[0;35m"); // CYAN
            printf("\033[A\rme: %s\n", temp2);
            printf("\033[0m"); // DEFAULT
            write(server, temp2, sizeof(temp2));
         }

         else if (strcasecmp(first, "-chatlog") == 0 || strcasecmp(first, "-c") == 0) {

           // erases -c/-chatlog
           printf("\033[A\r       \n");

           // open chat.log for reading
           FILE *read_call;
           read_call = fopen (CHATLOG, "r");
           if (read_call == NULL) {
             fprintf(stderr, "\nError Opening %s\n\n", CHATLOG);
             exit(1);
           }

           printf("\033[0;33m"); // YELLOW
           printf("\033[A\r\n==================================================================================\n");
           printf("\033[0m"); // DEFAULT

           add_activity(4);


           // heading
           // includes size of chatlog and uses that number to calculate the total number of messages
           printf("\033[0;36m"); // CYAN
           printf("[ CHATLOG ] ");

           printf("\033[0m"); // DEFAULT
           printf(" [ CHATLOG SIZE: %lu BYTES ] ", file_size(CHATLOG));

           printf("\033[0;36m"); // CYAN
           printf(" [ TOTAL NUMBER OF MESSAGES: %lu ]\n\n", ((file_size(CHATLOG) / sizeof(struct chatlog))));

           printf("\033[0m"); // DEFAULT

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

           printf("\033[0;33m"); // YELLOW
           printf("==================================================================================\n\n");
           printf("\033[0m"); // DEFAULT

           fclose(read_call);
         }

         else if (strcasecmp(first, "-activitylog") == 0 || strcasecmp(first, "-a") == 0) {

           // erases -a/-activitylog
           printf("\033[A\r       \n");

           // open activity.log for reading
           FILE *read_call;
           read_call = fopen(ACTIVITYLOG, "r");
           if (read_call == NULL) {
             fprintf(stderr, "\nError Opening %s\n\n", ACTIVITYLOG);
             exit(1);
           }

           printf("\033[0;36m"); // CYAN
           printf("\033[A\r\n==================================================================================\n");
           printf("\033[0m"); // DEFAULT

           add_activity(5);

           // heading
           // includes size of chatlog and uses that number to calculate the total number of entries
           printf("\033[0;33m"); // YELLOW
           printf("[ ACTIVITYLOG ] ");

           printf("\033[0m"); // DEFAULT
           printf(" [ ACTIVITYLOG SIZE: %lu BYTES ] ", file_size(ACTIVITYLOG));

           printf("\033[0;33m"); // YELLOW
           printf(" [ TOTAL NUMBER OF ENTRIES: %lu ]\n\n", ((file_size(ACTIVITYLOG) / sizeof(struct activitylog))));

           printf("\033[0m"); // DEFAULT

           // struct - stores userid, time, and message
           struct activitylog log;

           // reads and prints the activitylog
           while (fread (&log, sizeof(log), 1, read_call)) {

             // [ TIME ]
             char *foo = asctime(&log.time);
             foo[strlen(foo) - 1] = 0;
             printf("[ %s ] ", foo);

             // [ USERID ]
             // sprintf(temp, "user%d", log.userid);
             // printf("[ %s ] ", temp);

             // [ ACTIVITY ]
             if      (log.activity == 1) printf("[ user connected ]");
             else if (log.activity == 2) printf("[ user disconnected ]");
             else if (log.activity == 3) printf("[ user sent a message ]");
             else if (log.activity == 4) printf("[ user viewed the chatlog ]");
             else if (log.activity == 5) printf("[ user viewed the activitylog ]");
             else if (log.activity == 6) printf("[ server started ]");
             else if (log.activity == 7) printf("[ server closed ]");
             printf("\n");
           }

           printf("\033[0;36m"); // CYAN
           printf("==================================================================================\n\n");
           printf("\033[0m"); // DEFAULT

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

// void add_activity(int id, int type) {
void add_activity(int type) {
  FILE *open_call_activitylog;
  open_call_activitylog = fopen(ACTIVITYLOG, "a");
  if (open_call_activitylog == NULL) {
    fprintf(stderr, "\nError opening %s\n\n", ACTIVITYLOG);
    exit(1);
  }

  // file size before writing
  int activitylog_size_before = file_size(ACTIVITYLOG);

  // struct
  struct activitylog connect;

  // userid
  // connect.userid = id;

  // char temp[BUFFER_SIZE];
  // sprintf(temp, "user%d", id); // temp = user1, user2, etc.
  // printf("[ %s ] ", temp);

  // activity
  /*
       1 - CONNECT
       2 - DISCONNECT
       3 - SENT A MESSAGE
       4 - VIEWED THE CHATLOG
       5 - VIEWED THE ACTIVITYLOG
       6 - SERVER STARTED
  */
  connect.activity = type;
  // if      (connect.activity == 1) printf("[ user connected ]");
  // else if (connect.activity == 2) printf("[ user disconnected ]");
  // else if (connect.activity == 3) printf("[ user sent a message ]");
  // else if (connect.activity == 4) printf("[ user viewed the chatlog ]");
  // else if (connect.activity == 5) printf("[ user viewed the activitylog ]");

  // time
  time_t now;
  struct tm ts;
  time(&now);
  ts = *localtime(&now);
  connect.time = ts;

  // char *foo = asctime(&connect.time);
  // foo[strlen(foo) - 1] = 0;
  //
  // printf("\n          ");
  // printf("[ %s ] ", foo);

  // write to activity
  fwrite(&connect, sizeof(connect), 1, open_call_activitylog);

  // close
  fclose(open_call_activitylog);

  // printf("\n          ");
  // printf("[ %lu bytes written to %s ] ", file_size(ACTIVITYLOG) - activitylog_size_before, ACTIVITYLOG);
  // printf("[ size of %s: %lu bytes ]\n", ACTIVITYLOG, file_size(ACTIVITYLOG));
}
