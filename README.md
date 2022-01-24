# Systems Final Project - "Disaccord"
**Members**: Grace Chen, Kyle Li <br>
**Period**: 4 <br>
**Project Statement**: We will create a very basic messaging server somewhat inspired by Discord or Slack. <br>

## Broad Description of Project
A terminal-based messaging server loosely inspired by Discord. Clients connect to a server to send things to each another. They can send messages, files, and "decorate" their text messages using keyboard shortcuts. 

## Description of User Interface (How to use our project):
First, start up the server after running make by running ./server. <br>
On other terminal windows, run ./client on each window to connect multiple clients to the server. <br> 
On the client-end, they can use the flag -m or -message (ex. "-m hi hows your day" to send hi hows your day) to send a regular text message. <br>
They can also use the flag -f or -file (ex. "-f text.txt" to send text.txt) to send files to the other clients. <br>
Clients can use ctrl+b to "bold" their words (** around their words) or ctrl+i to italicize their words (* around their words). 

## Description of Technical Design with omits/new parts added:
* Sockets (Grace)
  * connecting users(clients) to a server where they send messages to each other
  * server returns a "new message" to every client everytime a client sends something to the server so that the client end can see all the messages
  * (OMITTED)"subserver" or channels also could be an entirely new server itself and clients will simply connect to multiple servers at once
  * (NEW) server uses select to navigate through multiple clients and clients use select to navigate between STDIN and new messages from the server (other clients)
    * (NEW) <sys/ioctl.h> is included so ioctl is used to detect whether or not the socket has something new to read (a new message)
  * (NEW) clients are identified by usernames based on their file descriptor number

* Working with files (Kyle)
  * (OMITTED)send files or images over the server so that other clients can access/see them
  * (NEW) saving text messages into a file and allowing clients to access that file to see past logs of messages

* Finding information about files (Kyle)
  * display information about files such as size, file type, and a preview of files
  * having a preview of a file will require the use of READ
  * at some point there could be a way to change a file using WRITE by a client and send the changed file back over the server 

* Signals (Grace)
  * (OMITTED)shortcuts to bold or italicize words 
  * (NEW) to log if a user disconnected or reconnected 

* (OMITTED) Layouting the GUI (Kyle)
  *  use gtk in c
  *  still need to research

## Some known bugs or weird things:
* When a client recieves a message from another client while they're still typing a message, it will look a little wonky on the terminal but what they have already typed before receiving a new message is included in the next message they send. 

## Features we weren't able to implement (reiterated from above):
* subservers or channels
* the gtk gui 
* without the gui, the bold and italicize won't appear directly on the text but will still appear as asterisks around the text

## Timeline:
* 1/13 - Have a basic terminal based messaging server made. Clients will be able to connect and messages they send will be displayed both on their end and on the server.
* 1/14 - Make it so user commands are recognized. For example, "message something" will send a regular message. 
* 1/15 - See if creating a subserver is possible so that users can join. Figure out if this can be accomplished with either making an entirely new server or somehow connecting servers with one another so that theres a "subserver". Add a user command of "create channel" to create a subserver
* 1/17 - Complete sending files over the server to other clients and displaying information about files to users
* 1/19 - Use the GUI to complete the signals component
* 1/20 to 1/21 - If there's still time, continue to polish everything and make it so that the clients also have a GUI instead of just using the terminal 
