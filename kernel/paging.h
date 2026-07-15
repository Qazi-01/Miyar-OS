#ifndef PAGING_H
#define PAGING_H

#define PAGE_PRESENT  0x001
#define PAGE_WRITABLE 0x002
#define PAGE_USER     0x004

#include <stdint.h>

void paging_init(void);
uint32_t *paging_get_directory(void);
uint32_t *paging_get_first_table(void);

#endif