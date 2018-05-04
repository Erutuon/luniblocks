#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lua.h"
#include "lauxlib.h"
#include "lapi.h"
#include "blocks.c"

/*
 *  Access information on Unicode blocks.
 *  Stores blocks as userdata (pointers to block array in blocks.h).
 */

#define BLOCKS_T            "blocks"
#define BLOCK_T             "block"
#define UNICODE_BLOCK_CACHE "Unicode_blocks"
#define CHARS_CACHE         "block_characters"

#define UTF8BUFFSZ 8
#define MAXUNICODE 0x10FFFF

#define check_query_len(L, arg, query_len) \
	if (!(1 <= query_len && query_len <= MAXBLOCKNAME - 1)) \
		return luaL_argerror(L, arg, \
			"query length out of valid range")

#define block_index_in_range(idx) (0 <= (idx) && (idx) <= BLOCKCOUNT - 1)
#define lua_check_block_index(L, arg, idx) \
	luaL_argcheck(L, block_index_in_range(idx), arg, "value out of range");

#define push_and_set(L, k, v, type) \
	lua_push##type(L, v); \
	lua_setfield(L, -2, k)

#define printhex(val, buff) \
	sprintf(buff, "%X", (unsigned) val)

#define getintfield_printhex(L, idx, codepoint, key, buff) \
	lua_getfield(L, idx, key); \
	codepoint = lua_tointeger(L, -1); /* This could fail. */ \
	printhex(codepoint, buff); \
	lua_pop(L, 1)

#define lua_newmetatable_setfuncs(L, tname, funcs) \
	luaL_newmetatable(L, tname), \
	luaL_setfuncs(L, funcs, 0)

#define lua_newregtable(L, k) \
	lua_newtable(L), \
	lua_setfield(L, LUA_REGISTRYINDEX, k)

/*
 *  From lutf8lib.c.
 *  Returns pointer to the byte after the byte sequence if it is valid, or else NULL.
 */
static const char *utf8_decode (const char *o, int *val) {
	static const unsigned int limits[] = {0xFF, 0x7F, 0x7FF, 0xFFFF};
	const unsigned char *s = (const unsigned char *)o;
	unsigned int c = s[0];
	unsigned int res = 0;  /* final result */
	if (c < 0x80)  /* ascii? */
		res = c;
	else {
		int count = 0;  /* to count number of continuation bytes */
		while (c & 0x40) {  /* still have continuation bytes? */
			int cc = s[++count];  /* read next byte */
			if ((cc & 0xC0) != 0x80)  /* not a continuation byte? */
				return NULL;  /* invalid byte sequence */
			res = (res << 6) | (cc & 0x3F);  /* add lower 6 bits from cont. byte */
			c <<= 1;  /* to test next bit */
		}
		res |= (c & 0x7F) << (count * 5);  /* add first byte */
		if (count > 3 || res > MAXUNICODE || res <= limits[count])
			return NULL;  /* invalid byte sequence */
		s += count;  /* skip continuation bytes read */
	}
	if (val != NULL) *val = res;
	return (const char *)s + 1;  /* +1 to include first byte */
}

/*
 *  From luaO_utf8esc in lobject.c.
 *  buff must be at least 4 bytes to print a valid Unicode codepoint,
 *  but 8 bytes allows for all values of unsigned long?
 */
static int utf8_encode (char * buff, unsigned long x) {
	int n = 1;  /* number of bytes put in buffer (backwards) */
	lua_assert(x <= 0x10FFFF);
	if (x < 0x80)  /* ascii? */
		buff[UTF8BUFFSZ - 1] = cast(char, x);
	else {  /* need continuation bytes */
		unsigned int mfb = 0x3f;  /* maximum that fits in first byte */
		do {  /* add continuation bytes */
			buff[UTF8BUFFSZ - (n++)] = cast(char, 0x80 | (x & 0x3f));
			x >>= 6;  /* remove added bits */
			mfb >>= 1;  /* now there is one less bit available in first byte */
		} while (x > mfb);  /* still needs continuation byte? */
		buff[UTF8BUFFSZ - n] = cast(char, (~mfb << 1) | x);  /* add first byte */
	}
	return n;
}

static int lua_new_block (lua_State * L, const Block * block) {
	const Block * * p = (const Block * *) lua_newuserdata(L, sizeof(Block * *));
	*p = block;
	
	luaL_setmetatable(L, BLOCK_T); // Give it the block metatable.
	
	return 1;
}

