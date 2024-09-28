// SPDX-License-Identifier: BSD-3-Clause

#include <unistd.h>
#include <internal/types.h>
#include <internal/syscall.h>
#include <errno.h>

off_t lseek(int fd, off_t offset, int whence)
{
	// pass fd, offset, whence as parameters after __NR_lseek
	off_t success = syscall(__NR_lseek, fd, offset, whence);

	if (success < 0) {
		errno = -success;
		return -1;
	}

	return success;
}
