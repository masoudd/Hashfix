#ifndef common_h
#define common_h

#include <inttypes.h>
#include <stdio.h>
#include <nettle/md5.h>

void to_hex(int len, uint8_t data[static len], char str[static len * 2 + 1]);
long fsize(FILE *);
int compute_hash_hex(size_t length, uint8_t data[static length], char hex[static MD5_DIGEST_SIZE]);

#endif /* common_h */
