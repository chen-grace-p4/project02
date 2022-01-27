# 1/13/2022
4:11 PM **Grace**: added basic files from prev assignment for server and multiple clients
<br>

# 1/17/2022
11:37 PM **Grace**: changed server to the select server file given. edited it so that it sends something from one client to every client except itself. client side is still a bit weird when receiving messages (only receiving after sending own message)
<br>

# 1/18/2022
12:23 AM **Grace**: fixed it so that a client can now send multiple messages without receiving a message to other clients. however, client still needs to send something to receive messages correctly.
<br><br>
10:56 AM **Grace**: made it so that program can detect client "commands" in front of a message and made it so when they send update, they get the most recent messages
<br>

# 1/19/2022
10:57 AM **Grace**: realized we need to use select for client side as well for it to work correctly, so changed some comments and plan/setup files for select
<br>

# 1/21/2022
10:57 AM **Grace**: continued setting up files for a selectclient
<br>

# 1/23/2022
7:27 PM **Grace**: completed basic client side using select so that messages can properly be received and sent simultaneously now
<br><br>
9:04 PM **Grace**: added usernames to distinguish between clients on server file and fixed some formatting issues
<br><br>
11:38 PM **Grace**: added proper "flags" for the user and trimmed the flag off the message sent so it looks better
<br>

# 1/24/2022
1:28 AM **Grace**: server displays client-side messages as well for now, updated README to match current project, added instructions in server and client
<br><br>
4:03 PM **Kyle**: server now writes to chatlog (old write)
<br>
5:01 PM **Kyle**: started implementation of fread/fwrite, server can now print updates
<br>

# 1/25/2022
4:45 AM **Kyle**: chatlog in complete working order
<br>
6:56 AM **Kyle**: new formatting for printing, started activitylog
<br>
9:47 AM **Kyle**: added "connected" to activitylog
<br>
10:48 AM **Kyle**: fixed userid
<br>
10:53 AM **Kyle**: testing colored print statements
<br>
12:59 PM **Kyle**: sent a message is now recorded in the activitylog
<br>
1:18 PM **Kyle**: moved writing to activitylog into a helper function, 
<br>

# 1/26/2022
10:24 AM **Kyle**: added view chatlog/activitylog to activitylog, removed userid from activitylog
<br>
10:49 AM **Grace**: completed signals for disconnecting
<br>
1:40 PM **Grace**: updated README to match project now
<br>
2:02 PM **Kyle**: updated README to match project now
<br>
7:46 PM **Grace**: added devlog
<br>
7:57 PM **Grace**: fixed format of user/me display in client, modified format of how messages are shown, fixed instructions
<br>

# 1/27/2022
3:01 AM **Kyle**: fixed instructions, fixed formatting on printed statements, added new coloring
<br>
3:48 AM **Kyle**: updated README
<br>
5:35 AM **Kyle**: updated devlog
