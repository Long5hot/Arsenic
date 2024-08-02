```

program     → declaration *EOF;

declaration → classDecl | funDecl | varDecl | statement;

statement   → exprStmt | forStmt | ifStmt | printStmt | returnStmt | whileStmt |
              block;

classDecl   → "class" IDENTIFIER "{" function* "}" ;

funDecl     → "fun" function;

varDecl     → "var" IDENTIFIER("=" expression) ? ";";

function    → IDENTIFIER "(" parameters ? ")" block;

parameters  → IDENTIFIER("," IDENTIFIER) *;

returnStmt  → "return" expression ? ";";

block       → "{" declaration * "}";

exprStmt    → expression ";";

printStmt   → "print" expression ";";

whileStmt   → "while" "(" expression ")" statement;

forStmt     → "for" "("(varDecl | exprStmt | ";") expression ? ";" expression ? ")" statement;

ifStmt      → "if" "(" expression ")" statement("else" statement)? ;

expression  → assignment;

assignment  → IDENTIFIER "=" assignment | logic_or;

logic_or    → logic_and("or" logic_and) *;

logic_and   → equality("and" equality) *;

equality    → comparison(("!=" | "==") comparison) *;

comparison  → term((">" | ">=" | "<" | "<=") term) *;

term        → factor(("-" | "+") factor) *;

factor      → unary(("/" | "*") unary) *;

unary       → ("!" | "-") unary | call;

call        → primary ( "(" arguments? ")" )* ;

arguments   → expression("," expression) *;

primary     → "true" | "false" | "nil" | NUMBER | STRING | "(" expression ")" |
              IDENTIFIER;

```
