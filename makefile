all: client server

# client: testclient.o networking.o
# 	gcc -o client testclient.o networking.o

client: selectclient.o networking.o
	gcc -o client selectclient.o networking.o

server: testserver.o networking.o
	gcc -o server testserver.o networking.o

# testclient.o: testclient.c networking.h
# 	gcc -c testclient.c

selectclient.o: selectclient.c networking.h
	gcc -c selectclient.c

testserver.o: testserver.c networking.h
	gcc -c testserver.c

networking.o: networking.c networking.h
	gcc -c networking.c

clean:
	rm *.o
	rm server
	rm client
	rm chat.log
	rm activity.log