/*
 *  Push a block as a userdata.
 *  Cache blocks the first time they are accessed and return the cached value
 *  thereafter, so that blocks will compare as equal.
 */
 static int lua_push_block (lua_State * L, const Block * block) {
	if (block == NULL)
		return luaL_error(L, "no block supplied to lua_push_block");
	
	int index = block - blocks + 1; /* one-based indexing */
	if (lua_getfield(L, LUA_REGISTRYINDEX, UNICODE_BLOCK_CACHE) != LUA_TTABLE)
		return luaL_error(L, "block cache is not a table");
	
	if (   lua_geti(L, -1, index) != LUA_TUSERDATA /* Get cache[index] and test that it is userdata. */
		|| luaL_testudata(L, -1, BLOCK_T) == NULL ) { /* If cache[index] doesn't have block metatable, construct new table. */
		lua_pop(L, 1); /* Pop cache[index]. */
		
		lua_new_block(L, block);
		
		lua_pushvalue(L, -1); /* Copy block table. */
		lua_seti(L, -3, index); /* cache[index] = block */
	}
	lua_remove(L, -2); /* Remove cache. */
	
	return 1;
}

/* Push an array of block tables. */
int static lua_push_block_list (lua_State * L, Block_Results results) {
	if (results == NULL)
		return luaL_error(L, "no blocks supplied to lua_push_block_list");
	
	unsigned short i = 0, size = 0;
	
	while (results[size] != NULL) ++size; /* 'results' is NULL-terminated. */
	
	lua_pushinteger(L, (lua_Integer) size);
	
	lua_newtable(L);
	for (i = 0; i < size; ++i) {
		lua_push_block(L, results[i]);
		lua_seti(L, -2, i + 1); /* one-based indexing */
	}
	
	return 1;
}

/* Push the block at the given index. */
static int lua_getblock (lua_State * L, lua_Integer i) {
	if (i < 0)
		i = BLOCKCOUNT + i + 1;
	if (!block_index_in_range(i - 1)) {
		lua_pushnil(L);
		return 1;
	}
	return lua_push_block(L, &blocks[i - 1]); /* one-based indexing */
}

/*
 *  Check that the value at the given index is a block userdata, and if not
 *  throw an error.
 */
const Block * * lua_check_block (lua_State * L, int arg) {
	return (const Block * *) luaL_checkudata(L, arg, BLOCK_T);
}

static void lua_check_blocks (lua_State * L, int arg) {
	int top = lua_gettop(L);
	if (!(lua_type(L, arg) == LUA_TTABLE
		  && luaL_getmetafield(L, arg, "__name") == LUA_TSTRING /* Check type of __name metafield. */
		  && (lua_pushstring(L, BLOCKS_T), /* Check value of metafield. */
		      lua_rawequal(L, -2, -1)))) {
		const char * msg = lua_pushfstring(L, "%s expected, got %s",
			"blocks library", luaL_typename(L, arg));
		luaL_argerror(L, arg, msg);
	}
	lua_settop(L, top); /* Pop BLOCKS_T and metafield. */
}

/* Return all blocks whose names contain a string. */
static int lua_find_blocks (lua_State * L) {
	size_t query_len;
	const char * query = luaL_checklstring(L, 1, &query_len);
	check_query_len(L, 1, query_len);
	
	Block_Results results;
	if (find_blocks(query, results) == 0) {
		lua_pushnil(L);
		return 1;
	}
	/* or error message?
	return luaL_error(L, "no results found for '%s'", query); */
	
	return lua_push_block_list(L, results);
}

/* Find the block to which a codepoint belongs. */
Block * search_codepoint_in_blocks (lua_Integer codepoint) {
	if (0 <= codepoint && codepoint <= 0x10FFFF) {
		unsigned short low = 0, high = BLOCKCOUNT - 1, middle;
		while (low <= high) {
			middle = (low + high) / 2;
			if (codepoint < blocks[middle].low)
				high = middle - 1;
			else if (blocks[middle].high < codepoint)
				low = middle + 1;
			else
				return &blocks[middle];
		}
	}
	return NULL;
}

