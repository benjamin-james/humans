CC = gcc
CFLAGS += -W -Wall -Werror -g -O2

DESTDIR = /
PREFIX = /usr/local

LOCATION = $(addprefix $(DESTDIR), $(addprefix $(PREFIX), /bin))

all: humans

humans: humans.c
	$(CC) $(CFLAGS) $< -o $@
clean:
	$(RM) humans
install: humans
	mkdir -p $(LOCATION)
	install -c humans $(LOCATION)
