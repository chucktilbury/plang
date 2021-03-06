/**
    @file symbols/symbols.c
    
    @brief Symbol table 
    
    Symbols are kept in a tree structure. The left and right pointers point to
    "siblings" and the child pointer points to symbols that the current name
    "owns". This is used to help resolve whether a symbol is "in scope" or not.

    The symbol table always has a "root" symbol that can be used on code to
    specify that the following symbol segments are from the "root" of the tree.

    All symbols are stored by their root name. For example, a symbol like
    "name.asdf.plart" has "plart" as its root. All of the type information and
    value information, if any, is stored by that name in the tree.

**/
#include "common.h"
#include "scanner.h"

#include "symbols.h"

// TODO: open context is automatically called for symbol types that require it.

// Symbol table root pointer.
symbol_t* root_context = NULL;  // shared with the resolver
static symbol_context_t* current_context = NULL;
static int context_serial = 0;

/**
    @brief Destroy a single symbol.
    
    @param sym 
**/
static void destroy_symbol(symbol_t* sym) {

    if(sym != NULL) {
        if(sym->name != NULL)
            FREE(sym->name);
        FREE(sym);
    }
}

/**
    @brief Destroy symbol tree object
    
    @param sym 
**/
static void destroy_symbol_tree(symbol_t* sym) {

    if(sym->left != NULL)
        destroy_symbol_tree(sym->left);
    if(sym->right != NULL)
        destroy_symbol_tree(sym->right);
    destroy_symbol(sym);
}

/**
    @brief Recursive function to destroy tree.
    
    @param cont 
**/
void destroy_tree(symbol_context_t* cont) {

    if(cont != NULL) {
        if(cont->sym != NULL)
            destroy_symbol_tree(cont->sym);
        if(cont->next != NULL)
            destroy_tree(cont->next);
    }
}

/**
    @brief This is called by the atexit() routine.
    
**/
static void destroy_table() {

    destroy_tree(root_context->context);
    destroy_symbol(root_context);
}

/*****************************************************
    Interface
*****************************************************/

/**
    @brief This stores the "root" symbol and sets up the destroy function.
    
**/
void init_symbol_table() {

    assert(root_context == NULL);
    assert(current_context == NULL);

    // fill out the symbol
    symbol_t* sym = create_symbol("object");
    sym->name_type = SYM_NO_NAME_TYPE;
    sym->assign_type = SYM_NO_ASSIGN_TYPE;
    sym->scope = SYM_PUBLIC_SCOPE_TYPE;

    root_context = sym;
    symbol_context_t* cont = ALLOC_DS(symbol_context_t);

    // assign the root context
    sym->context = cont;
    current_context = cont;
    //open_symbol_context(NULL);

    atexit(destroy_table);
}

/**
    @brief Create a symbol object
    
    Allocate memory for a symbol and assign the name. The caller then will fill
    in the data structure as required. The name may be NULL when the situation 
    calls for that, but trying to store a symbol with a NULL name will cause a 
    fatal error.

    @param name 
    @return symbol_t* 

**/
symbol_t* create_symbol(const char* name) {

    symbol_t* sym = ALLOC_DS(symbol_t);
    if(name != NULL)
        sym->name = STRDUP(name);

    return sym;
}

/**
    @brief Store the completed symbol into the table.
    
    Store the completed symbol into the table according to the current symbol
    context on the top of the context stack. The symbol will be a "sibling" to
    the current context. Returns SYM_NO_ERROR if the symbol was stored, 
    otherwise returns an error code.

    @param sym 
    @return symbol_error_t 

**/
symbol_error_t store_symbol(symbol_t* sym) {

    assert(sym != NULL);
    assert(sym->name != NULL);
    assert(current_context != NULL);

    symbol_t* tmpsym = current_context->sym;
    
    if(tmpsym == NULL) {
        current_context->sym = sym;
        return SYM_NO_ERROR;
    }
    else {
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
    }

    fatal_error("invalid state in store_symbol()");
    return SYM_ERROR; // should never happen
}

/**
    @brief Find a symbol in the current symbol context.
    
    This does not have the ability to change the context, as the resolver does. 
    The children of the current context is also searched. The symbol data 
    structure is returned if it is found, otherwise, return NULL. Does not 
    return the symbol context.

    @param name 
    @return symbol_t* 

**/
symbol_t* find_symbol(const char* name) {

    assert(name != NULL);
    assert(current_context != NULL);

    symbol_t* retv = current_context->sym;

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
    @brief Create a context entry and add it to the symbol.

    Links the context entry to the previous context so that it can be traced
    by the resolver.
    
    @param sym 

**/
static void add_context(symbol_t* sym) {

    assert(current_context != NULL);
    assert(sym != NULL);
    // cannot open symbol table if one is already open on this name
    assert(sym->context == NULL);

    symbol_context_t* cont = ALLOC_DS(symbol_context_t);

    //printf("cont NULL cont = %p\n", cont);
    sym->context = cont;
    cont->prev = current_context;
    current_context->next = cont;
    current_context = cont;
}

/**
    @brief Create and store a new symbol table context. 
    
    If the name is NULL, then create an anonymous context in the current one. 
    Otherwise, find the name in the current context and open a context on it. 

    @param name 
    @return symbol_error_t 

**/
symbol_error_t open_symbol_context(const char* name) {

    if(name != NULL) {
        // create a context on the name given and make it the current one
        symbol_t* sym = find_symbol(name);

        if(sym == NULL) {
            syntax("Cannot open context: symbol '%s' is not defined.", name);
            return SYM_NOT_FOUND;
        }
        else {
            add_context(sym);
        }
    }
    else {
        // create an anonymous context on the current one and make it current
        char tbuf[10];
        sprintf(tbuf, "%09d", context_serial++);
        symbol_t* sym = create_symbol(tbuf);
        store_symbol(sym);
        add_context(sym);
    }

    return SYM_NO_ERROR;
}

/**
    @brief Revert the current context back to the previous one. 
    
    If the previous context is the root context, then there is no context to 
    close, so return SYM_CONTEXT_ERROR. Otherwise, return SYM_NO_ERROR.

    @return symbol_error_t 

**/
symbol_error_t close_symbol_context() {

    assert(current_context != NULL);
    assert(current_context->prev != NULL);

    symbol_t* sym = current_context->prev->sym;

    if(!strcmp(sym->name, "object")) 
        return SYM_CONTEXT_ERROR;
    else
        current_context = current_context->prev;

    context_serial = 0;
    return SYM_NO_ERROR;
}

/**
    @brief Get the symbol object in the current table context.
    
    @return symbol_t* 
    
**/
symbol_t* get_symbol_context() {

    return current_context->prev->sym;
}

/**
    @brief print out the whole symbol table
    
**/
static int dump_indent = 0;
static void print_sym(symbol_t* sym) {

    for(int i = 0; i < dump_indent; i++)
        fputc('-', stdout);

    printf("%s\n", sym->name);
}

static void dump_symbols(symbol_t* sym) {

    if(sym != NULL) {
        if(sym->left != NULL)
            dump_symbols(sym->left);
        if(sym->right != NULL)
            dump_symbols(sym->right);
        print_sym(sym);
        
        if(sym->context != NULL) {
            dump_indent++;
            dump_symbols(sym->context->sym);
            dump_indent--;
        }
    }
}

/**
    @brief For debugging. Dump a list of everything in the symbol table.
    
**/
void dump_symbol_table() {

    dump_symbols(root_context);
}
