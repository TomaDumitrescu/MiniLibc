// SPDX-License-Identifier: BSD-3-Clause

#include <internal/syscall.h>
#include <internal/mm/mem_list.h>
#include <internal/types.h>
#include <internal/essentials.h>
#include <sys/mman.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#ifndef MAP_ANONYMOUS
#define MAP_ANONYMOUS	0x20
#endif

// no brk in this implementation
void *malloc(size_t size)
{
	void *null_addr = NULL;

	// no file implied => MAP_ANONYMOUS flag and fd = -1
	void *location = mmap(null_addr, size, PROT_READ | PROT_WRITE | PROT_EXEC,
						  MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

	if (location == MAP_FAILED) {
		errno = ENOMEM;
		return NULL;
	}

	/* The use of a list is necesary to retain the size
	when free is called, because free calls munmap that
	requires the length = num_bytes (automatically deleting
	miniblocks and blocks parts from that zone).
	*/

	// new alloc => new mem_list element
	int check_val = mem_list_add(location, size);

	if (check_val < 0) {
		errno = ENOMEM;
		munmap(location, size);
		return NULL;
	}

	return location;
}

void *calloc(size_t nmemb, size_t size)
{
	// using auxiliary function malloc, memlist updated properly
	void *location = malloc(nmemb * size);

	if (location == NULL) {
		errno = ENOMEM;
		return NULL;
	}

	size_t cnt = 1;
	char *itr_bytes = (char *)location;
	for (; cnt++ < nmemb * size; itr_bytes++)
		*itr_bytes = '\0';		// the zero of calloc

	return location;
}

void free(void *ptr)
{
	// use the dereferenced value of return_val of mem_list_find
	struct mem_list mem_element = *mem_list_find(ptr);

	int code = munmap(ptr, mem_element.len);
	if (code)
		errno = ENOMEM;

	code = mem_list_del(ptr);
	if (code < 0)
		errno = ENOMEM;
}

void *realloc(void *ptr, size_t size)
{
	// another solution: use of mremap
	if (!ptr)
		free(ptr);

	ptr = malloc(size);

	return ptr;
}

void *reallocarray(void *ptr, size_t nmemb, size_t size)
{
	// another possibility: mremap
	if (!ptr)
		free(ptr);

	ptr = calloc(nmemb, size);

	return ptr;
}
