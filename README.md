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
ljbg <input> [-o/--output <output>] [--args <arg> <arg> ...]

Arguments are expected in this exact order!
```
