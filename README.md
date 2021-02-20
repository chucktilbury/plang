# Plang
This is the repository for the **Plang** language. The **Plang** language is a mixture of several languages, including C and Python. It is a simple translator that accepts **Plang** and translates it to C. Some other iteration of the language may use LLVM to generate code. This will never be an interpreted language system. 

## Why?
The reasons for implementing **Plang** are very simple. I love programming languages and I love writing code. I have always wanted to create a language from scratch and then write a bunch of code in it. I have made numerous attempts, and as of February 2021, this is the latest one. 

## Goals
* **Plang** is object oriented. Only classes are used to implement functionality.
* **Plang** is strongly typed but also allows robust casting.
* **Plang** has garbage collection.
* **Plang** does not have pointers.
* **Plang** supports maps, dicts, and lists natively.
* **Plang** has a simplified native type system that supports ints, uints, floats, bools, and strings natively. 
* **Plang** follows the notion of having as few keywords and operators as possible, while still being able to implement complex programs. It is intended to be a application language, rather than a system language that has things like bitwise operations.
* **Plang** follows the notion of having just one way to do something, rather than numerous semantic paths. It's intended to be as simple as possible, but no simpler.

## Syntax

The following is a list of diagrams that show the whole syntax of **Plang**. The only thing that is not shown in the diagrams is the contents of terminal objects such as comments, symbols, strings, and numbers. The intention is to demonstrate the workings of the parser, not the scanner. 

![term-nterm](docs/pics/term-nterm.jpg)

Top level non-terminal is a Module.

![module](docs/pics/module.jpg)

An import statement is used to bring in symbols from another module. It does not bring in that module's implementation. 

![import-statement](docs/pics/import-statement.jpg)

The class declaration is a simple declaration that does not include any initialization. The reason for that is that when the source code is translated into C, the classes become ```typedef struct```s.

![class-decl](docs/pics/class-decl.jpg)

The method and data declarations only contain the scope, the assignment type, and the types of the parameters for method declarations. 

![method-decl](docs/pics/method-decl.jpg)

Class data declaration does not allow assignment. 

![class-data-decl](docs/pics/class-data-decl.jpg)

The attribute scope non-terminal is only used inside a class. The type attribute gives the assignment type. These keywords have the meaning one might expect.

![scope-type](docs/pics/scope-type.jpg)

The complex symbol non-terminal is used everywhere a symbol reference is applicable. The complex symbol name refers more to the implementation of the resolver than the actual usage because a simple name is also acceptable there.

![complex_name](docs/pics/complex_name.jpg)

A formatted string is used everywhere that a quoted string is acceptable, included as an argument to a formatted string. Note that only double-quoted string can be formatted. Single quoted string are taken to be absolute literals.

![formatted-string](docs/pics/formatted-string.jpg)

A method definition is exactly that. These diagrams do not cover the rules for defining and implementing constructors and destructors. Note that only the type and scope are not given in the method definition because they have already been specified in the declaration. All of the following diagrams support method definitions.

![method-name-definition](docs/pics/method-name-definition.jpg)

![method-name-definition](docs/pics/method-body.jpg)

![method-body-statement](docs/pics/method-body-statement.jpg)

 ![assignment-statement](docs/pics/assignment-statement.jpg)

![func-call-statement](docs/pics/func-call-statement.jpg)

![data-decl](docs/pics/data-decl.jpg)

![data-definition](docs/pics/data-definition.jpg)

![if-else-clause](docs/pics/if-else-clause.jpg)

![while-clause](docs/pics/while-clause.jpg)

![for-clause](docs/pics/for-clause.jpg)

![inline-clause](docs/pics/inline-clause.jpg)

![try-except-clause](docs/pics/try-except-clause.jpg)

![switch-case-clause](docs/pics/switch-case-clause.jpg)

![expressions](docs/pics/expressions.jpg)



