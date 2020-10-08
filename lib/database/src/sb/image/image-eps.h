#pragma once

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

int writeEps(FILE *fp, int w, int h, int nc, const unsigned char *bytes,
             float px, float py);

#ifdef __cplusplus
}
#endif

