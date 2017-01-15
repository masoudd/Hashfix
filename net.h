#ifndef net_h
#define net_h
#include <inttypes.h>
#include <stdbool.h>

bool supports_resume(char *url);
uint8_t *download_chunk(uint32_t start, uint32_t end, char *url);

#endif
