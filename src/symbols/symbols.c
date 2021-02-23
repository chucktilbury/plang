/**
 * @file symbols/symbols.c
 * @brief Symbol table
 *
 * Symbols are kept in a tree structure. The left and right pointers point to
 * "siblings" and the child pointer points to symbols that the current name
 * "owns". This is used to help resolve whether a symbol is "in scope" or not.
 *
 * The symbol table always has a "root" symbol that can be used on code to
 * specify that the following symbol segments are from the "root" of the tree.
 *
 * All symbols are stored by their root name. For example, a symbol like
 * "name.asdf.plart" has "plart" as its root. All of the type information and
 * value information, if any, is stored by that name in the tree.
 *
 */
#include "common.h"
#include "scanner.h"

#include "symbols.h"

// Symbol table root pointer.
static symbol_t* root;

// Symbol table context stack.
typedef struct _symbol_stack_t {
    symbol_t* sym;
    struct _symbol_stack_t* next;
} symbol_stack_t;

symbol_stack_t* sym_stack;

/**
 * @brief Destroy the symbol stack.
 * 
 */
static void destroy_symbol_stack() {

    if(sym_stack != NULL) {
        symbol_stack_t* sym;
        symbol_stack_t* next;

        for(sym = sym_stack; sym != NULL; sym = next) {
            next = sym->next;
            FREE(sym);
        }
        FREE(sym_stack);
    }
}

/**
 * @brief Init the symbol stack.
 * 
 */
static void init_symbol_stack() {

    assert(sym_stack == NULL);
}

/**
 * @brief Destroy a single symbol.
 * 
 * @param sym 
 */
static void destroy_symbol(symbol_t* sym) {

    if(sym != NULL) {
        if(sym->name != NULL)
            FREE(sym->name);
        FREE(sym);
    }
}

/**
 * @brief Recursive function to destroy tree.
 * 
 */
void destroy_tree(symbol_t* sym) {

    if(sym->child != NULL)
        destroy_tree(sym->child);
    if(sym->left != NULL)
        destroy_tree(sym->left);
    if(sym->right != NULL)
        destroy_tree(sym->right);
    destroy_symbol(sym);
}

/**
 * @brief This is called by the atexit() routine.
 * 
 */
static void destroy_table() {

    destroy_tree(root);
}

/**
 * @brief This stores the "root" symbol and sets up the destroy function.
 * 
 */
void init_symbol_table() {

    init_symbol_stack();

    // fill out the symbol
    symbol_t* sym = create_symbol("root");
    sym->name_type = SYM_NO_NAME_TYPE;
    sym->assign_type = SYM_NO_ASSIGN_TYPE;
    sym->scope = SYM_PUBLIC_SCOPE_TYPE;

    // assign the root
    root = sym;
    push_symbol_context(sym);

    atexit(destroy_symbol_stack);
    atexit(destroy_table);
}

/**
 * @brief Create a symbol object.
 *
 * Allocate memory for a symbol and assign the name. The caller then will fill
 * in the data structure as required. The name may be NULL when the situation 
 * calls for that, but trying to store a symbol with a NULL name will cause a 
 * fatal error.
 * 
 * @param name 
 * @return symbol_t* 
 */
symbol_t* create_symbol(const char* name) {

    assert(name != NULL);

    symbol_t* sym = ALLOC_DS(symbol_t);
    if(name != NULL)
        sym->name = STRDUP(name);

    return sym;
}

/**
 * @brief Store the completed symbol into the table.
 *
 * Store the completed symbol into the table according to the current symbol
 * context on the top of the context stack. The symbol will be a "sibling" to
 * the current context. Returns SYM_NO_ERROR if the symbol was stored, 
 * otherwise returns an error code.
 * 
 * @param sym 
 * @return symbol_error_t 
 */
symbol_error_t store_symbol(symbol_t* sym) {

    assert(sym != NULL);
    assert(sym_stack != NULL);

    symbol_t* tmpsym = sym_stack->sym;
    
    while(tmpsym != NULL) {
        int cmp = strcmp(sym->name, tmpsym->name);
        if(cmp == 0)
            return SYM_EXISTS;
        else if(cmp < 0) {
            if(tmpsym->left == NULL) {
                tmpsym->left = sym;
                return SYM_NO_ERROR;
            }
            else
                tmpsym = tmpsym->left;
        }
        else {
            if(tmpsym->right == NULL) {
                tmpsym->right = sym;
                return SYM_NO_ERROR;
            }
            else
                tmpsym = tmpsym->right;
        }
    }

    fatal_error("invalid state in store_symbol()");
    return SYM_ERROR; // should never happen
}

/**
 * @brief Find a symbol in the current symbol context.
 *
 * This does not have the ability to change the context, as the resolver does. 
 * The children of the current context is also searched. The symbol data 
 * structure is returned if it is found, otherwise, return NULL. Does not 
 * return the symbol context.
 * 
 * @param name 
 * @return symbol_t* 
 */
symbol_t* find_symbol(const char* name) {

    assert(name != NULL);
    assert(sym_stack != NULL);

    symbol_t* retv = sym_stack->sym;

    while(retv != NULL) {
        int cmp = strcmp(name, retv->name);
        if(cmp == 0)
            break;
        else
            retv = (cmp < 0)? retv->left: retv->right;
    }

    return retv;
}

/**
 * @brief Push the current symbol on the context stack.
 * 
 * Push the current symbol on the context stack. This is to be called when a
 * new symbol context is to become the current context, including anonymous
 * contexts. A new context is pushed whenever a '{' token is read.
 *
 * @param sym 
 */
void push_symbol_context(symbol_t* sym) {

    assert(sym != NULL);
    assert(sym_stack != NULL);

    symbol_stack_t* stk = ALLOC_DS(symbol_stack_t);

    stk->sym = sym;
    stk->next = sym_stack;
    sym_stack = stk;
}

/**
 */

/**
 * @brief Pop the symbol context.
 *
 *  The symbol context is popped when a '}' token is read.
 */
symbol_t* pop_symbol_context() {

    assert(sym_stack != NULL);

    symbol_t* sym = sym_stack->sym;
    symbol_stack_t* sst = sym_stack;

    sym_stack = sym_stack->next;
    FREE(sst);

    return sym;
}

/**
 * @brief Return the top of the symbol context stack, but don't pop it.
 * 
 * @return symbol_t* 
 */
symbol_t* peek_symbol_context() {

    return sym_stack->sym;
}
