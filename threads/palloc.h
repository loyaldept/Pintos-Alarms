#ifndef THREADS_PALLOC_H
#define THREADS_PALLOC_H

#include <stddef.h>

/* Page allocation flags */
#define PAL_ZERO 0x01

/* Page allocation functions */
void *palloc_get_page(int flags);
void palloc_free_page(void *page);

#endif /* THREADS_PALLOC_H */

