# Lambda Calculus Interpreter

Reference:
- https://en.wikipedia.org/wiki/Lambda_calculus_definition#Syntax_definition_in_BNF
- https://ruslanspivak.com/lsbasi-part1/
- https://tromp.github.io/cl/Binary_lambda_calculus.html
- https://cs.stackexchange.com/questions/109954/writing-a-grammar-for-lambda-calculus

## BNF used for Normal Lambda Calculus in this project
```
<expr> ::= <abs>
         | <app>
<item> ::= ( <expr> )
         | <var>
<var> ::= (^[\^\.\(\)\s])+
<abs> ::= ^ <var> . <expr>
<app> ::= <item>
        | <app> (<abs> | <item>)
```

## BNF used for De Bruijn indexed Lambda Calculus in this project
```
<expr> ::= <abs>
         | <app>
<item> ::= ( <expr> )
         | <var>
<var> ::= [0-9]+
<abs> ::= ^ <expr>
<app> ::= <item>
        | <app> (<abs> | <item>)
```

## BNF used for Binary Lambda Calculus in this project
```
<expr> ::= 00 <expr>
         | 01 <expr> <expr>
         | 1+0
```

## Ussage
Note
```
-l lambda calculus
-i De Bruijn indexed lambda calculus
-b Binary Lambda Calculus
```

From -x（-l, -i, -b） convert to -y（-l, -i, -b）
```
bin/lc -x -y <<EOF
> type
> whatever you want
> to type
> EOF
```
