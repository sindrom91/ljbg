local ffi = require("ffi")

ffi.cdef[[
union u {
    int x;
    float f;
};
]]
