#ifndef _EPS_H
#define _EPS_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

int writeEps(FILE * fp, int w, int h, int nc, unsigned char * bytes, float px, float py);

#ifdef __cplusplus
}
#endif

#endif /* _EPS_H */
