libs := -lnettle
cflags := -std=c99 -Wall -static

all: hashlist hashfix

hashfix: hashfix.c hashfix.h
	gcc $(cflags)  hashfix.c $(libs) -o $@

hashlist: hashlist.c
	gcc $(cflags) $^ $(libs) -o $@


.PHONY: all
