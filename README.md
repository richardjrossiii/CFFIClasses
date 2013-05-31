##CFFIClasses
Adding classes to C via libffi & P99

###Goals:

The goal of this project was to create a simple API for defining C++-like classes in GNU C, without overencumbering it with a large runtime. It does not aim to have traditional polymorphism whatsoever, and is more of a mash-up of macros than anything else.

###Features:

 - Constructor and destructor support
 - Curry `me` parameter using libffi
 
###Limitations:

 - Currently can only have 10 arguments passed to a method
 - Does not support raw `struct`s in argument lists (or return types), only pointers
 - Methods must cast `me` to the proper type in any method other than the constructor or destructor
 - Relies upon many features of GNU C (statement expressions, `typeof`, etc.)
 - Probably not very portable (only tested on 64 bit Mac OSX)
 - Only one class per implementation file.

###Roadmap:

 - Nothing currently. This was just a simple side-project, I don't expect it to amount to much. Patches are welcome, though!
 
###Compiling:

 - Couldn't be simpler. Simply run the following command in your favorite terminal:
 
        clang *.c libffi.a -o ffi_classes

    And run the executable now contained in `ffi_classes`.

###Examples:

 - Creating a class is simple, just use the `FFI_CLASS` construct in the header, and `FFI_CLASS_IMPL` in the implementation (`.c`). You can use it as follows:
  
    **Foo.h:**

        #include "FFI_Class.h"
        
        FFI_CLASS(Foo,
            FFI_METHOD(void, foo),
            FFI_METHOD(int, bar, int)
        );
    
    **Foo.c:**
    
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

 - Using a class is just as simple:
 
    **main.c:**

        #include "Foo.h"

        int main() {
            FooType foo = FooClass.create();
        
            foo->foo();
            printf("result of bar is: %i\n", foo->bar(10));
        
            foo->destroy();
        }
        
###License:

Copyright 2013 Richard J. Ross III.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
