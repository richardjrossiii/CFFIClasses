//
//  FFI_Class.h
//

#ifndef _FFI_CLASS_H
#define _FFI_CLASS_H

#include "ffi/ffi.h"
#include "p99/p99.h"

#define FFI_CONSTRUCTOR_IMPL(impl) FFI_CONSTRUCTOR_IMPL(impl)
#define FFI_DESTRUCTOR_IMPL(impl) FFI_DESTRUCTOR_IMPL(impl)

#define FFI_METHOD(ret, name, argtypes...) FFI_METHOD(ret, name, ##argtypes)
#define FFI_IVAR(type, name) FFI_IVAR(type, name)

#define _HEADER_FFI_METHOD(ret, name, argtypes...) const ret (*name)(argtypes)
#define _HEADER_FFI_IVAR(type, name) /* no need for ivars in the header */

// a helper for easily defining a FFI class header
#define FFI_CLASS(name, methods...)\
typedef struct {\
    void (*destroy)();\
    _FFI_PREFIX_APPLY(_HEADER_, methods);\
    \
    unsigned char internal[0];\
} *name ## Type;\
extern const struct {\
    const name ## Type (*create)();\
} name ## Class\

#define cic(a, b, c, d) (__builtin_choose_expr(__builtin_types_compatible_p(typeof(a), b), c, d))

#define ffi_type_of(x) (\
    cic(x, signed char,     &ffi_type_schar,\
    cic(x, unsigned char,   &ffi_type_uchar,\
    cic(x, signed short,    &ffi_type_sshort,\
    cic(x, unsigned short,  &ffi_type_ushort,\
    cic(x, signed int,      &ffi_type_sint,\
    cic(x, unsigned int,    &ffi_type_uint,\
    cic(x, signed long,     &ffi_type_slong,\
    cic(x, unsigned long,   &ffi_type_ulong,\
    cic(x, float,           &ffi_type_float,\
    cic(x, double,          &ffi_type_double,\
    cic(x, void *,          &ffi_type_pointer,\
                            &ffi_type_void\
))))))))))))

#define ffi_type_of_name(x) ffi_type_of(typeof(({ __attribute__((unused)) x, _ffi_decl; _ffi_decl; })))

#define __ffi_num_args(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, VAL, ...) VAL
#define _ffi_num_args(...) __ffi_num_args(0, ##__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

#define __ffi_type_array_10(arg, args...) ffi_type_of_name(arg), __ffi_type_array_9(args)
#define __ffi_type_array_9(arg, args...)  ffi_type_of_name(arg), __ffi_type_array_8(args)
#define __ffi_type_array_8(arg, args...)  ffi_type_of_name(arg), __ffi_type_array_7(args)
#define __ffi_type_array_7(arg, args...)  ffi_type_of_name(arg), __ffi_type_array_6(args)
#define __ffi_type_array_6(arg, args...)  ffi_type_of_name(arg), __ffi_type_array_5(args)
#define __ffi_type_array_5(arg, args...)  ffi_type_of_name(arg), __ffi_type_array_4(args)
#define __ffi_type_array_4(arg, args...)  ffi_type_of_name(arg), __ffi_type_array_3(args)
#define __ffi_type_array_3(arg, args...)  ffi_type_of_name(arg), __ffi_type_array_2(args)
#define __ffi_type_array_2(arg, args...)  ffi_type_of_name(arg), __ffi_type_array_1(args)
#define __ffi_type_array_1(arg, args...)  ffi_type_of_name(arg), __ffi_type_array_0(args)
#define __ffi_type_array_0(...)

#define __ffi_type_array(narg, args...) __ffi_type_array_ ## narg (args)
#define _ffi_type_array(narg, args...) __ffi_type_array(narg, ##args)
#define ffi_type_array(args...) _ffi_type_array(_ffi_num_args(args), ##args)

#define FFI_CURRY_FUNCTION(resaddr, fptr, arg, rtype, p_arg_types...) ({\
    ffi_type *closure_types[] = { ffi_type_array(p_arg_types) };\
    _ffi_class_curry_function(resaddr, fptr, arg, sizeof(closure_types) / sizeof(*closure_types), ffi_type_of(rtype), closure_types);\
})

#define __FFI_PREFIX_APPLY(prefix, arg, index) P99_PASTE(prefix, arg)
#define _FFI_PREFIX_APPLY(prefix, args...) P99_FOR(prefix, P99_NARG(args), P00_SEP, __FFI_PREFIX_APPLY, args)

#define _IMPL_FFI_CONSTRUCTOR_IMPL(impl) /* no need for constructor here */
#define _IMPL_FFI_DESTRUCTOR_IMPL(impl) /* no need for destructor here */
#define _IMPL_FFI_METHOD(ret, name, argtypes...) ret (*name)(argtypes);
#define _IMPL_FFI_METHOD_IMPL(method, impl) _IMPL_ ## method
#define _IMPL_FFI_IVAR(type, name) type name;

#define _IMPL_CURRIED_FFI_CONSTRUCTOR_IMPL(impl) /* no need for constructor here */
#define _IMPL_CURRIED_FFI_DESTRUCTOR_IMPL(impl) /* no need for destructor here */
#define _IMPL_CURRIED_FFI_METHOD(ret, name, argtypes...) _ffi_class_curried_function name ## _fn;
#define _IMPL_CURRIED_FFI_METHOD_IMPL(method, impl) _IMPL_CURRIED_ ## method
#define _IMPL_CURRIED_FFI_IVAR(type, name)

