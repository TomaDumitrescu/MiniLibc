// SPDX-License-Identifier: BSD-3-Clause

#include <sys/stat.h>
#include <internal/types.h>
#include <unistd.h>
#include <internal/syscall.h>
#include <fcntl.h>
#include <errno.h>

#define makedev(x, y) ( \
		(((x)&0xfffff000ULL) << 32) | \
	(((x)&0x00000fffULL) << 8) | \
		(((y)&0xffffff00ULL) << 12) | \
	(((y)&0x000000ffULL)) \
	)

/* Structure describing file characteristics as defined in linux/stat.h */
struct statx {
	uint32_t stx_mask;
	uint32_t stx_blksize;
	uint64_t stx_attributes;
	uint32_t stx_nlink;
	uint32_t stx_uid;
	uint32_t stx_gid;
	uint16_t stx_mode;
	uint16_t pad1;
	uint64_t stx_ino;
	uint64_t stx_size;
	uint64_t stx_blocks;
	uint64_t stx_attributes_mask;
	struct {
		int64_t tv_sec;
		uint32_t tv_nsec;
		int32_t pad;
	} stx_atime, stx_btime, stx_ctime, stx_mtime;
	uint32_t stx_rdev_major;
	uint32_t stx_rdev_minor;
	uint32_t stx_dev_major;
	uint32_t stx_dev_minor;
	uint64_t spare[14];
};

// predefined flag dor statx
#ifndef STATX_BASIC_STATS
#define STATX_BASIC_STATS	0x000007ffU
#endif

/*
	The implementation uses the methods and logic from
	"https://git.musl-libc.org/cgit/musl/tree/src/stat/fstatat.c"
*/
int fstatat_statx(int fd, const char *restrict path, struct stat *restrict st, int flag)
{
	struct statx file_statx;

	// passing the address of file_statx to match stat* type in statx OS function call
	int out = syscall(__NR_statx, fd, path, flag, STATX_BASIC_STATS, &file_statx);
	if (out) {
		errno = -out;
		return -1;
	}

	// make devices
	st->st_dev = makedev(file_statx.stx_dev_major, file_statx.stx_dev_minor);
	st->st_rdev = makedev(file_statx.stx_rdev_major, file_statx.stx_rdev_minor);

	/* statx data copied to stat, in order
	no possibility of = type copy, since the structures are not identical */
	st->st_ino = file_statx.stx_ino;
	st->st_nlink = file_statx.stx_nlink;
	st->st_mode = file_statx.stx_mode;
	st->st_uid = file_statx.stx_uid;
	st->st_gid = file_statx.stx_gid;
	st->__pad0 = file_statx.pad1;
	st->st_size = file_statx.stx_size;
	st->st_blksize = file_statx.stx_blksize;
	st->st_blocks = file_statx.stx_blocks;

	// errors on copying for st_atime_nsec and its complements
	st->st_atime = file_statx.stx_atime.tv_sec;
	st->st_mtime = file_statx.stx_mtime.tv_sec;
	st->st_ctime = file_statx.stx_ctime.tv_sec;

	return 0;
}

int fstatat(int fd, const char *restrict path, struct stat *restrict st, int flag)
{
	return fstatat_statx(fd, path, st, flag);
}
