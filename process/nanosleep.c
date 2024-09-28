#include <time.h>
#include <internal/syscall.h>
#include <stddef.h>
#include <errno.h>

int nanosleep(const struct timespec *req, struct timespec *rem)
{
	// OS call to nanosleep
	int analyse = syscall(__NR_nanosleep, req, rem);

	if (analyse) {
		errno = -analyse;
		return -1;
	}

	return 0;
}
