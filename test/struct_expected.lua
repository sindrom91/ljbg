local ffi = require("ffi")

ffi.cdef[[
struct s {
    int x;
};
typedef struct {
    int x;
} ts;
struct __attribute__((packed)) ps {
    int x;
    short y;
};
]]
