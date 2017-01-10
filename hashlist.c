#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <nettle/md5.h>

#include "common.h"

static char *name = "hashlist";

void help(void) {
    printf("Usage: %s FILE LIST\n", name);
    printf("\tWill not overwrite LIST if it already exist\n");
}

static const int chunk_len = 1024 * 1024; // 1MiB

int main(int argc, char *argv[]) {
    name = argv[0];

    if (argc != 3) {
        help();
        return 1;
    }


    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror(NULL);
        exit(1);
    }

    FILE *hashlist;

    errno = 0;
    int r = access(argv[2], F_OK);
    if (r == 0) {
        fprintf(stderr, "%s already exists, will not overwrite it\n", argv[2]);
        exit(EXIT_FAILURE);
    } else if (r == -1 && errno == ENOENT) {
        hashlist = fopen(argv[2], "w");
        if (!hashlist) {
            perror(NULL);
            exit(1);
        }
    } else {
        perror(NULL);
        fprintf(stderr, "checking %s encountered an error:\n", argv[2]);
        exit(EXIT_FAILURE);
    }

    /* first line is size of each chunk and size of the whole file */
    long size = fsize(file);
    fprintf(hashlist, "%d %ld\n", chunk_len, size);

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
