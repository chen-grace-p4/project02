
#include "networking.h"

static void signal_catcher(int signal) {
   if (signal == SIGINT) {
      //apending message to file noting the program exited...

      // printf("\n");

      printf("\033[0;31m"); // RED
      printf("\033[A\r\n==============================================================================================\n");
      printf("[ SERVER CLOSING... ]\n");
      printf("\033[A\r\n==============================================================================================\n");
      printf("\033[0m"); // DEFAULT
      printf("\n");

      add_activity(7);

      //exit with "0" as the status means program successfully terminated
      //without error
      exit(0);
   }
}

int main() {
  signal(SIGINT, signal_catcher);

  printf("\033[0;33m"); // YELLOW
  printf("\033[A\r\n==============================================================================================\n");
  // printf("\033[0m"); // DEFAULT

  printf("[ CLIENT COMMANDS ]\n");

  printf("\t[ -m YOUR_MESSAGE_HERE ] or [ -message YOUR_MESSAGE_HERE ] to send a message.\n");
  printf("\t[ -c ] or [ -chatlog ] to view the CHATLOG.\n");
  printf("\t[ -a ] or [ -activitylog ] to view the ACTIVITYLOG.\n");
  printf("\t[ ctrl+c ] to disconnect from the server.\n");

  // printf("\033[0;33m"); // YELLOW
  printf("\033[A\r\n==============================================================================================\n");
  printf("\033[0m"); // DEFAULT
  printf("\n");

  // printf("INSTRUCTIONS FOR CLIENTS:\n");
  // printf("\t - ideally, all clients should be connected at the same time to see all messages.\n");
  // printf("\t  clients that connect later on can only view past messages in the chat log.\n");
  // printf("\n");
  // printf("Client commands...\n");
  // printf("\t - '-m yourmessagehere' OR '-message yourmessagehere' to send a regular message.\n");
  // printf("\t - '-c' OR '-chatlog' to view chatlog.\n");
  // printf("\t - '-a' OR '-activitylog' to view activitylog.\n");
  // printf("\t - ctrl+c to disconnect from server.\n");

  add_activity(6);

  int fd, listen_socket, maxfd, client;
  //this is the fd_set that will be modified by select
  fd_set read_fds;
  //this is the full fd_set to keep a stable record
  fd_set clients;
  char buffer[BUFFER_SIZE];

  //make sure clients is totally empty
  FD_ZERO( &clients );
  listen_socket = server_setup();
  // printf("listen socket: %d\n", listen_socket);

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
    // printf("select ready: %d\n", i);

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

          // ACTIVITY LOG ============================================================================================================

          add_activity(1);

          // FILE *open_call;
          // open_call = fopen(ACTIVITYLOG, "a");
          // if (open_call == NULL) {
          //   fprintf(stderr, "\nError opening %s\n\n", ACTIVITYLOG);
          //   exit(1);
          // }
          //
          // // file size before writing
          // int activitylog_size_before = file_size(ACTIVITYLOG);
          //
          // // struct
          // struct activitylog connect;
          //
          // // userid
          // int id = client-3;
          // connect.userid = id;
          // char temp[BUFFER_SIZE];
          // sprintf(temp, "user%d", id); // temp = user1, user2, etc.
          // printf("[ %s ] ", temp);
          //
          // // activity
          // /*
          //      1 - CONNECT
          //      2 - DISCONNECT
          //      3 - SENT A MESSAGE
          //      4 - VIEWED THE CHATLOG
          //      5 - VIEWED THE ACTIVITYLOG
          // */
          // connect.activity = 1;
          // if      (connect.activity == 1) printf("[ connected ]");
          // else if (connect.activity == 2) printf("[ disconnected ]");
          // else if (connect.activity == 3) printf("[ sent a message ]");
          // else if (connect.activity == 4) printf("[ viewed the chatlog ]");
          // else if (connect.activity == 5) printf("[ viewed the activitylog ]");
          //
          // // time
          // time_t now;
          // struct tm ts;
          // time(&now);
          // ts = *localtime(&now);
          // connect.time = ts;
          //
          // char *foo = asctime(&connect.time);
          // foo[strlen(foo) - 1] = 0;
          //
          // printf("\n          ");
          // printf("[ %s ] ", foo);
          //
          // // write to activity
          // fwrite(&connect, sizeof(connect), 1, open_call);
          //
          // // close
          // fclose(open_call);
          //
          // printf("\n          ");
          // printf("[ %lu bytes written to %s ] ", file_size(ACTIVITYLOG) - activitylog_size_before, ACTIVITYLOG);
          // printf("[ size of %s: %lu bytes ]\n", ACTIVITYLOG, file_size(ACTIVITYLOG));

          // ACTIVITY LOG ============================================================================================================

        }//new connections

        //listen socket not ready so existing client
        else {
          //printf("ready fd: %d\n", fd);
          //if the socket got closed, select will trigger
          //if read returns 0 bytes, then we should close the
          //connection, otherwise process it.
          if (read(fd, buffer, sizeof(buffer)) ) {

            // printf("\n");

             int id = fd-3;
             char temp[BUFFER_SIZE];
             //sprintf(temp, "||user%d||: ' ", id);
             sprintf(temp, "[user%d]: ", id);

             // CHAT LOG ================================================================================================================

             FILE *open_call;
             open_call = fopen(CHATLOG, "a");
             if (open_call == NULL) {
               fprintf(stderr, "\nError opening %s\n\n", CHATLOG);
               exit(1);
             }

             // file size before writing
             int chatlog_size_before = file_size(CHATLOG);

             // struct
             struct chatlog store;

             // userid
             store.userid = id;

             char temp2[BUFFER_SIZE];
             sprintf(temp2, "user%d", store.userid);
             printf("[ %s ] ", temp2);

             // message
             strcpy(store.message, buffer);
             printf("[ %s]", store.message);


             // time
             time_t now;
             struct tm ts;
             time(&now);
             ts = *localtime(&now);
             store.time = ts;

             char *foo = asctime(&store.time);
             foo[strlen(foo) - 1] = 0;

             printf("\n          ");
             printf("[ %s ] ", foo);

             // write to chatlog
             fwrite(&store, sizeof(store), 1, open_call);

             // close
             fclose(open_call);

             // printing out updates about chat.log
             printf("\n          ");
             printf("[ %lu bytes written to %s ] ", file_size(CHATLOG) - chatlog_size_before, CHATLOG);
             printf("[ size of %s: %lu bytes ]\n\n", CHATLOG, file_size(CHATLOG));

             // CHAT LOG ================================================================================================================



             // ACTIVITY LOG ============================================================================================================


             add_activity(3);

             // FILE *open_call_activitylog;
             // open_call_activitylog = fopen(ACTIVITYLOG, "a");
             // if (open_call_activitylog == NULL) {
             //   fprintf(stderr, "\nError opening %s\n\n", ACTIVITYLOG);
             //   exit(1);
             // }
             //
             // // file size before writing
             // int activitylog_size_before = file_size(ACTIVITYLOG);
             //
             // // struct
             // struct activitylog connect;
             //
             // // userid
             // connect.userid = id;
             // printf("[ %s ] ", temp2);
             //
             // // activity
             // /*
             //      1 - CONNECT
             //      2 - DISCONNECT
             //      3 - SENT A MESSAGE
             //      4 - VIEWED THE CHATLOG
             //      5 - VIEWED THE ACTIVITYLOG
             // */
             // connect.activity = 3;
             // if      (connect.activity == 1) printf("[ connected ]");
             // else if (connect.activity == 2) printf("[ disconnected ]");
             // else if (connect.activity == 3) printf("[ sent a message ]");
             // else if (connect.activity == 4) printf("[ viewed the chatlog ]");
             // else if (connect.activity == 5) printf("[ viewed the activitylog ]");
             //
             // // time
             // time_t now_activitylog;
             // struct tm ts_activitylog;
             // time(&now_activitylog);
             // ts_activitylog = *localtime(&now_activitylog);
             // connect.time = ts_activitylog;
             //
             // char *fox = asctime(&connect.time);
             // fox[strlen(fox) - 1] = 0;
             //
             // printf("\n          ");
             // printf("[ %s ] ", fox);
             //
             // // write to activity
             // fwrite(&connect, sizeof(connect), 1, open_call);
             //
             // // close
             // fclose(open_call_activitylog);
             //
             // printf("\n          ");
             // printf("[ %lu bytes written to %s ] ", file_size(ACTIVITYLOG) - activitylog_size_before, ACTIVITYLOG);
             // printf("[ size of %s: %lu bytes ]\n", ACTIVITYLOG, file_size(ACTIVITYLOG));


             // ACTIVITY LOG ============================================================================================================






             strcat(temp, buffer); // !!!****** DO NOT DELETE VERY VERY VERY IMPORTANT ******!!!
             // printf("%s\n", temp);

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
  // close(create_call);

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

  // printf("\n");

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
  */
  connect.activity = type;
  if      (connect.activity == 1) printf("[ user connected ]");
  else if (connect.activity == 2) printf("[ user disconnected ]");
  else if (connect.activity == 3) printf("[ user sent a message ]");
  else if (connect.activity == 4) printf("[ user viewed the chatlog ]");
  else if (connect.activity == 5) printf("[ user viewed the activitylog ]");
  else if (connect.activity == 6) printf("[ server started ]");
  else if (connect.activity == 7) printf("[ server closed ]");

  // time
  time_t now;
  struct tm ts;
  time(&now);
  ts = *localtime(&now);
  connect.time = ts;

  char *foo = asctime(&connect.time);
  foo[strlen(foo) - 1] = 0;

  printf("\n          ");
  printf("[ %s ]", foo);

  // write to activity
  fwrite(&connect, sizeof(connect), 1, open_call_activitylog);

  // close
  fclose(open_call_activitylog);

  printf("\n          ");
  printf("[ %lu bytes written to %s ] ", file_size(ACTIVITYLOG) - activitylog_size_before, ACTIVITYLOG);
  printf("[ size of %s: %lu bytes ]\n\n", ACTIVITYLOG, file_size(ACTIVITYLOG));
}
