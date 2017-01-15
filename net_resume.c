#include <string.h>
#include <stdbool.h>
#include <curl/curl.h>
#include "net.h"

static volatile bool resume = false;

size_t header_callback(char *buffer, size_t size, size_t nitems, void* userdata) {
    char *str = "HTTP/1.1 206";
    size_t len = strlen(str);
    if (size * nitems >= len) {
        int r = strncmp(buffer, str, len);
        if (r == 0) {
            resume = true;
        }
    }

    return size * nitems;
}

bool supports_resume(char *url) {

    curl_global_init(CURL_GLOBAL_ALL);
    CURL *handle = curl_easy_init();

    curl_easy_setopt(handle, CURLOPT_URL, url);

    char tmp[40];
    struct curl_slist *slist = NULL;
    snprintf(tmp, sizeof(tmp), "Range: bytes=%" PRIu32 "-%" PRIu32, 0, 100); //this range shouldn't matter
    slist = curl_slist_append(slist, tmp);
    curl_easy_setopt(handle, CURLOPT_HTTPHEADER, slist);
    curl_easy_setopt(handle, CURLOPT_NOBODY, 1L); //Will send HEAD request
    curl_easy_setopt(handle, CURLOPT_HEADERFUNCTION, header_callback);
    curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(handle, CURLOPT_MAXREDIRS, 10L);

    CURLcode success = curl_easy_perform(handle);

    curl_slist_free_all(slist); //no matter the result, we should free this

    if (success == CURLE_OK) {
        return resume;
    } else {
        return false;
    }
}
