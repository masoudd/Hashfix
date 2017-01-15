#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <inttypes.h>
#include <stdbool.h>

#include <curl/curl.h>
#include "net.h"

bool debug = true;

struct writedata {
    uint8_t *data;
    size_t size;
    size_t written;
};

size_t write_function(char *ptr, size_t size, size_t nmemb, void *userdata) {
    if (debug) {
        printf("write_function: got size: %zd, nmemb: %zd, data: ", size, nmemb);
        for (int i = 0; i < size * nmemb; i++) {
            putchar(ptr[i]);
        }
        putchar('\n');
    }
    struct writedata *wdptr = userdata;
    size_t w;
    if (size * nmemb > wdptr->size - wdptr->written) {
        return 0; // This should trigger some error in libcurl
    }
    for (w = 0; w < size * nmemb; w++) {
        wdptr->data[wdptr->written + w] = ptr[w];
    }
    wdptr->written += w;
    if (debug) {
        printf("write_function: wrote %zd bytes\n", w);
    }
    return w;
}

uint8_t *download_chunk(uint32_t start, uint32_t end, char *url) {

    curl_global_init(CURL_GLOBAL_ALL);
    struct writedata wd;
    wd.written = 0;
    wd.size = end - start + 1;
    wd.data = malloc(wd.size);
    if (wd.data == NULL) { //malloc failed...
        return NULL;
    }

    CURL *handle = curl_easy_init();

    curl_easy_setopt(handle, CURLOPT_URL, url);
    curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(handle, CURLOPT_MAXREDIRS, 10L);

    char tmp[40];
    struct curl_slist *slist = NULL;
    snprintf(tmp, sizeof(tmp), "Range: bytes=%" PRIu32 "-%" PRIu32, start, end);
    slist = curl_slist_append(slist, tmp);
    curl_easy_setopt(handle, CURLOPT_HTTPHEADER, slist);

    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &wd);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_function);
    CURLcode success = curl_easy_perform(handle);
    curl_slist_free_all(slist); //no matter the result, we should free this
    if (success == CURLE_OK) {
        return wd.data;
    } else {
        return NULL;
    }
}
