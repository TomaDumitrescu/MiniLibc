#include <time.h>
#include <internal/syscall.h>
#include <stddef.h>
#include <errno.h>

// some system implementations have num_sec of type float
int sleep(int num_sec)
{
	if (num_sec < 0)
		return -1;

	// remaining_time is necesary especially when process is killed
	struct timespec requested_time, remaining_time;

	requested_time.tv_sec = num_sec, requested_time.tv_nsec = 0;

	// passing the structure addresses (modifications occur)
	int out_code = nanosleep(&requested_time, &remaining_time);

	if (out_code) {
		errno = -out_code;
		return -1;
	}

	return 0;
}
