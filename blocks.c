#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "blocks.h"

/*
 *  str is null-terminated, and area of memory that lower_str points to must be of the
 *  same length as that pointed to by str.
 */
static void str_to_lower (const char * str, char * lower_str) {
	while (*str) {
		*lower_str++ = tolower(*str++);
	}
	*lower_str = '\0';
}

/*
 *  Determine maximum width of name and codepoint fields in blocks in blocklist.
 *  blocklist must be terminated by NULL.
 */
static void get_block_widths (const Block * * blocklist, uchar * name_width, uchar * hex_width) {
	uint i = 0;
	const Block * b;
	*name_width = 0;
	*hex_width  = 0;
	while ((b = blocklist[i++]) != NULL) {
		*name_width = max(strlen(b->name), *name_width);
		/*
		 *  Determine number of hex digits needed to represent largest codepoint in blocks.
		 *  Increment hex_width until b->high shifted over by *hex_width ^ 16.
		 *  x >> (y << 2) == uint(x / y ^ (2 ^ 4)) == uint(x / y ^ 16) = uint(log(x, 16) / y)
		 */
		while (b->high >> (*hex_width << 2))
			++*hex_width;
	}
}

extern void print_block (const Block * b, uchar name_width, uchar hex_width) {
	printf("%*s" GAP UNIFORMAT GAP UNIFORMAT "\n",
		name_width + 1, b->name,
		hex_width, b->low,
		hex_width, b->high);
}

extern void show_blocks (Block_Results results) {
	uchar name_width,
	      hex_width;
	ushrt i = 0;
	const Block * b;

	get_block_widths(results, &name_width, &hex_width);
	while ((b = results[i++]) != NULL)
		print_block(b, name_width, hex_width);
}

/* Search for next block after position 'pos' matching query 'lower_query'. */
extern Block * find_next_block (const char lower_query[MAXBLOCKNAME], ushrt * pos) {
	char lower_name[MAXBLOCKNAME];
	
	ushrt i = *pos;
	for (; i < BLOCKCOUNT; ++i) {
		str_to_lower(blocks[i].name, lower_name);
		if (strstr(lower_name, lower_query) != NULL)
			break;
	}
	*pos = i + 1;
	return i < BLOCKCOUNT ? &blocks[i] : NULL;
}

/* Case-insensitive search for block names. Returns number of results. */
extern ushrt find_blocks (const char * query, Block_Results results) {
	ushrt pos = 0, result_i = 0;
	char lower_query[MAXBLOCKNAME];
	str_to_lower(query, lower_query);
	
	/* Store pointers to matching blocks, then NULL at the end. */
	do
		results[result_i++] = find_next_block(lower_query, &pos);
	while (pos < BLOCKCOUNT);
	
	return result_i - 1;
}