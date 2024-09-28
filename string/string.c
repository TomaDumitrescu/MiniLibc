// SPDX-License-Identifier: BSD-3-Clause

#include <string.h>
#define NULL_ERROR -8942893

char *strcpy(char *destination, const char *source)
{
	if (!destination || !source)
		return NULL;

	// not alterating destination, source pointers locations
	char *itr_d = &destination[0], *itr_s = (char *)(&source[0]);

	do {
		*(itr_d++) = *(itr_s++);
	} while (((int)(*itr_s)) != 0b0);

	// adding null string terminator
	*itr_d = *itr_s;

	return destination;
}

char *strncpy(char *destination, const char *source, size_t len)
{
	if (!destination || !source)
		return NULL;

	// verifying to not have more than source len bytes to copy
	size_t len_max = 0, cnt = 1;
	char *vf0 = (char *)(&source[0]);
	while (((int)(*vf0)) != 0b0)
		vf0++, ++len_max;

	// clamp function
	if (len > len_max)
		len = len_max;

	char *itr_d = &destination[0], *itr_s = (char *)(&source[0]);
	for (; cnt++ <= len;)
		*(itr_d++) = *(itr_s++);

	*itr_d = *itr_s;

	return destination;
}

char *strcat(char *destination, const char *source)
{
	// minimal memory check
	if (!destination || !source)
		return NULL;

	char *itr_str_d = &destination[0];
	while (((int)(*itr_str_d)) != 0b0)
		itr_str_d++;

	/* assuming dest has enough memory, pass source start
	address instead of dest[len(dest)]*/
	char *itr_str_s = (char *)(&source[0]);

	do {
		*(itr_str_d++) = *(itr_str_s++);
	} while (((int)(*itr_str_s)) != 0b0);

	*itr_str_d = *itr_str_s;

	return destination;
}

char *strncat(char *destination, const char *source, size_t len)
{
	if (!destination || !source)
		return NULL;

	size_t len_max = 0, cnt = 1;
	char *vf__longer = (char *)(&source[0]);
	while (((int)(*vf__longer)) != 0b0)
		vf__longer++, ++len_max;

	if (len > len_max)
		len = len_max;

	char *itr_d = &(destination[0]), *itr_s = (char *)(&source[0]);
	// moving at the end of dest string
	while (((int)(*itr_d)) != 0b0)
		itr_d++;

	for (; cnt++ <= len;)
		*(itr_d++) = *(itr_s++);

	/* necesary to put '\0', since *itr_s might be
	another character */
	*itr_d = '\0';

	return destination;
}

int strcmp(const char *str1, const char *str2)
{
	if (!str1 || !str2)
		return NULL_ERROR;

	char *itr_s1 = (char *)(&str1[0]);
	char *itr_s2 = (char *)(&str2[0]);
	while (((int)(*itr_s1)) * ((int)(*itr_s2)) != 0b0) {
		// goes until two characters differ, sufficient to conclude
		if (((int)(*itr_s1)) > ((int)(*itr_s2)))
			return 1;

		if (((int)(*itr_s1)) < ((int) (*itr_s2)))
			return -1;

		itr_s1++, ++itr_s2;
	}

	if (*itr_s1 == *itr_s2)
		return 0;

	if (((int)(*itr_s1)) != 0b0)
		return 1;

	return -1;
}

int strncmp(const char *str1, const char *str2, size_t len)
{
	if (!str1 || !str2)
		return NULL_ERROR;

	char *itr_s1 = (char *)(&str1[0]), *itr_s2 = (char *)(&str2[0]);
	size_t n = 0;

	while (((int)(*itr_s1)) * ((int)(*itr_s2)) != 0b0 && ++n <= len) {
		// goes until two characters differ, sufficient to conclude
		if (((int)(*itr_s1)) > ((int)(*itr_s2)))
			return 1;

		if (((int)(*itr_s1)) < ((int) (*itr_s2)))
			return -1;

		itr_s1++, ++itr_s2;
	}

	// only len characters are verified
	return 0;
}

