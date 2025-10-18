#ifndef THREADS_LIST_H
#define THREADS_LIST_H

struct list_elem {
    struct list_elem *prev;
    struct list_elem *next;
};

struct list {
    struct list_elem head;
    struct list_elem tail;
};

/* Macro to get the containing struct */
#define list_entry(LIST_ELEM, STRUCT, MEMBER)           \
        ((STRUCT *) ((uint8_t *) &(LIST_ELEM)->next     \
                     - offsetof (STRUCT, MEMBER.next)))

/* Insert element before another element */
static inline void list_insert(struct list_elem *before, struct list_elem *elem) {
    elem->prev = before->prev;
    elem->next = before;
    before->prev->next = elem;
    before->prev = elem;
}

/* Pop front element */
static inline struct list_elem* list_pop_front(struct list *list) {
    struct list_elem *front = list->head.next;
    front->prev->next = front->next;
    front->next->prev = front->prev;
    return front;
}

static inline void list_init(struct list *lst) {
    lst->head.next = &lst->tail;
    lst->tail.prev = &lst->head;
}

static inline void list_push_back(struct list *lst, struct list_elem *elem) {
    elem->prev = lst->tail.prev;
    elem->next = &lst->tail;
    lst->tail.prev->next = elem;
    lst->tail.prev = elem;
}

static inline struct list_elem* list_begin(struct list *lst) {
    return lst->head.next;
}

static inline struct list_elem* list_end(struct list *lst) {
    return &lst->tail;
}

static inline struct list_elem* list_next(struct list_elem *elem) {
    return elem->next;
}

static inline struct list_elem* list_remove(struct list_elem *elem) {
    elem->prev->next = elem->next;
    elem->next->prev = elem->prev;
    return elem->next;
}

static inline int list_empty(struct list *lst) {
    return lst->head.next == &lst->tail;
}

/* Insert element in sorted order */
static inline void list_insert_ordered(struct list *list, struct list_elem *elem, 
                                       bool (*less)(const struct list_elem *, const struct list_elem *, void *), 
                                       void *aux) {
    struct list_elem *e;
    for (e = list->head.next; e != &list->tail; e = e->next) {
        if (less(elem, e, aux)) {
            list_insert(e, elem);
            return;
        }
    }
    list_push_back(list, elem);
}

#endif