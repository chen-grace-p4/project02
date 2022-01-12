# Systems Final Project - Disaccord
**Members**: Grace Chen, Kyle Li <br>
**Period**: 4 <br>
**Project Statement**: Basic Chatting Server Similar To Discord/RocketChat <br>
## Description of User Interface:
At the most basic point, users will interact with the server using the termainal. A server will start up. Then, users connected to it will be prompted for their username and given instructions. Users will then be able to send things based on commands they put. "message ..." will send a normal text message and have it displayed on the server for all uesrs to see. "upload ....txt" will upload the designated file or image to the server. 

Once the most basic interface is made, there will be a GUI display for the server instead. At this point, the user wil be able to keyboard shortcuts such as Ctrl + B to bold or Ctrl + I to italicize words. If we get to a GUI for the users too, we'll try to make it so Ctrl+C for copy and Ctrl + V for paste works as well, and make the place to enter messages more streamlined (allow the use of the cursor to move the point).

## Description of Technical Design:
* Sockets (Grace)
  * connecting users(clients) to a server where they send messages to each other
  * server returns a "new message" to every client everytime a client sends something to the server so that the client end can see all the messages
  * not sure if server should display all the messages on the server itself
* Working with files (Kyle)
  * send files or images over the server so that other clients can access/see them
* Finding information about files (Kyle)
  * display information about files such as size, file type, and a preview of files
* Signals (Grace)
  * shortcuts to copy and paste, to bold or italicize words 
* Layouting the GUI (Grace + Kyle)
  *  something

## Timeline:
* 1/14 - Have a basic terminal based messaging server made. Clients will be able to connect and messages they send will be displayed on the server.
