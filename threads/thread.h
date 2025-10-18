/* threads/thread.h - thread management by Zuhayr. */

#ifndef THREADS_THREAD_H
#define THREAD_THREAD_H

#include <debug.h>
#include <list.h>
#include <stdint.h>

/* it states in a thread's life cycle. */
enum thread_status
  {
    THREAD_RUNNING,     
    THREAD_READY,     
    THREAD_BLOCKED,     
    THREAD_DYING        
  };

/* thread identifier type. */
typedef int tid_t;
#define TID_ERROR ((tid_t) -1)          

/* thread priorities. */
#define PRI_MIN 0                       
#define PRI_DEFAULT 31                  
#define PRI_MAX 63                      

/* a kernel thread or user process. */
struct thread
  {
    tid_t tid;                          
    enum thread_status status;          
    char name[16];                      
    uint8_t *stack;                     
    int priority;                       
    struct list_elem allelem;           


    struct list_elem elem;            

    int64_t wakeup_tick;                

#ifdef USERPROG
   
    uint32_t *pagedir;                 
#endif

    /* owned by thread.c. */
    unsigned magic;                  
  };

/* if false (default), use round-robin scheduler. */
extern bool thread_mlfqs;

void thread_init (void);
void thread_start (void);
void thread_tick (void);
void thread_print_stats (void);
tid_t thread_create (const char *name, int priority, void (*function)(void *), void *aux);
void thread_block (void);
void thread_unblock (struct thread *);
struct thread *thread_current (void);
tid_t thread_tid (void);
const char *thread_name (void);
void thread_exit (void) NO_RETURN;
void thread_yield (void);

/* it performs here some operation on thread t, given auxiliary data aux. */
typedef void thread_action_func (struct thread *t, void *aux);
void thread_foreach (thread_action_func *, void *);

int thread_get_priority (void);
void thread_set_priority (int);

int thread_get_nice (void);
void thread_set_nice (int);
int thread_get_recent_cpu (void);
int thread_get_load_avg (void);

#endif /* threads/thread.h */


