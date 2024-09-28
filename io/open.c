// SPDX-License-Identifier: BSD-3-Clause

#include <fcntl.h>
#include <internal/syscall.h>
#include <internal/types.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/stat.h>

/* documentation: https://man7.org/linux/man-pages/man2/open.2.html */
int open(const char *filename, int flags, ...)
{
	if (!filename)
		return -1;

	/* the third parameter exists only if the file is not created
	=> flags & O_CREAT for mode, because flags = O1 | O2 | ... | On */

	int file_not_exists = flags & O_CREAT;

	if (~file_not_exists) {
		int verif00 = syscall(__NR_open, filename, flags);
		if (verif00 < 0) {
			// pass the negative returned value of syscall to errno
			errno = -verif00;
			return -1;
		}

		// operation executed successfully
		return verif00;
	}

	/* using stdarg.h tools, methods obtained from
	https://ocw.cs.pub.ro/courses/programare/laboratoare/lab13 */

	va_list argument_list;
	va_start(argument_list, flags);
	mode_t mode_param = va_arg(argument_list, mode_t);

	// calling creat instead of open to avoid problems
	int verif01 = syscall(__NR_creat, filename, mode_param);
	if (verif01 < 0) {
		errno = -verif01;
		return -1;
	}

	return verif01;
}
