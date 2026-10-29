
Readme · MD
# Compiler---LMTE
 
# RandomGibberish
 
**This is the code and the documentation/README associated with Assignment 2b: Custom Language Implementation**
 
Group members: Viet Hai Lam a2938816, Aden Ellery a1970601, Dragan Draganovic a3181710, Chenliang sun a1953023
 
## Features of the coding language
 
## Basic Features
 
### These are as per the basic functionality list in the rubric
 
**Variables:**
 
- **Types:**
  - Within out language we have 6 key variable types which each have their own declaration parameters. These variable types include:
    - Vboolean (Bool)
      - Boolean operations carry different names in this language (real = true and cake = false)
    - Vletter (Char)
    - Vwords  (String)
    - Vcharacter (one character either letter or number )
    - Vnum (any number)
    - VDigit (number 0-9)
  - These types go before the name of the variable and are declared in the manner:
```
    Vnum age = 1
    Vletter char = #q#
    Vboolean true_condition = real
```
 
- **Type Management:**
  - Type management is handled completely within out compiler.
  - As we are using python, which doesnt have type declaration when declaring a variable. It doesnt matter if we dont include it when in python.
  - Our code does checks on whether the variable has been declared if it doenst have a type at the start (and will fail if it isnt declared)
  - Whats entered in the variable will also be checked to ensure that the type aligns with whats entered in the variable.
- **Literals:**
  - Literals are handled very simplistically in the same way as any other variable. The declaration of these are the same way as above and are variables that remain unchanged.
  - Example:
```
    Vnum age = 18
```
 
**Arithemtic:**
 
