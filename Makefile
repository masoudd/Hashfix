libs := -lnettle
cflags := -std=c99 -Wall -static

all: hashlist hashfix

hashfix: hashfix.c hashfix.h common.c common.h
	gcc $(cflags)  hashfix.c common.c net.c $(libs) -o hashfix

hashlist: hashlist.c common.c common.h
	gcc $(cflags) hashlist.c common.c $(libs) -o hashlist

debug: hashlist.c common.c common.h hashfix.c
	gcc $(cflags) -g hashlist.c common.c $(libs) -o hashlist_debug
	gcc $(cflags) -g hashfix.c common.c net.c $(libs) -o hashfix_debug

clean:
	rm hashfix hashlist *_debug


.PHONY: all debug clean
