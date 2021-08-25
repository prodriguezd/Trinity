CC = gcc
CFLAGS = -Wall -Wextra -lm -lpthread
DEPS = Command.h File.h Server.h Structure.h Utils.h
OBJ = Command.o File.o Server.o Trinity.o Utils.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

trinity: $(OBJ)
	@echo "Working on main"
	$(CC) -o $@ $^ $(CFLAGS)
	@echo "Done!"

.PHONY: clean

clean:
	@echo "Cleaning up"
	rm -f *.o
