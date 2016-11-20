#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <nettle/md5.h>


static const int chunk_len = 1024 * 1024; // 1MiB

static void to_hex (int len, uint8_t *data, char *str) {
    for (int i = 0; i < len; i++)
        sprintf(str + 2 * i, "%02x", data[i]);

    str[2 * len] = '\0';
}

int main (int argc, char *argv[]) {

    if (argc != 3) {
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror(NULL);
        exit(1);
    }

    FILE *hashlist = fopen(argv[2], "w");
    if (!hashlist) {
        perror(NULL);
        exit(1);
    }

    struct md5_ctx ctx;
    md5_init(&ctx);
    char hex[MD5_DIGEST_SIZE * 2 + 1];
    uint8_t digest[MD5_DIGEST_SIZE];
    uint8_t *buffer = malloc(chunk_len);




    bool loop = true;
    while (loop) {
        int done = fread(buffer, 1, chunk_len, file);
        if (done < chunk_len)
            loop = false;

        md5_update(&ctx, done, buffer);
        md5_digest(&ctx, MD5_DIGEST_SIZE, digest);
        to_hex(MD5_DIGEST_SIZE, digest, hex);
        fprintf(hashlist, "%s\n", hex);

    }

    fclose(file);
    fclose(hashlist);
    free(buffer);
    return 0;
}
