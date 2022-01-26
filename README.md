# Systems Final Project - "Disaccord"
**Members**: Grace Chen, Kyle Li <br>
**Period**: 4 <br>
**Project Statement**: A very basic messaging server somewhat inspired by Discord or Slack. <br>

## Broad Description of Project
A terminal-based messaging server loosely inspired by Discord. A client (the user) connects to a server, where they can send text messages to other clients (users) that are also connected to the server. A CHATLOG records all of these messages, even if all users disconnect and if the server closes. An ACTIVITYLOG records all "activities" performed in the chat server (a list of what activities are recorded can be found in the Description of Technical Design).

## Description of User Interface (How to use our project):
First, start up the server after running make by running ./server. <br> <br>
On other terminal windows, run ./client on each window to connect multiple clients to the server. It is best to connect clients all at once since previously sent messages will not be sent to new clients (but can be viewed in the activitylog, see below). <br> <br>
On the client-end...
* use the flag -m or -message (ex. "-m hi hows your day" to send hi hows your day) to send a regular text message. <br>
* use flag -c or -chatlog to view a log of all messages sent, who sent them, and the time they were sent. <br>
* use flag -a or -activitylog to view a log of activities (ex. client disconnected and more) and the time they were performed. <br>

Instructions are shown on the teminal in server and client as well (if an unrecognized flag or no flag is used in the client, a 'help' guide will pop up). <br>
<br>
Also note: the activity log will continue tracking activity even after server and clients disconnect and reconnect again to log different sessions. It will only clear everytime you run make clean. 
<br>

## Description of Technical Design:<br>NOTE: Sections omitted from the original proposal are marked with "(OMITTED)." All new additions are marked with (NEW).
* **Sockets (Grace)**
  * connecting users(clients) to a server where they send messages to each other
  * (OMITTED)"subserver" or channels also could be an entirely new server itself and clients will simply connect to multiple servers at once
  * (NEW) server uses select to navigate through multiple clients and clients use select to navigate between STDIN and new messages from the server (other clients)
       * (NEW) <sys/ioctl.h> is included so ioctl is used to detect whether or not the socket has something new to read (a new message)
  * (NEW) clients are identified by usernames based on their file descriptor number

* **Working with files (Kyle)**
  * (OMITTED) send files or images over the server so that other clients can access/see them
  * (NEW) CHATLOG (chat.log): Saves all messages sent by the users into a file. All users/clients can access the CHATLOG using -c or -chatlog.
  * (NEW) ACTIVITYLOG (activity.log): Saves all "activities" performed in the chat server. UserIDs are NOT recorded in the file, thus making this an "anonymous" ACTIVITYLOG. All users/clients can access the ACTIVITYLOG using -a or -activitylog.
    * SEVEN different "activities" are recorded in the ACTIVITYLOG:
      * 1: User connected
      * 2: User disconnected
      * 3: User sent a message
      * 4: User viewed the CHATLOG
      * 5: User viewed the ACTIVITYLOG
      * 6: Server started
      * 7: Server closed

* **Finding information about files (Kyle)**
  * (OMITTED) display information about files such as size, file type, and a preview of files
  * (OMITTED) having a preview of a file will require the use of READ
  * (OMITTED) at some point there could be a way to change a file using WRITE by a client and send the changed file back over the server 

* **Signals (Grace)**
  * (NEW) to log if a user/server disconnect
    * ctrl+c in the server or client disconnects them and also logs their disconnect in the activity log
    * catches SIGINT, prints a disconnecting message, then adds activity to log

* (OMITTED) Layouting the GUI (Kyle)

## Some known bugs or weird things:
* When a client recieves a message from another client while they're still typing a message, it will look a little wonky on the terminal but what they have already typed before receiving a new message is included in the next message they send.
* If the server closes while users are still connected, the users are NOT automatically disconnected. In our testing, two calls of -m/-message by the user leads to the client breaking. This is different from the user manually disconnecting with ctrl+c. Therefore, this "breaking" is NOT recorded as the user disconnecting in the ACTIVITYLOG.

## Features we didn't implement that were in our proposal (reiterated from above):
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
