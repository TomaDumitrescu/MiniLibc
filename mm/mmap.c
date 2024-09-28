// SPDX-License-Identifier: BSD-3-Clause

#include <sys/mman.h>
#include <sys/stat.h>
#include <errno.h>
#include <internal/syscall.h>
#include <internal/types.h>
#include <fcntl.h>

#ifndef MAP_ANONYMOUS
#define MAP_ANONYMOUS	0x20
#endif

#define MAP_PARAM (MAP_SHARED | MAP_PRIVATE)

void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset)
{
	if ((flags & MAP_PARAM) == 0) {
		errno = EINVAL;
		return MAP_FAILED;
	}

	if (fd < 0 && fd != -1) {
		errno = EBADF;
		return MAP_FAILED;
	}

	// If a file exists (fd valid), it's possible to get its statistics
	if (fd != -1) {
		struct stat try_get_stats;
		int stats_ok = fstat(fd, &try_get_stats);

		// checking if the positive file descriptor is valid
		if (stats_ok) {
			errno = EBADF;
			return MAP_FAILED;
		}
	}

	// mmap core error-handling
	void *locate_ptr = (void *)syscall(__NR_mmap, addr, length, prot, flags, fd, offset);

	if (locate_ptr == MAP_FAILED) {
		errno = -*(int *)locate_ptr;
		return locate_ptr;
	}

	return locate_ptr;
}

void *mremap(void *old_address, size_t old_size, size_t new_size, int flags)
{
	void *location = (void *)syscall(__NR_mremap, old_address, old_size, new_size, flags);

	if (location == MAP_FAILED) {
		// *(int *) converts (void *) to int
		errno = -*(int *)location;
		return location;
	}

	return location;
}

int munmap(void *addr, size_t length)
{
	// useful for free
	int return_val = syscall(__NR_munmap, addr, length);

	if (return_val) {
		errno = -return_val;
		return -1;
	}

	return 0;
}
