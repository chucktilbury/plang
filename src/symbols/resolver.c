/**
    @file resolver.c

    @brief Resolve name references in the symbol table one segment at a time.

    A complete complex name is a series of symbols that are separated by a '.'
    character. Each part of the name, not including the '.', is a name segment.

    As the resolver works, if looks in the current symbol context first for the
    first segment of the name. The search changes contexts toward the root
    until it reaches the root. If it is not found in the root, then it is not
    found. If it is found, then the next segment immediate is searched moving
    up the tree toward the leaves, changing the context on each move. If the
    symbol is not found in the next immediate segment, then it's not found.

    The resolver state is maintained by the concept of "opening" and "closing"
    a resolve. When the first segment is searched, the resolver is opened and
    when the last segment is searched, the caller closes the resolve with a
    function call. The resolver is then ready for the next resolve. The
    resolver state is maintained internally by using the back links in the
    symbol context and keeping a global "current_context" pointer.

    Note that the resolver and the store functionalities have to operate
    independently of each other, but they share the same data structure (i.e.
    the symbol table). There is no problem with re-entrancy only because the
    compiler is single threaded.

**/
#include "common.h"
#include "scanner.h"
#include "symbols.h"

typedef struct _symbol_stack_t {
    symbol_t* sym;
    struct _symbol_stack_t* next;
} symbol_stack_t;

extern symbol_t* root_context;
static symbol_stack_t* sym_stack = NULL;

static void push_sym_stack(symbol_t* sym) {

    assert(sym != NULL);
    symbol_stack_t* nss = ALLOC_DS(symbol_stack_t);
    nss->sym = sym;
    if(sym_stack != NULL)
        nss->next = sym_stack;
    sym_stack = nss;
}

static symbol_t* pop_sym_stack() {

    symbol_t* sym = NULL;

    if(symbol_stack != NULL) {
        symbol_stack_t* stk = symbol_stack;
        sym = stk->sym;
        assert(sym != NULL);
        symbol_stack = stk->next;
        FREE(stk);
    }

    return sym; // return NULL if the stack is empty
}

static symbol_t* peek_sym_stack() {

    assert(symbol_stack != NULL);
    symbol_t* sym = symbol_stack->sym;
    assert(sym != NULL);

    return sym;
}

static void clear_sym_stack() {

    while(NULL != pop_sym_stack()) { /* empty */ }
}


/*****************************************************
    Interface
*****************************************************/

/**
    @brief This must be called on a new symbol lookup.

    When a new symbol is being located, the look up is traced from the current
    context back to the root context. Each child context is searched before
    moving up to the next context. If the symbol is not found in the root
    context, then the symbol is not found. If the symbol is found, then it's
    pushed on the stack for future reference, when the symbol is semantically
    analyzed. The symbol is semantically analyzed when the resolver is closed.

    If the symbol is found, then a pointer to it is returned. If the symbol is
    not found, then a NULL pointer is returned.

    For example, the name "name1.name2.name3".
    This function is called when "name1" is encountered. The name must be a
    class or an import, but that is not known until more of the symbol is
    resolved. So if it's found, then it is simply returned.

    @param name
    @return symbol_t*

**/
symbol_t* open_resolver(const char* name) {

}

/**
    @brief This is used for looking up a compound symbol.

    This is to be called with the segment name of the symbol. The '.' character
    is not included. All of the children on the previous context are searched
    and only the children. If it is not found in the children. then it is not
    found. If it is found, then the symbol is pushed on the stack for
    validation then the current resolve is closed.

    When the symbol is found, the symbol pointer is returned. When the symbol
    is not found, then a NULL pointer is returned.

    For example, the name "name1.name2.name3".
    This function will be called against "name2" and "name3". Each symbol, if
    it is found in the immediate child context, then it is pushed on the stack
    and returned as-is.

    @param name
    @return symbol_t*

**/
symbol_t* resolve_symbol(const char* name) {

}

/**
    @brief Signal the resolver that the current symbol search is ended.

    When the current symbol is closed, then the symbol is semantically analyzed
    for validity. For example a symbol who's root is an int, but the second
    segment is a class, then it is not a valid symbol and a clear error message
    is issued, based on the values in the stack.

    When the search is closed, then the state of the resolver is reset and it's
    made ready for the beginning of the next resolve. If the symbol is valid
    then the pointer to the end symbol is returned. If the symbol is invalid,
    then NULL is returned.

    @return symbol_t*

**/
symbol_t* close_resolver() {

    clear_sym_stack()
    current_context = NULL;
}

