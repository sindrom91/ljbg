# ljbg - LuaJIT FFI bindings generator

## How to compile?

```
meson setup ./build/ --buildtype=release
meson compile -C ./build/
```

## How to run tests?

```
meson test -C ./build/
```

## How to use?

```
ljbg <input> --output <output> --args <arg> <arg> ...
```
