all:server client

server:server.c readn_writen.c
	gcc $^ -o $@

client:client.c readn_writen.c
	gcc $^ -o $@

.PHONY:clean

clean:
	rm -f server
	rm -f client
