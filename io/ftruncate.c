// SPDX-License-Identifier: BSD-3-Clause

#include <unistd.h>
#include <internal/syscall.h>
#include <internal/types.h>
#include <errno.h>

int ftruncate(int fd, off_t length)
{
	int _return_value = syscall(__NR_ftruncate, fd, length);

	if (_return_value) {
		// updating errno variable
		errno = -_return_value;
		return -1;
	}

	return 0;
}
