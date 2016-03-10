CC=gcc
CFLAGS=-pthread -I/usr/include/playerctl -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/include/alsa -lplayerctl-1.0 -lgobject-2.0 -lglib-2.0 -lasound -Wall -Wextra

all: i3sl.c
	$(CC) i3sl.c blocks.c -o i3sl $(CFLAGS)

install:
	install i3sl /usr/local/bin/

clean:
	rm i3sl
