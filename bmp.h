#ifdef __cplusplus
extern "C" {
#endif
#include "stdafx.h"

enum { ERR_BMP_OK, ERR_BMP_NOBMP, ERR_BMP_COMPRESS, ERR_BMP_NOTFOUND };
int read_bmp(char *fname, void **dest, int dest_bpp, int *w, int *h, int alpha, int header_size);

#ifdef __cplusplus
}
#endif