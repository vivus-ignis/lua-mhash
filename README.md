lua-mhash
=========

Lua bindings to mhash library. Forked from http://lua-users.org/files/wiki_insecure/users/tnodir/
Modified to compile for lua 5.1.

Installing
----------

Clone the code, edit Makefile (adjust paths), then make + make install.

Example usage
--------------

```lua
local mhash = require "mhash"

-- list all hashing algorithms known to mhash library
local h_id = 1
local max_h_id = mhash.count()
while h_id <= max_h_id do
  local h_name = mhash.name(h_id)
  if h_name then
    print(h_id .. " : " .. mhash.name(h_id) )
  else
    print(h_id .. " : ---")
  end
  h_id = h_id + 1
end

-- calculate MD5 hash for a string and convert it to hex
-- http://snippets.luacode.org/snippets/String_to_Hex_String_68
function HexDumpString(str,spacer)
  return (
    string.gsub(str,"(.)",
      function (c)
        return string.format("%02X%s",string.byte(c), spacer or "")
      end)
  )
end

local m = mhash.instance() -- create an object

m:init(2)            -- initialize for MD5 (id 2) algorithm

m:add("This string will be hashed with MD5 algorithm")

local hashed = HexDumpString(m:done(true)) -- do the hashing and convert to hex
                            -- boolean parameter for 'done' method tells whether
                            -- to discard the result or not
```
