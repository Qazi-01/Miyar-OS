#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>

void paging_init(void);
uint32_t *paging_get_directory(void);
uint32_t *paging_get_first_table(void);

#endif