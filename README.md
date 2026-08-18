Scientific Calculator in C

A console-based scientific calculator developed in C as a Programming Fundamentals project. The calculator evaluates mathematical expressions using a custom stack-based approach.

Features:
Basic arithmetic operations: +, -, *, /
Modulus %
Power ^
Parentheses
Implicit multiplication, e.g. 2(3+4)
Square root sqrt
Cube root cbrt
Trigonometric functions: sin, cos, tan
Decimal numbers
Operator precedence
Division-by-zero checking
Dynamic memory allocation for stacks

Concepts Used:
Structures in C
Pointers
Dynamic memory allocation
Arrays
Stack data structure
Functions
Recursion
String processing
Mathematical functions
Expression evaluation

How It Works

The calculator uses two custom stacks:

Number Stack — stores numerical values.
Operator Stack — stores mathematical operators.

The expression is processed character by character while considering operator precedence and parentheses. Mathematical functions such as sqrt, cbrt, sin, cos, and tan are also supported.

Example:
Enter a string (supports +, -, *, /, %, ^, sqrt, cbrt, sin, cos, tan): 2(3+4)
Result: 14.000000


Technologies:
C
Standard C Library
Math Library

Academic Project:
This project was developed as part of a Programming Fundamentals course during my Software Engineering studies.
