local blocks = require 'blocks'

local basic_latin = blocks.find 'latin'
-- block_mt.__len
assert(#basic_latin == 0x80) -- number of codepoints in block, including unassigned codepoints
-- block_mt.__tostring
assert(tostring(basic_latin) == 'Basic Latin (U+0-7F)')
-- equality
assert(blocks[1] == blocks[1]) -- Block objects are cached, so no __eq metamethod is needed.
-- block_mt.__lt
assert(blocks[1] < blocks[2])
-- block_mt.__le
assert(blocks[1] <= blocks[1])
assert(blocks[1] <= blocks[2])
assert(blocks[1] <= blocks[-1])
-- block_mt.__index
assert(basic_latin[1] == '\0') -- index -> character
assert(basic_latin[-1] == basic_latin[#basic_latin])
assert(basic_latin['\0'] == 1) -- character -> index
assert(basic_latin['\0\1'] == nil)
assert(basic_latin[basic_latin[1]] == 1)

-- Print all codepoints (in UTF-8 encoding).
assert(basic_latin:chars() == '\x00\x01\x02\x03\x04\x05\x06\a\b\t\n\x0B\f\r\z
\x0E\x0F\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F !\"#\z
$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmn\z
opqrstuvwxyz{|}~\x7F')

-- blocks_mt.__index
assert(blocks[-1] == blocks[#blocks])
assert(blocks[blocks[-1]] == #blocks)
-- character -> block
assert(blocks.a == blocks[1])
assert(blocks.ab == nil)
assert(blocks['\u{E01F0}'] == nil) -- codepoint that doesn't belong to a block
-- block -> index of block
assert(blocks[blocks[1]] == 1)
assert(blocks[basic_latin] == 1)
-- Check for error.
assert(pcall(function () getmetatable(blocks).__index('hello', 'a') end) == false)
-- blocks_mt.__len
assert(#blocks == 280) -- number of Unicode blocks

-- Blocks methods:
-- Get block to which a codepoint belongs.
assert(blocks.cptoblock(('a'):byte()) == basic_latin)

-- Find all blocks whose name matches a string (case-insensitively).
local all_latin = blocks.findall 'latin'
assert(#all_latin == 8) -- 8 blocks with 'Latin' in their name

-- Check lowercasing of query.
local i, b = blocks.next 'LaTiN'
assert(i == 1 and b == basic_latin)
local count = 0
for _ in blocks.match 'LaTiN' do
	count = count + 1
end
assert(count == #all_latin)

print('Success!')