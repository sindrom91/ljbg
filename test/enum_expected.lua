local ffi = require("ffi")

ffi.cdef[[
enum e {
    FIRST,
    SECOND
};
typedef enum  {
    THIRD,
    FOURTH
} te;
]]
