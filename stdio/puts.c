#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#ifndef stdout
#define stdout 1
#endif

#ifndef EOF
#define EOF -1
#endif

/* implementation of puts() using predefined read(), write()
in io/read_write.c */
int puts(const char *str) {
	if (!str)
		return EOF;

	/* printing str's bytes (new line char not contained)
	and retaining the number of bytes written in vf1 */
	int len_str = strlen(str);
	int vf1 = write(stdout, (const void *)str, len_str);

	if (vf1 < len_str)
		return EOF;

	// appending '\n'
	const char new_line = '\n';
	int vf2 = write(stdout, (const void *)&new_line, 1);

	if (vf2 < 1)
		return EOF;

	// operation executed with no errors, '\n' counts also
	return (++len_str);
}