#define _HEADER_IMPL_FFI_CONSTRUCTOR_IMPL(impl) /* no need for constructor here */
#define _HEADER_IMPL_FFI_DESTRUCTOR_IMPL(impl) /* no need for destructor here */
#define _HEADER_IMPL_FFI_METHOD(ret, name, argtypes...) static ret _ffi_class_ ## name ## _impl(void *me, ##argtypes)
#define _HEADER_IMPL_FFI_METHOD_IMPL(method, impl) _HEADER_IMPL_ ## method
#define _HEADER_IMPL_FFI_IVAR(type, name) /* no need for ivars here */

#define _ACTUAL_IMPL_FFI_CONSTRUCTOR_IMPL(impl) /* no need for constructor here */
#define _ACTUAL_IMPL_FFI_DESTRUCTOR_IMPL(impl) /* no need for destructor here */
#define _ACTUAL_IMPL_FFI_METHOD_IMPL(method, impl) _HEADER_IMPL_FFI_METHOD_IMPL(method, impl) impl
#define _ACTUAL_IMPL_FFI_IVAR(type, name) /* no need for ivars here */

#define _CONSTRUCTOR_IMPL_FFI_CONSTRUCTOR_IMPL(impl) impl
#define _CONSTRUCTOR_IMPL_FFI_DESTRUCTOR_IMPL(impl) /* no need for destructor here */
#define _CONSTRUCTOR_IMPL_FFI_METHOD(ret, name, argtypes...) me->name = FFI_CURRY_FUNCTION(&(me->name ## _fn), (void *) _ffi_class_ ## name ## _impl, me, ret, ##argtypes)
#define _CONSTRUCTOR_IMPL_FFI_METHOD_IMPL(method, impl) _CONSTRUCTOR_IMPL_ ## method
#define _CONSTRUCTOR_IMPL_FFI_IVAR(type, name) /* no need for ivars here */

#define _DESTRUCTOR_IMPL_FFI_CONSTRUCTOR_IMPL(impl) /* no need for constructor here */
#define _DESTRUCTOR_IMPL_FFI_DESTRUCTOR_IMPL(impl) impl /* run the custom destructor */
#define _DESTRUCTOR_IMPL_FFI_METHOD(ret, name, argtypes...) _ffi_class_destroy_curried_function(&(me->name ## _fn))
#define _DESTRUCTOR_IMPL_FFI_METHOD_IMPL(method, impl) _DESTRUCTOR_IMPL_ ## method
#define _DESTRUCTOR_IMPL_FFI_IVAR(type, name) /* no need to destroy ivars manually */

#define FFI_METHOD_IMPL(method, impl) FFI_METHOD_IMPL(method, impl)
#define FFI_CLASS_IMPL(name, methods...)\
_FFI_PREFIX_APPLY(_HEADER_IMPL_, methods)\
typedef struct {\
    void (*destroy)();\
    _FFI_PREFIX_APPLY(_IMPL_, methods)\
    \
    _ffi_class_curried_function destroy_fn;\
    _FFI_PREFIX_APPLY(_IMPL_CURRIED_, methods)\
} *name ## Type;\
struct {\
    name ## Type (*create)();\
} name ## Class;\
_FFI_PREFIX_APPLY(_ACTUAL_IMPL_, methods)\
static void _ffi_class_destroy_impl(FooType me) {\
    _FFI_PREFIX_APPLY(_DESTRUCTOR_IMPL_, methods)\
    \
    free(me);\
}\
static name ## Type _ffi_class_ ## name ## _create() {\
    name ## Type me = calloc(1, sizeof(*me));\
    \
    _FFI_PREFIX_APPLY(_CONSTRUCTOR_IMPL_, methods)\
    _CONSTRUCTOR_IMPL_FFI_METHOD(void, destroy);\
    \
    return me;\
}\
__attribute__((constructor)) static void _ffi_class_ ## name ## _init() {\
    name ## Class.create = _ffi_class_ ## name ## _create;\
}

// the struct containing all of our curried information.
typedef struct {
    // the argument we are passing to the 'real' target
    void *argument;
    
    // the target of this message - a function pointer
    void *target;
    
    // the argument types of this closure (which must be free'd when destroyed)
    ffi_type **closure_argtypes;
    
    // the interface of this closure (which must be free'd when destroyed)
    ffi_cif *closure_cif;
    
    // the actual closure of this interface
    ffi_closure *closure;
    
    // the argument types of the target (must be free'd when destroyed)
    ffi_type **target_argtypes;
    
    // the calling interface of the target (must be free'd when destroyed)
    ffi_cif *target_cif;
} _ffi_class_curried_function;

// this is a behind the scenes method, used for actually currying the function
extern void *_ffi_class_curry_function(_ffi_class_curried_function *fn, void *fptr, void *arg, int argc, ffi_type *rtype, ffi_type *atypes[]);
extern void _ffi_class_destroy_curried_function(_ffi_class_curried_function *fn);

#endif
