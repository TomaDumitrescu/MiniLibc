#ifndef __TIME_H__
# define __TIME_H__	1

// respecting the format of other headers
#ifdef __cplusplus
extern "C" {
#endif

#include <internal/types.h>

/* https://en.cppreference.com/w/c/chrono/CLOCKS_PER_SEC:
"POSIX defines CLOCKS_PER_SEC as one million, regardless of
the actual precision of clock." */
#define CLOCKS_PER_SEC 1000000

// the structure respects the standard
typedef struct timespec timespec;
struct timespec {
	time_t tv_sec;
	long int tv_nsec;
};

int nanosleep(const struct timespec *req, struct timespec *rem);
int sleep(int num_sec);

#ifdef __cplusplus
}
#endif

#endif
