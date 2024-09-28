// SPDX-License-Identifier: BSD-3-Clause

#include <unistd.h>
#include <internal/syscall.h>
#include <internal/types.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>

int truncate(const char *path, off_t length)
{
	/* truncate handles directory or non-existent file cases
	as directories cannot be directly truncated in C */
	int truncate_code = syscall(__NR_truncate, path, length);

	if (truncate_code) {
		errno = -truncate_code;
		return -1;
	}

	return 0;
}
