# about
this is a brainfuck interpreter.

before running a program, all brackets are validated and every
`[` / `]` pair is resolved once into a precompiled jump table.

# make dependency
cmake, gcc/clang, make

# quick start

```bash
mkdir -p build
cd build
cmake .. && make
```
Run:
```bash
./bf ../test/bfs/hello.bf
```

JUST FOR FUN
