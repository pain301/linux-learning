all:server client

server:server.c wrap.c
	gcc $^ -o $@ -lpthread

client:client.c wrap.c
	gcc $^ -o $@ -lpthread

.PHONY:clean

clean:
	rm -f server
	rm -f client
