# Lambda Calculus Interpreter

Reference:
- https://en.wikipedia.org/wiki/Lambda_calculus_definition#Syntax_definition_in_BNF
- https://ruslanspivak.com/lsbasi-part1/
- https://tromp.github.io/cl/Binary_lambda_calculus.html
- https://cs.stackexchange.com/questions/109954/writing-a-grammar-for-lambda-calculus
- https://stackoverflow.com/questions/22390641/read-file-by-bits-c

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
l lambda calculus
i De Bruijn indexed lambda calculus
bt Binary Lambda Calculus in text format
b Binary Lambda Calculus in binary format
```

From （l, i, bt, b） convert to （l, i, bt, b）
```
bin/lc [--i=type] [input file [...]] [--o=type] [output file [...]]
```

with std input
```
bin/lc [--i=x] [input file [...]] [--o=y] [output file [...]] <<EOF
> type
> whatever you want
> to type
> EOF
```
