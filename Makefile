CC = cc
CFLAGS = -Wall -Wextra -g

objects = src/main.o src/system.o src/auth.o src/db.o
atm: $(objects)
	$(CC) -o atm $(objects) -lcrypto -lsqlite3

%.o: %.c src/header.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(objects) atm