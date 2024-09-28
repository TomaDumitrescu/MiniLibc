// SPDX-License-Identifier: BSD-3-Clause

#include <sys/stat.h>
#include <internal/syscall.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int stat(const char *restrict path, struct stat *restrict buf)
{
	if (!path)
		return -1;

	// error-handling by stat syscall
	int vf_stat = syscall(__NR_stat, path, buf);

	if (vf_stat < 0) {
		errno = -vf_stat;
		return -1;
	}

	return 0;
}
