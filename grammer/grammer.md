```

program     → declaration* EOF ;

declaration → varDecl
              | statement ;

statement   → exprStmt
              | printStmt
              | block ;

block       → "{" declaration* "}" ;

varDecl     → "var" IDENTIFIER ( "=" expression )? ";" ;

exprStmt    → expression ";" ;
printStmt   → "print" expression ";" ;

expression  → assignment;
assignment  → IDENTIFIER "=" assignment
              | equality;
equality    → comparison ( ( "!=" | "==" ) comparison )* ;
comparison  → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
term        → factor ( ( "-" | "+" ) factor )* ;
factor      → unary ( ( "/" | "*" ) unary )* ;
unary       → ( "!" | "-" ) unary
              | primary ;
primary     → "true" | "false" | "nil"
              | NUMBER | STRING
              | "(" expression ")"
              | IDENTIFIER ;
```
