#ifndef THREADS_FLAGS_H
#define THREADS_FLAGS_H

/* EFLAGS bits */
#define FLAG_IF 0x00000200  /* Interrupt Flag */
#define FLAG_MBS 0x00000002 /* Must Be Set */

/* Selectors */
#define SELECTOR_KDSEG 0x10    /* Kernel data segment */
#define SELECTOR_KCODE 0x08     /* Kernel code segment */
#define RPL_0 0x00              /* Ring 0 */

#endif /* THREADS_FLAGS_H */

