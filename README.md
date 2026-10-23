# Compiler---LMTE
**This is the code and the documentation/README associated with Assignment 2b: Custom Language Implementation**
Group members: Viet Hai Lam a2938816, Aden Ellery a1970601, Dragan Draganovic a3181710, Chenliang sun a1953023


## Features of the coding language 
## Basic Features:
### These are as per the basic functionality list in the rubric
**Variables:**
    -**Types:**
- Within out language we have 6 key variable types which each have their own declaration parameters. These variable types include:
            - Vboolean (Bool)
            - Vletter (Char)
            - Vwords  (String)
            - Vcharacter (one character either letter or number )
            - Vnum (any number)
            - VDigit (number 0-9)
        
        - These types go before the name of the variable and are declared in the manner:
            - Vnum age = 1
            - Vletter char = #q#
            - Vboolean true_condition = real

    - **Type Management:**
        - Type management is handled completely within out compiler. 
        - As we are using python, which doesnt have type declaration when declaring a variable. It doesnt matter if we dont include it when in python. 
        - Our code does checks on whether the variable has been declared if it doenst have a type at the start (and will fail if it isnt declared)
        - Whats entered in the variable will also be checked to ensure that the type aligns with whats entered in the variable. 

    -**Literals:**
        - Literals are handled very simplistically in the same way as any other variable.
        The declaration of these are the same way as above and are variables that remain unchanged. 
            eg: Vnum age = 18

**Arithemtic:**
    -

**Iteration:**
    -

**Functions**
    -

**Lists (Multi element array):**
**Comparison Expression**
**Conditional Statements:**
**How scope is implemented in our system:**
**Comments:**


## Advanced Features
### These are as per the advanced features implementation
#### Our key implementation was objects/classes which can be broken down further
    -
    -
    -
    -
#### Dynamic Memory allocation:



## Other Features
### These are our own extension to the project, though unsure what category these would fit in
**Print/Input**
**Size:**
**At:**