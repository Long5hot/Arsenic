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

Usage
---

Below program prints Fibonacci sequence till 20th number.

```cpp

fun fib(n) {
  if (n <= 1)
    return n;
  return fib(n - 2) + fib(n - 1);
}

for (var i = 0; i < 20; i = i + 1) {
  print fib(i);
}
```

To run above code, do:
```sh
arsenic test.nic
```
will produce below result
```
0.000000
1.000000
1.000000
2.000000
3.000000
5.000000
8.000000
13.000000
21.000000
34.000000
55.000000
89.000000
144.000000
233.000000
377.000000
610.000000
987.000000
1597.000000
2584.000000
4181.000000
```


