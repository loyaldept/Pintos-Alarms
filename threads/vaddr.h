#ifndef THREADS_VADDR_H
#define THREADS_VADDR_H

#include <stdint.h>

/* Page size */
#define PGSIZE 4096

/* Virtual address functions */
bool is_user_vaddr(const void *vaddr);
bool is_kernel_vaddr(const void *vaddr);

#endif /* THREADS_VADDR_H */

