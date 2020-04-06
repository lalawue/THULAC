--
-- Copyright (c) 2020 lalawue
--
-- This library is free software; you can redistribute it and/or modify it
-- under the terms of the MIT license. See LICENSE for details.
--

local LacCore = require("thulac")

local text = ...

if text == nil then
    print("lua test_thulua.lua 'SENTENCE'")
    os.exit(0)
end

local lac = LacCore.newLac(nil, nil, 0, 0, 0)
local count = lac:seg(text)
for i = 1, count, 1 do
    local word, tag = lac:fetch(i)
    print(word, tag)
end
lac:fini()
