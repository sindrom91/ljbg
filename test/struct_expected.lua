local ffi = require("ffi")

ffi.cdef[[
struct s {
    int x;
};
typedef struct {
    int x;
} ts;
]]
