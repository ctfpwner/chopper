// RUN: %llvmgcc %s -emit-llvm -O0 -c -o %t.bc
// RUN: rm -rf %t.klee-out
// RUN: %klee --output-dir=%t.klee-out -search=dfs -skip-functions=f,g %t.bc > %t.out 2>&1
// RUN: FileCheck %s -input-file=%t.out -check-prefix=CHECK-PATHS -check-prefix=CHECK-STATES -check-prefix=CHECK-SLICES -check-prefix=CHECK-SNAPSHOTS
// RUN: FileCheck %s -input-file=%t.out -check-prefix=CHECK-A
// RUN: FileCheck %s -input-file=%t.out -check-prefix=CHECK-ANOT
// RUN: FileCheck %s -input-file=%t.out -check-prefix=CHECK-B
// RUN: FileCheck %s -input-file=%t.out -check-prefix=CHECK-BNOT

// CHECK-PATHS: KLEE: done: completed paths = 5
// CHECK-STATES: KLEE: done: recovery states = 6
// CHECK-SLICES: KLEE: done: generated slices = 2
// CHECK-SNAPSHOTS: KLEE: done: created snapshots = 2

// CHECK-A:k is 3
// CHECK-ANOT:k is not 3
// CHECK-B:y is gt 2
// CHECK-BNOT:y is not gt 2

#include <stdio.h>
#include <assert.h>
#include <klee/klee.h>

typedef struct {
    int x;
    int y;
} point;

void f(point *o, int *a) {
	if (*a > 0)
		o->x++;
	else
		o->x--;
}

void g(point *o, int *b) {
	if (*b > 0)
		o->y = o->x * 2 + 1;
	else
		o->y = o->x + 1;
}

int main(int argc, char *argv[], char *envp[]) {
    point o = {
        .x = 1,
        .y = 0
    };
    int a,b,k;

    klee_make_symbolic(&a, sizeof(a), "a");
    klee_make_symbolic(&b, sizeof(b), "b");
    klee_make_symbolic(&k, sizeof(k), "k");

    f(&o, &a);
    if (k == 3) {
    	printf("k is 3\n");
    } else {
    	printf("k is not 3\n");
    	g(&o, &b);
        if (o.y > 2) {
            printf("y is gt 2\n"); 
        } else {
            printf("y is not gt 2\n"); 
        }
    }

    return 0;
}
