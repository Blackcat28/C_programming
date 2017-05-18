CFLAGS=-Wall -ggdb3 -pedantic
CC=gcc


all: shm_fork_client_server

shm_fork_client_server: shm_fork_client_server.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	find . -maxdepth 1 -type f -executable -delete