- Arithmetic is implemented in a manner that uses 5 different functionalities:
  - add (Referencing addition/+)
  - sub (Referencing subtraction/-)
  - mul (Referencing multiplication/*)
  - div (Referencing division/'/')
  - mod (Referencing modulo/%)
- These are implemented in the parser in a manner that seperates addition based arithemtic (add/sub) and multiplication based arithemtic (mul/div/mod). This allows for the implementation and usage of bedmas and ensure correct mathematical notation.
- This implementation makes explicit use of three-address-code via the use of brackets around each part of the equation in a tree like manner.
- brackets have higher precedence than mul/div/mod. Those  have higher precedence than add/sub
An example of this implementation is that this code:
 
```
1 mul (2 add 3) div 10
```
 
Outputs:
 
```
((1 * (2 + 3)) / 10)
```
 
**How scope is implemented in our system:**
 
- Within out code, scope is implemented in a familiar yet different system from other languages.
- Other examples like c/c++/c# use a bracket based system containing the code in between.
- Our language does a similar system but the left bracket ("(") is replaced by a colon (:) and the right bracket (")") is replaced by a semi-colon (;).
- An example of this system in use is:
```
iguessif(x lessThan 5):
    forwhencake:
        handbrake
    ;
;
```
 
**Note: The functions seen above will be explained below**
 
**Iteration:**
 
- Our implementation of iteration relies on one true loop type. names 'forwhencake' which executes indefinately until the 'handbrake' keyword is used.
- This loop type is a whiletrue loop and handbrake serves the role of a brake.
- Handbrake can onlt be used within the scope of a forwhencake loop and terminates the current loop.
- This is implemented in the manner
```
forwhencake:
    handbrake
;
```
 
**Comparison Expression**
 
- RandomGibberishs' implementation of comparison expressions can be split into 2 key types. Boolean operators and comparison operators.
#### Boolean Operators
 
Includes 2 expressions, dna and ro
 
- dna is equivalent to and
- ro is equivalent to or
Examples include:
 
#### Comparison Operators
 
Includes 6 expressions:
 
- Equalto (Equivalent to ==)
- Lessthan (Equivalent to <)
- Morethan (Equivalent to >)
- Istotallydefinitelynotequalto (Equivalent to !=)
- Equallessthan (Equivalent to <=>)
- Equalmorethan (Equivalent to >=)
Example of both of these:
 
```
iguessif(x isTotallyDefinitelyNotEqualTo 5):
    iguessif(x equalLessThan 10 dna x equalMoreThan 5):
        x = 7
    ;
    guessnot:
        x = 1
    ;
;
```
 
**Conditional Statements:**
 
- Conditional statements can be seen in the above examples and operate depending on if conditions are/arent met.
RandomGibberish implements 3 different conditional tags:
 
- iguessif
  - In other languages this corresponds to an if statement and is implemented in the manner:
```
    iguessif(x lessThan 5):
        forwhencake:
            handbrake
        ;
    ;
```
 
- guessthis
  - Is used in the place of an else if statement and is implemented in the manner:
```
    iguessif(x lessThan 5):
        forwhencake:
            handbrake
        ;
    ;
    guessthis(x moreThan 5 ro x isTotallyDefinitelyNotEqualTo 10):
        forwhencake:
            handbrake
        ;
    ;
```
 
- guessnot
  - Finally, guessnot is used in the place of an else statement, meaning it executes if no other conditions are met. Implemented in the manner:
```
    iguessif(x equalLessThan 10 dna x equalMoreThan 5):
        x = 7
    ;
    guessnot:
        x = 1
    ;
```
 
**Functions**
 
- Functions are handled quite different from other lanaguages.
- They start with the initial "FUNCTION" keyword, so the lexer/parser knows it a function. Its then followed up by the function return type, which are the same types as whats listed above.
- Like lists, each of the required paramaters are split via a "|"
- This line is the ended by a ":", this defines the start of the function scope, as mentioned above, its then ended by a ";"
The explaination above can be summaried by:
 
```
FUNCTION <Type> <FunctionName> <parameters (split by |)>:
    <Insert code within fucntion>
    leave <>
;
```
 
Examples:
 
```
<!-- Function declaration -->
FUNCTION Vboolean NewFunction2 Vnum num1 | Vnum num2:
    Vnum result = num1 add num2
    leave cake
;
 
<!-- Function call -->
Vnum new6 = f NewFunction2 3 | 3
```
 
**Lists (Multi element array):**
 
- RandomGibberish doesnt have statically allocated arrays and used dynamically allocated lists.
- These operate under a specific syntax and can be of any length (due to the list being dynamically allocated)
- The syntax for lists is as follows:
  - A list MUST start and end with a `"`
  - the first item in the list is always the name
  - Each item in the list (including after the name) is seperated by a `|`
Example:
 
```
"randoms | 1 | 2 add 3 | 10"
```
 
**Comments:**
 
- Comments are implemented in a similar way to as html.
- These use a structure beginning with `<!--` and ending with a `-->`
## Advanced Features
 
### These are as per the advanced features implementation
 
#### Our key implementation was objects/classes which can be broken down further
 
- In the Randomgibberish class syntax, there are two main components. First is the constructors, which itself is a normal function. The constructor is always public and has the name "SELF". Every time an object is declared, this function runs to help create the object and assign attributes. The next part of a class declaration are methods.  All methods are declared within a public/private/protected, which indicates their access modifiers. There can be multiple access modifier scopes of the same type. In each of these scope, functions are declared similar to how they would in global scope.
- You can have multtiple public, protected and private parameters, making class declaration and function organization more flexible
- INHERITENCE HERE
- PASTE AN EXAMPLE OF DECLARATION AND INHERITENCE
#### Dynamic Memory allocation
 
- RandomGibberish support dynamic memory allocation in the form of lists. Lists are a dynamically allocated data structure rather then the statically allocated arrays.
- This allows for the usage of dynamically allocated data strcutures and the unreliance on a finite list.
- We havent implemented memory management.
## Other Features
 
### These are our own extension to the project, though unsure what category these would fit in
 
#### Print/Input
 
- Print and input are implemented using new syntax in the RandomGibberish Language 
- It uses the new keyworks say (print) and ask (input) in the syntax below:
  - `say <output>`
- ask implementation allows for the user to input their own value under either variable types: Vwords, Vnum, VDigit and Vletter, in the format below:
  - `<Type> <VarName> = ask`
Examples:
```
say $Hello world$
say 5 add 3

Vwords nam2e = ask
Vnum ag2e = ask
```
  
#### Size:
 - Size is an extention on the list system
- It returns the size of the lists
- Implemented in the manner:
  - `<ListName>.size()`
#### At:
 
- At is also an extension on the list extension
- It returns the value at the index provided from a list
- Implemented in the manner:
  - `<ListName>.at[Index]`
## Code Structure/Layout
 
The codebase is split into many files that serve their own purpose. This explaination will go through what files are prsenet, the connection between the files and fucntions within.
 
### Connections between files/strcuture
 
Code written in RandomGibberish syntax is written into test.txt, this is then opened by the main.cpp file. The main.cpp file then starts the lexer (Lexer.h and Lexer.cpp) and tokenizes the code in text.txt, it then calls parserc (Parser.h and Parser.cpp) and, using the tokens generated by lexer and the expression types in Expr.h/.cpp, generates trees for each line of the test.txt file. Using these trees, the main file opens the output file (python.py) where it then generates the code for each tree and outputs it within the python file. It then closes teh file and outputs a confirmation message in the terminal.
 
### Files present
 
#### CodeGen.h
 
#### CodeGen.cpp
 
#### Expr.h
 
#### Expr.cpp
 
#### Lexer.h
 
#### Lexer.cpp
 
#### main.cpp
 
main.cpp is the entry point into the program. It loads the test.txt file as the path and reads each line of the file, holding it as source.
 
#### Parser.h
 
#### Parser.cpp
 
#### python.py
 
Python.py is the output file. During the code generation process, the codeGen.cpp functions write into this file.
This file is runnable using a python interpreter.
 
#### SymbolEntry.h


#### Optimiser.h


#### Optimiser.cpp
 
#### README.md
 
This file. It contains the analysis of RandomGibberish features, analysis of the code strcuture and the way the different files work together/what is present in each file. Along with the 2-3 test programs. The test programs are more extensive then test cases and analyse different fucntionality at once and the way they work together in scope, compilation and variable management.
 
#### test.txt
 
Test.txt is where the RandomGibberish code is written. It is a hardcoded path within main.cpp and is the file main.cpp uses for the lexer (and therefore the rest of the compilation process).
Includes a range of diffferent test cases which test different functionality of the program
 
#### Token.h
 
Token.h is the base struct for tokens (which are generated by the lexer). It takes in a token type and the value associated with the token and creates a token using the constructor (`Token(TokenType type, std::string value)`)


## Advanced Compilation Processes
### Type Checking
- Despite the lack of type initialising in python, our language requires a type specifier on declaration (AS SEEN ABOVE)
- It has checks for function returns, attributes and variables (as well as other specifiers, besides classes) and ensures they're of the same type.
- Throws an error if these aren't found to be correct. 

### Avoiding mathematical Errors
- The arithmetic functionality was implemented using extensive use of three address code.
- This was implemented in a manner that has extensive use of brackets depending on the calculation type (mulOP or addOP), ensuring the correctness of calculations. 

### Dead Code optimisation
- Dead code removal has been implemented for functions, classes and methods.
- It ensures that if either of these have not been used after declaration, they will not be compiled and will not show up within the python.py file. 
