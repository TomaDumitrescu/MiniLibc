// SPDX-License-Identifier: BSD-3-Clause

#include <unistd.h>
#include <internal/syscall.h>
#include <stdarg.h>
#include <errno.h>

// methods taken from sample read_write.c implementations
int close(int fd)
{
	//__NR_close = id number of the specific OS system call
	int _result_code = syscall(__NR_close, fd);

	if (_result_code) {
		errno = -_result_code;
		return -1;
	}

	// operation executed successfully
	return 0;
}
