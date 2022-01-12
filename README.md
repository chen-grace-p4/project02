# Systems Final Project - "Disaccord"
**Members**: Grace Chen, Kyle Li <br>
**Period**: 4 <br>
**Project Statement**: We will create a very basic messaging server somewhat inspired by Discord or Rocketchat. <br>
## Description of User Interface:
At the most basic level, users will interact with the server using the terminal. A server will start up. Then, users connected to it will be prompted for their username and given instructions. Users will then be able to send things based on commands they put. "message ..." will send a normal text message and have it displayed on the server for all uesrs to see. "upload ....txt" will upload the designated file or image to the server. 

Once the most basic interface is made, there will be a GUI display for the server instead. At this point, the user wil be able to keyboard shortcuts such as Ctrl + B to bold or Ctrl + I to italicize words. If we get to a GUI for the users too, we'll try to make it so Ctrl+C for copy and Ctrl + V for paste works as well, and make the place to enter messages more streamlined (allow the use of the cursor to move the point).

## Description of Technical Design:
* Sockets (Grace)
  * connecting users(clients) to a server where they send messages to each other
  * server returns a "new message" to every client everytime a client sends something to the server so that the client end can see all the messages
  * not sure if server should display all the messages on the server itself
  * "subserver" or channels also could be an entirely new server itself and clients will simply connect to multiple servers at once

* Processes? (Grace)
  * maybe use fork to somehow create a subserver which acts like "channels" in Discord/Rocketchat
   * users will be able to join channels with a command like "join channel=something"

* Working with files (Kyle)
  * send files or images over the server so that other clients can access/see them

* Finding information about files (Kyle)
  * display information about files such as size, file type, and a preview of files
  * having a preview of a file will require the use of READ
  * at some point there could be a way to change a file using WRITE by a client and send the changed file back over the server 

* Signals (Grace)
  * shortcuts to copy and paste, to bold or italicize words 
  * ex. ctrl b to bold, ctrl i to italicize, ctrl c and ctrl v to copy and paste

* Layouting the GUI (Kyle)
  *  use gtk in c
  *  still need to research

## Timeline:
* 1/13 - Have a basic terminal based messaging server made. Clients will be able to connect and messages they send will be displayed both on their end and on the server.
* 1/14 - Make it so user commands are recognized. For example, "message something" will send a regular message. 
* 1/15 - See if creating a subserver is possible so that users can join. Figure out if this can be accomplished with either making an entirely new server or somehow connecting servers with one another so that theres a "subserver". Add a user command of "create channel" to create a subserver
* 1/17 - Complete sending files over the server to other clients and displaying information about files to users
* 1/19 - Use the GUI to complete the signals component
* 1/20 to 1/21 - If there's still time, continue to polish everything and make it so that the clients also have a GUI instead of just using the terminal 
