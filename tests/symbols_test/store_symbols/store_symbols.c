/**
 * @file store_symbols.c
 * @brief Test the symbol table functionality.
 *
 * Functions to test:
 * void init_symbol_table();
 * symbol_t* create_symbol(const char*);
 * symbol_error_t store_symbol(symbol_t*);
 * symbol_t* find_symbol(const char*);
 * symbol_error_t open_symbol_context(const char* name);
 * symbol_error_t close_symbol_context();
 * symbol_t* get_symbol_context();
 * 
 * Use a debugger to really verify this. Otherwise, it's a sanity check.
 */
#include <stdarg.h>

#include "common.h"
#include "symbols.h"

// TODO: fill in the rest of the symbol context.
// TODO: convert this to formal unit tests.

/*
    if the flag is true, then the test passes if the symbol is not NULL. If the
    flag is false, then the test passes if the symbol is NULL.
 */
static void print_sym(char flag, symbol_t* sym, int idx, const char* name, const char* msg, ...) {


    printf("%s ", ((flag && sym != NULL) || (!flag && sym == NULL))? "PASS": "FAIL");
    printf("sym%03d: ", (idx <= 0)? 0: idx);
    printf("%-10s ", (name != NULL)? name: "NONE");
    printf("%s ", (sym != NULL)? "FOUND": "NOT FOUND");

    if(msg != NULL) {
        va_list args;
        va_start(args, msg);
        vprintf(msg, args);
        va_end(args);
    }
    printf("\n");
    
}

int main() {

    init_memory();
    init_symbol_table();
    char* strs1[] = {
        "asdf", "qwer", "werwe", "rewrew", "dfgddfg", "234908", "ewirou",
        "poipoi", "lkjlkj", "fkjgjh", "sdfkjhlj", "dsfkjl", "293847", "sdflkjsd",
        NULL
    };
    char* strs[] = {
        "symbol01", "symbol02", "symbol03", "symbol04", "symbol05", "symbol06", "symbol07", "symbol08", "symbol09", "symbol10",
        "symbol11", "symbol12", "symbol13", "symbol14", "symbol15", "symbol16", "symbol17", "symbol18", "symbol19", "symbol20",
        NULL
    };
    symbol_t* sym;

    for(int i = 0; strs[i] != NULL; i++) {
        sym = create_symbol(strs[i]);
        store_symbol(sym);
    }

    for(int i = 0; strs[i] != NULL; i++) {
        sym = find_symbol(strs[i]);
        print_sym(1, sym, i+1, strs[i], "(stored %s)", strs[i]);
    }

    char* tmps = "mnbgdf";
    sym = find_symbol(tmps);
    print_sym(0, sym, -1, tmps, "(was not stored)");

    sym = create_symbol(tmps);  // created at level 1
    store_symbol(sym);
    open_symbol_context(tmps);  // open level 2

    for(int i = 5; i < 11; i++) {
        sym = create_symbol(strs1[i]);
        store_symbol(sym);
    }

    open_symbol_context(NULL);  // level 3
    for(int i = 0; i < 7; i++) {
        sym = create_symbol(strs[i]);
        store_symbol(sym);
    }

    open_symbol_context(NULL);  // level 4
    for(int i = 12; i < 20; i++) {
        sym = create_symbol(strs[i]);
        store_symbol(sym);
    }

    close_symbol_context();     // level 3
    close_symbol_context();     // level 2

    open_symbol_context("fkjgjh");  // level 3
    for(int i = 8; i < 16; i++) {
        sym = create_symbol(strs[i]);
        store_symbol(sym);
    }

    open_symbol_context(NULL);  // level 4
    for(int i = 0; strs1[i] != NULL; i++) {
        sym = create_symbol(strs1[i]);
        store_symbol(sym);
    }


    printf("\n--- dump the table ---\n");
    dump_symbol_table();

    return 0;
}

