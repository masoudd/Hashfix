#include "common.h"

void to_hex(int len, uint8_t *data, char *str) {
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
