-- The MIT License (MIT)

-- Copyright (c) 2016 Colin Hall-Coates <yo@oka.io>

-- Permission is hereby granted, free of charge, to any person obtaining
-- a copy of this software and associated documentation files (the
-- "Software"), to deal in the Software without restriction, including
-- without limitation the rights to use, copy, modify, merge, publish,
-- distribute, sublicense, and/or sell copies of the Software, and to
-- permit persons to whom the Software is furnished to do so, subject to
-- the following conditions:

-- The above copyright notice and this permission notice shall be
-- included in all copies or substantial portions of the Software.

-- THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
-- EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
-- MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
-- NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
-- LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
-- OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
-- WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

-- randbytes.lua
-- Colin 'Oka' Hall-Coates
-- MIT, 2015

local defaults = setmetatable ({
  bytes = 4,
  mask = 256,
  file = 'urandom',
  filetable = {'urandom', 'random'}
}, { __newindex = function () return false end })

local files = {
  urandom = false,
  random = false
}

local utils = {}

function utils:gettable (...)
  local t, r = {...}, defaults.filetable
  if #t > 0 then r = t end
  return r
end

function utils:open (...)
  for _, f in next, self:gettable (...) do
    for k, _ in next, files do
      if k == f then
        files[f] = assert (io.open ('/dev/'..f, 'rb'))
      end
    end
  end
end

function utils:close (...)
  for _, f in next, self:gettable (...) do
    for k, _ in next, files do
      if files[f] and k == f then
        files[f] = not assert (files[f]:close ())
      end
    end
  end
end

function utils:reader (f, b)
  if f then
    return f:read (b or defaults.bytes)
  end
end

local randbytes = {
  generate = function (f, ...)
    if f then
      local n, m = 0, select (2, ...) or defaults.mask
      local s = utils:reader (f, select (1, ...))

      for i = 1, s:len () do
        n = m * n + s:byte (i)
      end

      return n
    end
  end
}

function randbytes:open (...)
  utils:open (...)
  return self
end

function randbytes:close (...)
  utils:close (...)
  return self
end

function randbytes:uread (...)
  return utils:reader (files.urandom, ...)
end

function randbytes:read (...)
  return utils:reader (files.random, ...)
end

function randbytes:urandom (...)
  return self.generate (files.urandom, ...)
end

function randbytes:random (...)
  return self.generate (files.random, ...)
end

function randbytes:setdefault (k, v)
  defaults[k] = v or defaults[k]
  return defaults[k]
end

utils:open ()

return setmetatable (randbytes, {
  __call = function (t, ...)
    return utils:reader (files[defaults.file], ...)
  end,
  __metatable = false,
  __newindex = function () return false end
})
