#include "common.h"

#include "scanner.h"
#include "parser.h"
#include "symbols.h"


/**
 * Top level interface. When this is entered, the 'class' keyword has been read
 * and the class name, followed by the class definition, is expected.
 *
 * This accepts a string like:
 *  name(name) { // $name$name
 *  name() {    // $name
 *  name {      // $name
 *
 */
parse_state_t class_definition() {

}
