# A Lambda Calculus Interpreter

Takes lambda expressions line by line and executes each of them step by step.

## Example
input:

	((\x.\y.(y x) i) \t.(h t))

output:

	((\x.\y.(y x) i) \t.(h t))
	(\y.(y i) \t.(h t))
	(\t.(h t) i)
	(h i)

## Lambda Expressions
A valid lambda expression has the form noted below:

	Term ::= Variable | (Term Term) | \Variable.Term

Where a variable is a lower-case alphabetic character.
