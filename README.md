# Lambda Calculus Interpreter

Reference:
- https://en.wikipedia.org/wiki/Lambda_calculus_definition#Syntax_definition_in_BNF
- https://ruslanspivak.com/lsbasi-part1/
- https://tromp.github.io/cl/Binary_lambda_calculus.html

## BNF used for Normal Lambda Calculus in this project
```
<expr> ::= ^ <var> . <expr>
         | <app>
<app> ::= <app> <item>
        | <item>
<item> ::= ( <expr> )
         | <var>
<var> ::= (^[\^\.\(\)\s])+
```

## BNF used for De Bruijn index Lambda Calculus in this project
```
<expr> ::= ^ <expr>
         | <app>
<app> ::= <app> <item>
        | <item>
<item> ::= ( <expr> )
         | <var>
<var> ::= [0-9]+
```

## BNF used for Binary Lambda Calculus in this project
```
<expr> ::= 00 <expr>
         | 01 <expr> <expr>
         | 1+0
```

## Ussage

From lambda calculus convert to index lambda calculus
```
./lc -i <<EOF
> type
> whatever you want
> to type
> EOF
```

From lambda calculus convert to binrary lambda calculus
```
./lc -b <<EOF
> type
> whatever you want
> to type
> EOF
```

From index lambda calculus
```
./ilc <<EOF
> type
> whatever you want
> to type
> EOF
```

Execute binary lambda calculus
```
./blc <<EOF
> type
> whatever you want
> to type
> EOF
```

Piplining lambda calculus to binary lambda calculus
```
./lc -b | ./blc <<EOF
> type
> whatever you want
> to type
> EOF
```