/* Return the block to which a codepoint belongs. */
static int lua_codepoint_to_block (lua_State * L) {
	lua_Integer codepoint = luaL_checkinteger(L, 1);
	
	if (!(0 <= codepoint && codepoint <= 0x10FFFF))
		luaL_error(L, "codepoint out of valid range (0-0x10FFFF)");
	
	Block * result = search_codepoint_in_blocks(codepoint);
	
	if (result == NULL) {
		char hexcodepoint[7]; /* "XXXXXX" */
		printhex(codepoint, hexcodepoint);
		luaL_error(L, "no block found for codepoint U+%s", hexcodepoint);
	}
	
	return lua_push_block(L, result);
}

/* Find the block whose lower bound is 'low_codepoint'. */
static int blocksearch (int low_codepoint) {
	if (0 <= low_codepoint && low_codepoint <= 0x10FFFF) {
		int low = 0, high = BLOCKCOUNT, middle;
		while (low <= high) {
			middle = (high + low) / 2;
			if (blocks[middle].low < low_codepoint)
				low = middle + 1;
			else if (low_codepoint < blocks[middle].low)
				high = middle - 1;
			else
				return middle;
		}
	}
	return -1;
}

/* Return the block at the given index, or the index of the given block. */
static int lua_blocks_index (lua_State * L) {
	lua_check_blocks(L, 1);
	switch (lua_type(L, 2)) {
		case LUA_TNUMBER: /* Return block at index. */
			return lua_getblock(L, luaL_checkinteger(L, 2));
		case LUA_TUSERDATA: { /* Return index of block. */
			lua_check_block(L, 2); /* unfortunately this returns an error ... */
			lua_getfield(L, 2, "low");
			int low_codepoint = lua_tointeger(L, -1);
			int block_index = blocksearch(low_codepoint);
			if (block_index >= 0) {
				lua_pushinteger(L, block_index + 1); /* one-based indexing */
				return 1;
			}
		}
		case LUA_TSTRING: { /* Retrieve block in which a codepoint is found. */
			size_t len = 0;
			const char * k = lua_tolstring(L, 2, &len);
			if (len > 0) {
				int codepoint    = -1;
				const char * end = utf8_decode(k, &codepoint);
				if (end == k + len) { /* single encoded codepoint */
					const Block * b = search_codepoint_in_blocks(codepoint);
					if (b != NULL) {
						lua_push_block(L, b);
						return 1;
					}
				}
			}
		}
		default:
			lua_pushnil(L);
			return 1;
	}
}

static int lua_blocks_newindex (lua_State * L) {
	lua_check_blocks(L, 1);
	switch (lua_type(L, 2)) {
		case LUA_TSTRING: /* Allow new methods to be defined. */
			lua_rawset(L, 1);
			return 0;
		default:
			return luaL_error(L, "new fields in 'blocks' must have string keys");
	}
}

static int lua_blocks_len (lua_State * L) {
	lua_pushinteger(L, BLOCKCOUNT);
	return 1;
}

/* Return a string representation of a block. */
static int lua_block_tostring (lua_State * L) {
	lua_check_block(L, 1);
	
	char hexcodepoint1[7]; /* "XXXXXX" */
	char hexcodepoint2[7];
	lua_Integer codepoint;
	
	getintfield_printhex(L, 1, codepoint, "low",  hexcodepoint1);
	getintfield_printhex(L, 1, codepoint, "high", hexcodepoint2);
	lua_getfield(L, 1, "name");
	const char * name = lua_tostring(L, -1); /* This could fail. */
	
	lua_pushfstring(L, "%s (U+%s-%s)", name, hexcodepoint1, hexcodepoint2);
	return 1;
}

void lua_push_codepoint_range (lua_State * L, int low, int high) {
	int encode_len;
	unsigned long codepoint;
	char encoded[UTF8BUFFSZ];
	luaL_Buffer b;
	luaL_buffinit(L, &b);
	
	for (codepoint = (unsigned long) low; codepoint <= (unsigned long) high; ++codepoint) {
		encode_len = utf8_encode(encoded, codepoint);
		luaL_addlstring(&b, &encoded[UTF8BUFFSZ - encode_len], encode_len);
	}
	
	luaL_pushresult(&b);
}

