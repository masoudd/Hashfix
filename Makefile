libs := -lnettle -lcurl
cflags := -std=c99 -Wall

all: hashlist hashfix

hashfix: hashfix.c hashfix.h common.c common.h net_download.c net_resume.c net.h
	gcc $(cflags)  hashfix.c common.c net_download.c net_resume.c $(libs) -o hashfix

hashlist: hashlist.c common.c common.h
	gcc $(cflags) hashlist.c common.c $(libs) -o hashlist

debug: hashlist.c common.c common.h hashfix.c net_download.c net_resume.c net.h
	gcc $(cflags) -g hashlist.c common.c $(libs) -o hashlist_debug
	gcc $(cflags) -g hashfix.c common.c net_download.c net_resume.c $(libs) -o hashfix_debug

clean:
	rm hashfix hashlist *_debug


.PHONY: all debug clean
