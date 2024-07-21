# Arsenic : C++ implementation of the Lox interpreter
A Tree-walk Interpreter implementation as shown in [*Crafting Interpreters*](http://craftinginterpreters.com).

Chapters complete
---
1. Intro
2. A Map of thr Territory
3. The Arsenic(Lox) language
4. Scanning
5. Represening code
6. Parsing Expressions
7. Evaluating Expressions
8. Statements and State
9. Control flow
10. Functions
11. Resolving and Binding (**in progress**)

Building
---

For normal build run:

```sh
# in build dir
cmake <src-dir> 
```
For debug build run:

```sh
# in build dir
cmake -DCMAKE_BUILD_TYPE=Debug <src-dir> 
```

and then

```sh
cmake --build .
```
