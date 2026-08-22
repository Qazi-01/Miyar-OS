#ifndef ADDRESS_SPACE_H
#define ADDRESS_SPACE_H

#include <stdint.h>

#define ADDRESS_SPACE_PAGE_TABLES 1024

typedef struct address_space {
    uint32_t *page_directory;
    uint32_t **page_tables;
} address_space_t;

address_space_t *address_space_create(void);
void address_space_destroy(address_space_t *address_space);
void address_space_activate(address_space_t *address_space);
address_space_t *address_space_current(void);
void address_space_init(void);
address_space_t *address_space_kernel(void);

#endif