/* Return a string containing the codepoints in a block (encoded in UTF-8). */
static int lua_print_codepoints (lua_State * L) {
	lua_check_block(L, 1);
	
	lua_getfield(L, LUA_REGISTRYINDEX, CHARS_CACHE); /* Get REGISTRY[CHARS_CACHE] (cache). */
	lua_pushvalue(L, 1); /* Copy block. */
	if (lua_gettable(L, -1) == LUA_TSTRING) /* If type(cache[block]) == string, return it. */
		return 1;
	else
		lua_pop(L, 1); /* Pop cache[block]. */
	
	lua_Integer high, low;
	lua_getfield(L, 1, "low"), lua_getfield(L, 1, "high");
	low  = lua_tointeger(L, -2);
	high = lua_tointeger(L, -1);
	
	lua_push_codepoint_range(L, low, high); /* Print chars. */
	lua_remove(L, -3), lua_remove(L, -2); /* Remove low and high. */
	
	lua_pushvalue(L, 1); /* Copy block. */
	lua_pushvalue(L, -2); /* Copy chars. */
	lua_settable(L, 2); /* cache[block] = chars */
	
	return 1;
}

static int lua_block_to_table (lua_State * L) {
	const Block * block = *lua_check_block(L, 1);
	lua_createtable(L, 0, 4);
	push_and_set(L, "low", block->low, integer);
	push_and_set(L, "high", block->high, integer);
	push_and_set(L, "name", block->name, string);
	return 1;
}

static int lua_block_index (lua_State * L) {
	const Block * block = *lua_check_block(L, 1);
	switch (lua_type(L, 2)) {
		case LUA_TSTRING: {
			size_t len;
			const char * k = lua_tolstring(L, 2, &len);
			switch (*k) {
				case 'c':
					if (strcmp(k, "chars") == 0) {
						lua_pushcfunction(L, lua_print_codepoints);
						return 1;
					}
				case 'h':
					if (strcmp(k, "high") == 0) {
						lua_pushinteger(L, block->high);
						return 1;
					}
				case 'l':
					if (strcmp(k, "low") == 0) {
						lua_pushinteger(L, block->low);
						return 1;
					}
				case 'n':
					if (strcmp(k, "name") == 0) {
						lua_pushstring(L, block->name);
						return 1;
					}
				case 't':
					if (strncmp(k, "to", 2) == 0) {
						if (strcmp(k + 2, "table") == 0) {
							lua_pushcfunction(L, lua_block_to_table);
							return 1;
						}
						else if (strcmp(k + 2, "string") == 0) {
							lua_pushcfunction(L, lua_block_tostring);
							return 1;
						}
					}
			}
			int codepoint = -1;
			const char * end = utf8_decode(k, &codepoint);
			if (end == k + len /* single encoded codepoint */
					/* and codepoint belongs to this block */
					&& block->low <= codepoint && codepoint <= block->high) {
				lua_pushinteger(L, codepoint - block->low + 1); /* one-based indexing */
				return 1; /* Return position of codepoint in block. */
			}
		}
		case LUA_TNUMBER: { /* Print codepoint number i (one-based index) in block. */
			int isnum = 0;
			const lua_Integer i = lua_tointegerx(L, 2, &isnum);
			if (isnum) {
				unsigned long codepoint = -1; /* arbitrary out-of-range value */
				if (i > 0) /* one-based indexing */
					codepoint = (unsigned long) block->low + i - 1;
				else if (i < 0)
					codepoint = (unsigned long) block->high + i + 1;
				
				if (block->low <= codepoint && codepoint <= block->high) {
					char encoded[UTF8BUFFSZ];
					int encode_len = utf8_encode(encoded, codepoint);
					lua_pushlstring(L, &encoded[UTF8BUFFSZ - encode_len], encode_len);
					return 1;
				}
			}
		}
	}
	lua_pushnil(L);
	return 1;
}

/* extern Block * find_next_block (const char lower_query[MAXBLOCKNAME], ushrt * pos); */

/* Whether to switch to stateless iterator and avoid checking arguments every time? */
/* An iterator function. */
static int lua_find_next_block (lua_State * L) {
	/*
	 *  lua_block_find_iter_gen sets query as first upvalue, otherwise query is the
	 *  first argument, and must be converted to lowercase.
	 */
	const char * query = lua_tostring(L, lua_upvalueindex(1));
	if (query == NULL) {
		size_t query_len;
		query = luaL_checklstring(L, 1, &query_len);
		check_query_len(L, 1, query_len);
		char lower_query[MAXBLOCKNAME];
		str_to_lower(query, lower_query);
		query = lower_query;
	}
	lua_Integer pos;
	pos	= luaL_optinteger(L, 2, 0);
	lua_check_block_index(L, 2, pos);
	ushrt actual_pos = (ushrt) pos;
	Block * block	 = find_next_block(query, &actual_pos);
	
	if (block != NULL) {
		lua_pushinteger(L, actual_pos);
		lua_push_block(L, block);
		return 2;
	}
	else {
		lua_pushnil(L);
		return 1;
	}
}

