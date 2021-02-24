/**
 * @file symbols_test.c
 * @brief Test the symbol table functionality.
 *
 * Functions to test:
 * void init_symbol_table();
 * symbol_t* create_symbol(const char*);
 * symbol_error_t store_symbol(symbol_t*);
 * symbol_t* find_symbol(const char*);
 * void push_symbol_context(symbol_t*);
 * symbol_t* pop_symbol_context();
 * symbol_t* peek_symbol_context();
 * 
 * Use a debugger to really verify this. Otherwise, it's a sanity check.
 */
#include "common.h"
#include "symbols.h"

static void print_sym(symbol_t* sym, int idx, const char* name) {

    if(sym != NULL)
        printf("%03d: sym: '%s' found (%s)\n", idx, sym->name, name);
    else
        printf("error: %03d: sym: '%s' not found\n", idx, name);
}

int main() {

    init_memory();
    init_symbol_table();
    char* strs[] = {
        "asdf", "qwer", "werwe", "rewrew", "dfgddfg", "234908", "ewirou",
        "poipoi", "lkjlkj", "fkjgjh", "sdfkjhlj", "dsfkjl", "293847", "sdflkjsd",
        NULL
    };
    symbol_t* sym;

    for(int i = 0; strs[i] != NULL; i++) {
        sym = create_symbol(strs[i]);
        store_symbol(sym);
    }

    for(int i = 0; strs[i] != NULL; i++) {
        sym = find_symbol(strs[i]);
        print_sym(sym, i, strs[i]);
    }

    sym = find_symbol("mnbgdf");
    if(sym != NULL)
        printf("error: sym: '%s' was not stored, but it was found (%s)\n", "mnbgdf", sym->name);
    else
        printf("sym: '%s' was not stored, and was not found\n", "mnbgdf");

    sym = create_symbol("somethingelse");
    push_symbol_context(sym);
    sym = peek_symbol_context();
    printf("sym: '%s' peeked symbol\n", sym->name);

    for(int i = 5; i < 11; i++) {
        sym = create_symbol(strs[i]);
        store_symbol(sym);
    }

    for(int i = 4; i < 12; i++) {
        sym = find_symbol(strs[i]);
        print_sym(sym, i, strs[i]);
    }

    sym = pop_symbol_context();
    printf("sym: '%s' popped symbol\n", sym->name);

    for(int i = 4; i < 12; i++) {
        sym = find_symbol(strs[i]);
        print_sym(sym, i, strs[i]);
    }

    return 0;
}

