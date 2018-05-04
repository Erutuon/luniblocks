# luniblocks

A little Lua 5.3 library in C for playing around with Unicode blocks. I created it mainly to practice using the C API.

# `blocks` library
All matches are case-insensitive.

## `find(query[, position])`
Returns the first block matching `query`. Query begins at one-based index `position`, which defaults to `1`.

## `findall(query)`
Returns a table of all blocks matching `query`.

## `match(query)`
Returns an iterator function whose first upvalue is `query` that iterates over all blocks matching `query`.

## `next(query, position)`
The iterator function returned by `match`. Finds the next block matching `query` after index `position` (which defaults to `0`) and returns its index and it, such that `for i, block in blocks.next('latin', 0) do end` iterates over all blocks matching `latin`.

## `cptoblock(codepoint)`
Returns the block to which a codepoint belongs. Throws an error if the integer is out of range or does not belong to a block.

## indexing
* integer
** Returns block at one-based index in array of blocks.
* single UTF-8 character
** Returns block to which character belongs, if any.

# `block` object
Block objects are userdata containing a reference to a block struct from blocks.h. They are cached, so that if the same block is retrieved twice, it will be equal to itself.