size_t strlen(const char *str)
{
	if (!str)
		return 0;

	size_t i = 0;
	char *itr = (char *)(&str[0]);
	for (; *itr != '\0'; itr++, i++)
		;

	return i;
}

char *strchr(const char *str, int c)
{
	if (!str)
		return NULL;

	char *itr_str = (char *)(&str[0]);
	while (((int)(*itr_str)) != 0b0) {
		if (((int)(*itr_str)) == c)
			return itr_str;

		itr_str++;
	}

	// no character found
	return NULL;
}

char *strrchr(const char *str, int c)
{
	if (!str)
		return NULL;

	char *itr = (char *)(&str[0]);
	int min_index = 0;
	while (((int)(*itr)) != 0b0)
		itr++, ++min_index;

	// last appartition -> invert the looping
	while (min_index-- >= 0) {
		if (*itr == ((char)c))
			return itr;

		itr--;
	}

	return NULL;
}

char *strstr(const char *haystack, const char *needle)
{
	if (!haystack || !needle)
		return NULL;

	char *itr_h = (char *)(&haystack[0]);

	// modified strlen to not alter the start_pointer;
	int len_h = strlen(itr_h), len_n = strlen(needle);

	int idx_h = 0;
	while (len_h - len_n - idx_h >= 0) {
		int cmp_factor = strncmp(itr_h, needle, len_n);

		// first occurence, the function returns
		if (cmp_factor == 0)
			return itr_h;

		idx_h++, ++itr_h;
	}

	return NULL;
}

char *strrstr(const char *haystack, const char *needle)
{
	if (!haystack || !needle)
		return NULL;

	char *itr_h = (char *)(&haystack[0]), *f_str = NULL;
	// modified strlen to not alter the start_pointer;
	int len_h = strlen(itr_h), len_n = strlen(needle);

	int idx_h = 0;
	while (len_h - len_n - idx_h >= 0) {
		int cmp_factor = strncmp(itr_h, needle, len_n);
		// we retain the last modification of the pointer
		if (cmp_factor == 0)
			f_str = itr_h;

		idx_h++, ++itr_h;
	}

	return f_str;
}

void *memcpy(void *destination, const void *source, size_t num)
{
	if (!destination || !source)
		return NULL;

	char *store_bytes = (char *)destination, *itr_src = (char *)source;
	size_t max_len = strlen(source);

	if (num > max_len)
		num = max_len;

	// auxiliary function to copy byte with byte the data
	strncpy(store_bytes, itr_src, num);

	return destination;
}

void *memmove(void *destination, const void *source, size_t num)
{
	if (!destination || !source)
		return NULL;

	if (destination == source)
		return destination;

	char *store_bytes = (char *)destination, *itr_src = (char *)source;
	size_t max_len = strlen(source), cnt = 1;

	if (num > max_len)
		num = max_len;

	if (store_bytes - itr_src > 0) {
		// no possibility of overlapping
		for (; cnt++ <= num;) {
			if (itr_src - (char*)(destination) >= 0)
				break;

			*(store_bytes++) = *(itr_src++);
		}
	} else {
		for (; cnt++ <= num;) {
			if (store_bytes - (char *)source >= 0)
				break;

			*(store_bytes++) = *(itr_src++);
		}
	}

	return destination;
}

int memcmp(const void *ptr1, const void *ptr2, size_t num)
{
	char *convert_str1 = (char *)ptr1, *convert_str2 = (char *)ptr2;

	// the case is reduced to comparing n characters
	return strncmp(convert_str1, convert_str2, num);
}

void *memset(void *source, int value, size_t num)
{
	if (!source)
		return NULL;

	char *convert_str = (char *)source;
	size_t max_len = strlen(convert_str), cnt = 1;

	if (num > max_len)
		num = max_len;

	// setting num bytes with the character with the ascii code value
	for (; cnt++ <= num;)
		*(convert_str++) = (char)(value);

	return source;
}
