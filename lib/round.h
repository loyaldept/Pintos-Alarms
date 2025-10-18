#ifndef LIB_ROUND_H
#define LIB_ROUND_H

/* round up x to the nearest multiple of n */
#define ROUND_UP(x, n) (((x) + (n) - 1) / (n) * (n))

/* round down x to the nearest multiple of n */
#define ROUND_DOWN(x, n) ((x) / (n) * (n))

#endif /* LIB_ROUND_H */
