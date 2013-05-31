//
//  Foo.h
//

#ifndef _FOO_H
#define _FOO_H

#include "FFI_Class.h"

FFI_CLASS(Foo,
    FFI_METHOD(void, foo),
    FFI_METHOD(int, bar, int)
);

#endif
