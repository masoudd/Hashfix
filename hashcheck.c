#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

/*
 * This program takes a file and a hashlist.
 * Checks the file against the hashlist and if a chunk of the file
 * does not match the hash in the hashlist, it prints a line stating
 * the chunk number */

static int chunk_len;

static char *name = "hashcheck";

void help(void) {
    printf("Usage: %s FILE LIST\n", name);
}

int main (int argc, char *argv[]) {

    name = argv[0];

    if (argc != 3) {
        help();
        return 1;
    }

    FILE *file = fopen(argv[1], "r+");
    if (!file) {
        perror(NULL);
        exit(1);
    }

    FILE *hashlist = fopen(argv[2], "r");
    if (!hashlist) {
        perror(NULL);
        exit(1);
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    long expected_size;
    long current_size = fsize(file);
    read = getline(&line, &len, hashlist);
    int matched = sscanf(line, "%d %ld ", &chunk_len, &expected_size);
    if (matched != 2) {
        fprintf(stderr, "Invalid first line in hash list: %s", argv[2]);
        exit(1);
    }
    if (expected_size != current_size) {
        fprintf(stderr, "File size is wrong, it is %ld, it should be %ld bytes\n", current_size, expected_size);
    }

    char hex[MD5_DIGEST_SIZE * 2 + 1];
    uint8_t *buffer = malloc(chunk_len);

    bool loop = true;
    unsigned int counter = 0;
    while (loop) {
        errno = 0;
        read = getline(&line, &len, hashlist);
        if (read == -1)
            break;
        if (read != MD5_DIGEST_SIZE * 2 + 1) {
            fprintf(stderr, "invalid hash: %s", line);
            break;
        }

        int done = fread(buffer, 1, chunk_len, file);
        if (done < chunk_len) {
            loop = false;
            if(!feof(file)) {
                fprintf(stderr, "Error occured during reading from file\n");
            }
        }
        compute_hash_hex(done, buffer, hex);

        if (strncmp(line, hex, MD5_DIGEST_SIZE * 2) != 0) {
            printf("chunk %u is wrong\n", counter);
        }

        counter++;
    }

    fclose(file);
    fclose(hashlist);

    free(line);
    free(buffer);
    return 0;
}
