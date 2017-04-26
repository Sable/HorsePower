# Coding styles

We introduce the conventions for writing C code for the HorsePower project.  These conventions are summarized based on our experience working on both C and Java programming.  We hope they could help improve the readability and quality of our code.

## Content

There are several parts we consider as follows.

**General rules**

- No class (i.e. no object, but structs are allowed)
- Macro based
- Indentation with 4 spaces
- Unix-like file format
- Configuration with Makefile
- [Test driven development](https://en.wikipedia.org/wiki/Test-driven_development)

**Styles of names**

- Single word
  + (case 1) All lowercase letters
  + (case 2) All uppercase letters
- Compound words
  + (case 3) Camel cases
  + (case 4) Camel cases but with capitalized first letter
  + (case 5) Underscore cases

**Function**

- The first letter of a function name should be capitalized, e.g. *FooName*
- A function body should not longer than one screen size (divided into sub-methods)

**Variable**

- A variable with multiple names should start with a lowercase letter, e.g. *getName*
- A variable with a single name consists of all lowercase letters, e.g. *pointer*

**Macros**

- If a single word, all uppercase letters, e.g. *P* and *MAX*
- Otherwise, all lowercase letters, e.g. maxsize

**Constant**

- All uppercase letters with underline, e.g. *STRING_SIZE*

**Struct**

- A struct name has two cases:
  1. follows function names if it is a struct name
  2. follows function names with a tailing number '0' if it is a struct pointer
- The names of elements follow variable names

**An example**

- See [demo.c](demo.c)

**Abbreviation**

| Short   | Original       |
| :------ | :------------- |
| ptr     | pointer        |
| val     | value          |
