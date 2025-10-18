#ifndef THREADS_INTERRUPT_H
#define THREADS_INTERRUPT_H

#include <stdbool.h>

/* interrupt levels */
enum intr_level {
    INTR_OFF,             /* interrupts disabled */
    INTR_ON               /* interrupts enabled */
};

/* interrupt priority levels */
#define TIMER_IRQ_PRIORITY 0

/* interrupt frame structure */
struct intr_frame {
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp_dummy;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
    uint16_t gs, :16;
    uint16_t fs, :16;
    uint16_t es, :16;
    uint16_t ds, :16;
    uint32_t vec_no;
    uint32_t error_code;
    void (*eip)(void);
    uint16_t cs, :16;
    uint32_t eflags;
    void *esp;
    uint16_t ss, :16;
};

/* interrupt functions */
enum intr_level intr_get_level(void);
enum intr_level intr_set_level(enum intr_level level);
enum intr_level intr_disable(void);
enum intr_level intr_enable(void);
bool intr_context(void);

/* interrupt registration */
void intr_register_int(uint8_t vec, void (*handler)(struct intr_frame *), 
                       const char *name, int priority);

#endif /* THREADS_INTERRUPT_H */
