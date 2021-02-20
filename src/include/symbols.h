
#ifndef __SYMBOLS_H__
#define __SYMBOLS_H__

#include <stdint.h>

typedef enum {
    SYM_NO_ERROR = 300,
    SYM_EXISTS,
    SYM_NOT_FOUND,
    SYM_ERROR,
} symbol_error_t;

typedef enum {
    // no assignment type is allowed
    SYM_NO_ASSIGN_TYPE = 325,
    SYM_INT_ASSIGN_TYPE,
    SYM_UINT_ASSIGN_TYPE,
    SYM_FLOAT_ASSIGN_TYPE,
    SYM_BOOL_ASSIGN_TYPE,
    SYM_STRING_ASSIGN_TYPE,
    SYM_DICT_ASSIGN_TYPE,
    SYM_MAP_ASSIGN_TYPE,
    SYM_LIST_ASSIGN_TYPE,
    // These two are handled the same except that the SYM_INHERIT_TYPE
    // type has a pointer to the symbol that the class inherited from
    // in the value union. The SYM_CLASS_TYPE means that there is no
    // base class.
    SYM_CLASS_ASSIGN_TYPE,
    SYM_INHERIT_ASSIGN_TYPE,
} assignment_type_t;

typedef enum {
    SYM_CLASS_NAME_TYPE = 350,
    SYM_METHOD_NAME_TYPE,
    SYM_VAR_NAME_TYPE,
    SYM_CONST_NAME_TYPE,
    SYM_IMPORT_NAME_TYPE,
    // name is a system-wide serial number and is only accessed at the top of
    // the symbol table stack.
    SYM_ANON_NAME_TYPE,
} name_type_t;

typedef enum {
    SYM_PUBLIC_SCOPE_TYPE = 375,
    SYM_PRIVATE_SCOPE_TYPE,
    SYM_PROTECTED_SCOPE_TYPE,
} symbol_scope_t;

typedef struct _symbol_t {

    // symbol attributes
    const char* name;
    name_type_t name_type;
    assignment_type_t assign_type;
    symbol_scope_t scope;

    // constant value, if any
    union {
        uint64_t uint_val;
        int64_t int_val;
        double float_val;
        char* str_val;
        struct _symbol_t* symbol;
    } const_val;

    // pointers for the tree
    struct _symbol_t* left;
    struct _symbol_t* right;
    struct _symbol_t* child;
} symbol_t;

// defined in symbols.c
void init_symbol_table();
symbol_error_t add_symbol();
symbol_error_t update_symbol(const char*, symbol_t*);
symbol_error_t get_symbol(const char* name, symbol_t* sym);

// defined in resolver.c


#endif