/* Generate an iterator for a 'for' loop. */
static int lua_block_find_iter_gen (lua_State * L) {
	const char * query = luaL_checkstring(L, 1);
	char lower_query[MAXBLOCKNAME];
	str_to_lower(query, lower_query);
	lua_pushstring(L, lower_query);
	lua_pushcclosure(L, lua_find_next_block, 1); /* upvalue: query */
	lua_pushinteger(L, 0);
	return 2;
}

/*
 *  Find the first block whose name contains the given string, at or after the
 *  given index (default, 1).
 */
static int lua_find_one_block (lua_State * L) {
	size_t query_len;
	const char * query = luaL_checklstring(L, 1, &query_len);
	check_query_len(L, 1, query_len);
	char lower_query[MAXBLOCKNAME];
	str_to_lower(query, lower_query);
	lua_Integer pos	= luaL_optinteger(L, 2, 1) - 1; /* one-based indexing */
	lua_check_block_index(L, 2, pos);
	ushrt actual_pos = (ushrt) pos;
	Block * block	 = find_next_block(lower_query, &actual_pos);
	if (block == NULL) {
		lua_pushnil(L);
		return 1;
	}
	else {
		lua_push_block(L, block);
		lua_pushinteger(L, actual_pos);
		return 2;
	}
}

/* Return number of codepoints in block. */
static int lua_block_len (lua_State * L) {
	lua_check_block(L, 1);
	lua_Integer high, low;
	lua_getfield(L, 1, "high");
	high = lua_tointeger(L, -1);
	lua_getfield(L, 1, "low");
	low = lua_tointeger(L, -1);
	lua_pushinteger(L, high - low + 1);
	return 1;
}

static int lua_block_compare(lua_State * L, int idx1, int idx2) {
	int orig_top = lua_gettop(L);
	lua_check_block(L, idx1);
	lua_check_block(L, idx2);
	if (lua_rawequal(L, 1, 2))
		return 0;
	else {
		lua_Integer low1, low2;
		lua_getfield(L, 1, "low"), lua_getfield(L, 2, "low");
		low1 = lua_tointeger(L, orig_top + 1);
		low2 = lua_tointeger(L, orig_top + 2);
		lua_settop(L, orig_top);
		return low1 < low2 ? -1 : 1;
	}
}

static int lua_block_lt (lua_State * L) {
	lua_pushboolean(L, lua_block_compare(L, 1, 2) == -1);
	return 1;
}

static int lua_block_le (lua_State * L) {
	lua_pushboolean(L, lua_block_compare(L, 1, 2) <= 0);
	return 1;
}

static luaL_Reg funcs[] = {
	{ "find",      lua_find_one_block },
	{ "findall",   lua_find_blocks },
	{ "cptoblock", lua_codepoint_to_block },
	{ "match",     lua_block_find_iter_gen },
	{ "next",      lua_find_next_block },
	{ NULL, NULL }
};

static luaL_Reg blocks_metamethods[] = {
	{ "__index",    lua_blocks_index },
	{ "__newindex", lua_blocks_newindex },
	{ "__len",      lua_blocks_len },
	{ NULL, NULL }
};

static luaL_Reg block_metamethods[] = {
	{ "__index",    lua_block_index },
	{ "__tostring", lua_block_tostring },
	{ "__len",      lua_block_len },
	{ "__lt",       lua_block_lt },
	{ "__le",       lua_block_le },
	{ NULL, NULL }
};

int luaopen_blocks(lua_State * L) {
	luaL_newlib(L, funcs);
	
	lua_newmetatable_setfuncs(L, BLOCKS_T, blocks_metamethods); /* Create blocks metatable. */
	lua_setmetatable(L, -2); /* Set it as metatable for library. */
	
	lua_newmetatable_setfuncs(L, BLOCK_T, block_metamethods); /* Create block metatable. */
	lua_pop(L, 1); /* Pop block metatable. */
	
	lua_newregtable(L, UNICODE_BLOCK_CACHE); /* Generate cache table for blocks. */
	lua_newregtable(L, CHARS_CACHE); /* Generate cache table for characters. */
	
	return 1;
}