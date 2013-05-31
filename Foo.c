//
//  Foo.c
//

#include <stdio.h>
#include <stdlib.h>

#include "FFI_Class.h"

FFI_CLASS_IMPL(Foo,
    FFI_CONSTRUCTOR_IMPL({
        me->x = 17;
    }),
               
    FFI_METHOD_IMPL(FFI_METHOD(void, foo), {
        printf("x is: %i\n", ((FooType) me)->x);
    }),
               
    FFI_METHOD_IMPL(FFI_METHOD(int, bar, int y), {
        return ((FooType) me)->x * 2 + y;
    }),
               
    FFI_IVAR(int, x)
);
