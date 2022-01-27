# Systems Final Project
### Disaccord
**Members**: Grace Chen, Kyle Li <br>
**Period**: 4 <br>
**Project Statement**: A very basic messaging server somewhat inspired by Discord or Slack. <br>

## Broad Description of Project
A terminal-based messaging server loosely inspired by Discord. A client (the user) connects to a server, where they can send text messages to other clients (users) that are also connected to the server. A CHATLOG records all of these messages, even if all users disconnect and if the server closes. An ACTIVITYLOG records all "activities" performed in the chat server (a list of what activities are recorded can be found in the Description of Technical Design).

## Instructions on how to use our project:
1. Build the project using `make`.
2. Open at minimum three terminal windows, one for the server and at minimum two for the clients.
   * You can use only one window for the client but you'd just be talking to yourself!
3. To start the server, run `./server`.
4. To connect to the server as a user, run `./client`.
   * You can do this multiple times.
   * Notes about our testing:
     * We have tested our code with up to 9 simultaneous users connected to the server.
     * We have only tested connections on the localhost (all clients are on the same machine).
5. You may use the following commands in `client`:
    * `-m YOUR_MESSAGE_HERE` or `-message YOUR_MESSAGE_HERE` to send a message.
      * Note about our testing:
        * We have NOT tested the character limit of the messages. Most of the messages we used to test our code were under 50 characters, though we assume it can support up to ~1000 characters as that is the size of the string storing the message in the `chatlog` struct.
    * `-c` or `-chatlog` to view the CHATLOG.
      * The CHATLOG stores all messages sent, including who sent the message and the time the message was sent.
    * `-a` or `-activitylog` to view the ACTIVITYLOG.
      * The ACTIVITYLOG stores seven different activities and the time the activity was performed. See more details under "Working with Files."
    * If you incorrectly enter one these commands or simply type something random and click `enter`/`return`, you will be alerted to enter a valid command.
6. If you have better things to do, you may disconnect from the server with `ctrl+c`!
    * You can reconnect to the server as long the server is still running. Data stored in CHATLOG/ACTIVITYLOG will NOT be deleted by disconnecting.
7. If you choose to relieve the server of its duties, you may end the server with `ctrl+c`!
    * You can restart the server again with a call of `./server`. Data stored in CHATLOG/ACTIVITYLOG will NOT be deleted by ending the server.
8. If you would like to wipe the CHATLOG/ACTIVITYLOG, run `make clean`. Then run `make` to rebuild the project and start fresh.

Have fun!


## Description of Technical Design:
### NOTE: Sections omitted from the original proposal are marked with "(OMITTED)." All new additions are marked with (NEW).
* **Sockets (Grace)**
  * connecting users (clients) to a server where they send messages to each other
  * (OMITTED) "subserver" or channels also could be an entirely new server itself and clients will simply connect to multiple servers at once
  * (NEW) server uses select to navigate through multiple clients and clients use select to navigate between STDIN and new messages from the server (other clients)
       * (NEW) <sys/ioctl.h> is included so ioctl is used to detect whether or not the socket has something new to read (a new message)
  * (NEW) clients are identified by usernames based on their file descriptor number

* **Working with Files (Kyle)**
  * (NEW) CHATLOG: Saves all messages sent by the users into a file (chat.log) using the `chatlog` struct. All users/clients can access the CHATLOG using -c or -chatlog.
    * CHATLOG records:
      * the message
      * the user who sent the message
      * the time when the message was sent
  * (NEW) ACTIVITYLOG: Saves all "activities" performed in the chat server into a file (activity.log) using the `activitylog` struct. All users/clients can access the ACTIVITYLOG using -a or -activitylog.
    * SEVEN different activities are recorded in the ACTIVITYLOG:
      * 1: User connected
      * 2: User disconnected
      * 3: User sent a message
      * 4: User viewed the CHATLOG
      * 5: User viewed the ACTIVITYLOG
      * 6: Server started
      * 7: Server closed
  * (NEW) UserIDs are NOT recorded in the ACTIVITYLOG due to some limitations regarding communication between the server and the client. Therefore, the ACTIVITYLOG is "anonymous." Only the activity and when the activity was performed are recorded.
  * (OMITTED) send files or images over the server so that other clients can access/see them

* **Finding Information About Files (Kyle)**
  * (NEW) Finding out the size of `chat.log` and `activity.log`. 
    * This information is used to calculate the number of messages saved in the CHATLOG and the number of entries saved in the ACTIVITYLOG. Since the only things being written to the files are structs of a constant size, dividing the size of the file by the size of the struct will result in the number of messages/entries. There are ways around using the file size to calculate the number of messages/entries, all of which are far less efficient.
  * (NEW) The size of the file and the number of messages/entries in the CHATLOG/ACTIVITYLOG is displayed in the first line of the CHATLOG/ACTIVITYLOG.
  * (OMITTED) display information about files such as size, file type, and a preview of files
  * (OMITTED) having a preview of a file will require the use of READ
  * (OMITTED) at some point there could be a way to change a file using WRITE by a client and send the changed file back over the server
  
* **Signals (Grace)**
  * (NEW) to log if a user/server disconnect
    * ctrl+c in the server or client disconnects them and also logs their disconnect in the activity log
    * catches SIGINT, prints a disconnecting message, then adds activity to log

* (OMITTED) Layouting the GUI (Kyle)

## Regarding our testing (reiterated from the Instructions):
* We have tested our code with up to 9 simultaneous users connected to the server.
* We have only tested connections on the localhost (all clients are on the same machine).
* We have NOT tested the character limit of the messages. Most of the messages we used to test our code were under 50 characters, though we assume it can support up to ~1000 characters as that is the size of the string storing the message in the `chatlog` struct.


## Some known bugs or weird things:
* When a client recieves a message from another client while they're still typing a message, it will look a little wonky on the terminal but what they have already typed before receiving a new message is included in the next message they send.
* If the server closes while users are still connected, the users are NOT automatically disconnected. In our testing, two calls of `-m`/`-message` by the user leads to the client breaking. This is different from the user manually disconnecting with `ctrl+c`. Therefore, this "breaking" is NOT recorded as the user disconnecting in the ACTIVITYLOG.
* The colors for the printed text appear different depending on the terminal being used. We apologize for any readability issues!


## Features we didn't implement that were in our proposal (reiterated from Description of Technical Design):
* subservers or channels
* the gtk gui 
* italicizing and bolding text
* sending files 


## Timeline:
* 1/13 - Have a basic terminal based messaging server made. Clients will be able to connect and messages they send will be displayed both on their end and on the server.
* 1/14 - Make it so user commands are recognized. For example, "message something" will send a regular message. 
* 1/15 - See if creating a subserver is possible so that users can join. Figure out if this can be accomplished with either making an entirely new server or somehow connecting servers with one another so that theres a "subserver". Add a user command of "create channel" to create a subserver
* 1/17 - Complete sending files over the server to other clients and displaying information about files to users
* 1/19 - Use the GUI to complete the signals component
* 1/20 to 1/21 - If there's still time, continue to polish everything and make it so that the clients also have a GUI instead of just using the terminal 
