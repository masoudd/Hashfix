#include "common.h"

void to_hex(int len, uint8_t data[static len], char str[static len * 2 + 1]) {
    for (int i = 0; i < len; i++)
        sprintf(str + 2 * i, "%02x", data[i]);

    str[2 * len] = '\0';
}

long fsize(FILE *file) {
    long orig = ftell(file);
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, orig, SEEK_SET);

    return size;
}

int compute_hash_hex(size_t length, uint8_t data[static length], char hex[static MD5_DIGEST_SIZE]) {
    struct md5_ctx ctx;
    uint8_t digest[MD5_DIGEST_SIZE];

    md5_init(&ctx);
    md5_update(&ctx, length, data);
    md5_digest(&ctx, MD5_DIGEST_SIZE, digest);
    to_hex(MD5_DIGEST_SIZE, digest, hex);
    return 0;
}
