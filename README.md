KLEE/Slicing Project
=============================
An extension of KLEE (http://klee.github.io).

## Build
Build the Slicing Library:
* https://github.com/davidtr1037/se-slicing

Build KLEE:
```
mkdir klee_build
cd klee_build
cmake \
    -DCMAKE_C_FLAGS="-m32 -g" \
    -DCMAKE_CXX_FLAGS="-m32 -g" \
    -DENABLE_SOLVER_STP=ON \
    -DENABLE_POSIX_RUNTIME=ON \
    -DENABLE_KLEE_UCLIBC=ON \
    -DKLEE_UCLIBC_PATH=<KLEE_UCLIBC_DIR> \
    -DLLVM_CONFIG_BINARY=<LLVM_CONFIG_PATH> \
    -DENABLE_UNIT_TESTS=OFF \
    -DKLEE_RUNTIME_BUILD_TYPE=Release+Asserts \
    -DENABLE_SYSTEM_TESTS=ON \
    -DSLICING_ROOT_DIR=<SLICING_PROJECT_DIR> \
    <KLEE_ROOT_DIR>
```

## Usage Example
Let's look at the following program:
```C
#include <stdio.h>

#include <klee/klee.h>

typedef struct {
    int x;
    int y;
} object_t;

void f(object_t *o) {
    o->x = 0;
    o->y = 0;
}

int main(int argc, char *argv[]) {
    object_t o;
    int k;

    klee_make_symbolic(&k, sizeof(k), "k");

    f(&o);
    if (k > 0) {
        printf("%d\n", o.x);
    } else {
        printf("%d\n", o.y);
    }

    return 0;
}
```

Compile the program:
```
clang -m32 -c -g -emit-llvm main.c -o main.bc
opt -mem2reg main.bc -o main.bc (required for better pointer analysis)
```

Run KLEE (static analysis related debug messages are written to stdout):
```
export LD_LIBRARY_PATH=<DG_BUILD_DIR>/src
klee -libc=klee -search=dfs -slice=f main.bc 1>out.log
```

Notes:
* For debugging the mechanism, add ```-debug-only=basic```.
* Currently, the supported search heuristics are: dfs, bfs.
