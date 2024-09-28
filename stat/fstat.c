// SPDX-License-Identifier: BSD-3-Clause

#include <sys/stat.h>
#include <internal/syscall.h>
#include <errno.h>

int fstat(int fd, struct stat *st)
{
	// error-handling by the system call
	int f_exit_val = syscall(__NR_fstat, fd, st);

	if (f_exit_val) {
		errno = -f_exit_val;
		return -1;
	}

	return 0;
}
