//
//  FFI_Class.c
//

#include "FFI_Class.h"

static void _ffi_class_currying__function(ffi_cif *cif, void *ret, void **args, void *ctx) {
    // jump into our new function
    _ffi_class_curried_function *fn = ctx;
    
    void **newargs = malloc(sizeof(void *) * (cif->nargs + 1));
    newargs[0] = &fn->argument;
    
    memcpy(&(newargs[1]), args, sizeof(void *) * cif->nargs);
    
    ffi_call(fn->target_cif, fn->target, ret, newargs);
}

void *_ffi_class_curry_function(_ffi_class_curried_function *fn, void *fptr, void *arg, int argc, ffi_type *rtype, ffi_type *atypes[]) {
    // executable address from the closure
    void *closurePointer = NULL;
    
    fn->closure_argtypes = malloc(sizeof(ffi_type *) * argc);
    memcpy(fn->closure_argtypes, atypes, sizeof(ffi_type *) * argc);
    
    fn->target_argtypes = malloc(sizeof(ffi_type *) * (argc + 1));
    fn->target_argtypes[0] = ffi_type_of(arg);
    
    memcpy(&(fn->target_argtypes[1]), atypes, sizeof(ffi_type *) * argc);
    
    fn->closure_cif = calloc(sizeof(ffi_cif), 1);
    fn->target_cif = calloc(sizeof(ffi_cif), 1);
    
    // allocate our closure
    fn->closure = ffi_closure_alloc(sizeof(ffi_closure), &closurePointer);
    
    // prepare the cifs
    ffi_prep_cif(fn->closure_cif, FFI_DEFAULT_ABI, argc, rtype, fn->closure_argtypes);
    ffi_prep_cif(fn->target_cif, FFI_DEFAULT_ABI, argc + 1, rtype, fn->target_argtypes);
    
    // setup the closure
    ffi_prep_closure_loc(fn->closure, fn->closure_cif, _ffi_class_currying__function, fn, closurePointer);
    
    fn->argument = arg;
    fn->target = fptr;
    
    return closurePointer;
}

extern void _ffi_class_destroy_curried_function(_ffi_class_curried_function *fn) {
    ffi_closure_free(fn->closure);
    free(fn->target_cif);
    free(fn->closure_cif);
    free(fn->target_argtypes);
    free(fn->closure_argtypes);
    
    fn->target = NULL;
    fn->argument = NULL;
}