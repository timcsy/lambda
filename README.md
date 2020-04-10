# Lambda Calculus Interpreter

Reference:
- https://en.wikipedia.org/wiki/Lambda_calculus_definition#Syntax_definition_in_BNF
- https://ruslanspivak.com/lsbasi-part1/
- https://tromp.github.io/cl/Binary_lambda_calculus.html

## BNF used in this project
```
<expr> ::= ^ <var> . <expr>
				 | <app>
<app> ::= <app> <item>
				| <item>
<item> ::= ( <expr> )
				 | <var>
<var> ::= (^[\^\.\(\)\s])*
```