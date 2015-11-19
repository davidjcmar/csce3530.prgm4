main: server.c client.c pass.c
	gcc server.c pass.c -o server
	gcc client.c pass.c -o client