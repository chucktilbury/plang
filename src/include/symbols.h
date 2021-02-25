/**
 * @file symbols.h
 * @brief Header file for symbols.c
 */
#ifndef __SYMBOLS_H__
#define __SYMBOLS_H__

#include <stdint.h>

typedef enum {
    SYM_NO_ERROR = 300, // Success
    SYM_EXISTS,         // Failed to add because the name already exists
    SYM_NOT_FOUND,      // Failed to find the name.
    SYM_CONTEXT_ERROR,  // Sym table context could not be closed.
    SYM_ERROR,          // Covers things like an invalid state. Normally fatal.
} symbol_error_t;

typedef enum {
    // no assignment type is allowed, trying to assign to something
    // generates a syntax error.
    SYM_NO_ASSIGN_TYPE = 325,
    // These are used when the assignment type is a literal constant.
    SYM_INT_ASSIGN_TYPE,
    SYM_UINT_ASSIGN_TYPE,
    SYM_FLOAT_ASSIGN_TYPE,
    SYM_BOOL_ASSIGN_TYPE,
    SYM_STRING_ASSIGN_TYPE,
    // These indicate that an indirection is needed to retrieve a value
    // for the name.
    SYM_DICT_ASSIGN_TYPE,
    SYM_MAP_ASSIGN_TYPE,
    SYM_LIST_ASSIGN_TYPE,
    // These two are handled the same except that the SYM_INHERIT_TYPE
    // type has a pointer to the symbol that the class inherited from
    // in the value union. The SYM_CLASS_TYPE means that there is no
    // base class.
    SYM_CLASS_ASSIGN_TYPE,
    SYM_INHERIT_ASSIGN_TYPE,
    // The assigment type is an expression, which could be any type and the
    // actual assignment type is in the expression data structure.
    SYM_EXPR_ASSIGN_TYPE,
} assignment_type_t;

typedef enum {
    SYM_NO_NAME_TYPE = 350,
    SYM_CLASS_NAME_TYPE,
    SYM_METHOD_NAME_TYPE,
    SYM_VAR_NAME_TYPE,
    SYM_CONST_NAME_TYPE,
    SYM_IMPORT_NAME_TYPE,
    // name is a system-wide serial number and is only accessed at the top of
    // the symbol table stack.
    SYM_ANON_NAME_TYPE,
} name_type_t;

typedef enum {
    // If the scope is not specified, then the scope is private.
    SYM_PUBLIC_SCOPE_TYPE = 375,
    SYM_PRIVATE_SCOPE_TYPE,
    SYM_PROTECTED_SCOPE_TYPE,
} symbol_scope_t;

// When a context is opened, this data structure is stored in the current 
// context. A pointer to the previous context is stored and the next 
// context is NULL. The previous context's next pointer is updated to 
// point to this context. This allows the resolver to move back and forth in
// the contexts that have been saved. No symbol is saved by opening a context.
typedef struct _symbol_context_t {
    struct _symbol_t* sym;          // tree that holds symbols for this context
    struct _symbol_context_t* prev; // pointer to the symbol that holds the previous context
    struct _symbol_context_t* next; // pointer to the symbol that holds the next context
} symbol_context_t;

typedef struct _symbol_t {
    // symbol attributes
    const char* name;       // search name of the symbol.
    name_type_t name_type;
    assignment_type_t assign_type;
    symbol_scope_t scope;

    // constant value, if any
    union {
        // These hold a literally defined value that is connected to the name.
        uint64_t uint_val;
        int64_t int_val;
        double float_val;
        char* str_val;
        // When the assignment object is a user-defined type or a variable,
        // this points to the symbol table entry.
        struct _symbol_t* symbol;
        // TODO: add data structure for expressions.
    } const_val;

    // pointers for the tree
    struct _symbol_t* left;
    struct _symbol_t* right;
    symbol_context_t* context;
} symbol_t;

// defined in symbols.c
void init_symbol_table();
symbol_t* create_symbol(const char* name);
symbol_error_t store_symbol(symbol_t* sym);
symbol_t* find_symbol(const char* name);
symbol_error_t open_symbol_context(const char* name);
symbol_error_t close_symbol_context();
symbol_t* get_symbol_context();
void dump_symbol_table();

// defined in resolver.c


#endif
