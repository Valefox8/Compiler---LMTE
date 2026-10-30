
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
 #### Classes
- Classes declarations are defined by a string starting and ending in "C", followed by a colon (eg. CMuleC:)
- In the Randomgibberish class syntax, there are two main components: 

- First is the constructors, which itself is a normal function. The constructor is always public and has the name "SELF". Every time an object is declared, this function runs to help create the object and assign attributes. 

- The next part of a class declaration are methods.  All methods are declared within a public/private/protected, which indicates their access modifiers. There can be multiple access modifier scopes of the same type. In each of these scope, functions are declared similar to how they would in global scope.
- You can have multtiple public, protected and private parameters, making class declaration and function organization more flexible. The public access method will allow outside scope to access any of the class member, protected resticts this to child scope and private will not allow any outside scope from accessing the class attributes and methods.
- All classes must have a constructor. If a class lack a constructor declaration, the program automatically create an empty constructor. This allows access to object declaration or inheritance.
- Below is an example of a class declaration
```
CMyVehicleC:
    FUNCTION SELF Vnum speed | Vnum value:
        public Vnum selfish -> speed = speed
        public Vnum selfish -> value = value
    ;

    public:
        FUNCTION Vnum DECELERATE Vnum hunger:
            Vnum time = speed add value - hunger
            leave time
        ;
    ;
;
```


#### Inheritance
- The language also support inheritance from only one parent class. There are a few main points to watch out for this feature.
- Firstly, children classes have access to parent public or protected attributes and method. They can read, change or use any functions and attributes of their parent class.
```
CanimalC:
    FUNCTION SELF Vnum speed | Vnum value:
        public Vnum selfish -> speed = speed
        public Vnum selfish -> age = speed
    ;
    public:
        FUNCTION Vnum EIGHT:
            say 8
            leave 8
        ;
    ;
;

CMyVehicleC: WithTheClassic CanimalC:
    FUNCTION SELF: TAKEOVER CanimalC -> f SELF 3 | 3:

        <!-- Using parent already declared methods and attributes -->
        public Vnum selfish -> speed = selfish -> f EIGHT
        selfish -> speed = selfish -> age
    ;
;
```

- Secondly, Randomgibberish allows overriding methods by using the TAKEOVER keyword. The user can either choose to completely overwrite a method or have the code from the parent class runs before the children class method. 

- Although children classes can directly override parent method directly without using the TAKEOVER keyword, using it offers some benefit. Child class method must have a specific implementation similar to parent method. In other words, the child method must have the same return type, access (public/protected) and parameters as the parent method.   
```
CMyVehicleC:
    public:

        <!-- Have the parent method run first -->
        FUNCTION Vnum DECELERATE Vnum speed3: TAKEOVER CanimalC -> f DECELERATE 3:
            leave speed1
        ;

        <!-- Dont let the parent method run -->
        FUNCTION Vnum ACCELERATE Vnum speed4: TAKEOVER:
            leave speed1
        ;

    ;
;

```
- Inheritance also forces the child class to have its own consturctor. This is to ensure the parent constructor always run before the the children constructor. Constructors are the only exception of overriding where the parameter list can change. Below is an example of inheritance where the parent class lack a constructor
```
CanimalC:
;

CMyVehicleC: WithTheClassic CanimalC:

    <!-- Must run an empty constructor -->
    FUNCTION SELF: TAKEOVER CanimalC -> f SELF:
        public selfish -> speed = 1
    ;
;
```

#### Object call
- Users can declare an object using the syntax, and use its attributes or method by:
```
CMyVehicleC:
    FUNCTION SELF:
        public Vnum selfish -> speed = 3
    ;
    public:
        FUNCTION Vnum EIGHT Vnum num:
            say 8
            leave 8
        ;
    ;
;

CMyVehicleC NewVehicla = CMyVehicleC        <!-- Object declaration -->
Vnum a = 10 add NewVehicla -> speed        <!-- using object attribute -->

<!-- using object method -->
NewVehicla -> f EIGHT 8
a = a add NewVehicla -> f EIGHT 8
```

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

The header file for the code generation component. It declares the CodeGen class, including its method GenCode used to generate Python code from the vector of parsed RandomGibberish expressions.

#### CodeGen.cpp

Contains the implementation of the CodeGen class. Its functions traverse the Abstract Syntax Tree (AST) (in this case is a vector of parsed expression) and generate the equivalent Python code in the form of std::string.

#### Expr.h

Defines the different expression structures used to represent RandomGibberish code after parsing. These structures form the Abstract Syntax Tree (AST), representing elements such as variable declarations, function declarations, classes, conditional statements, loops and expressions.

#### Expr.cpp

Contains helper methods specific for each each type of expressions. An example is Scope Expr, that stores the symbol table of the current scope. This feature will be further explained later. 

#### Lexer.h

The header file for the lexer. It declares the Lexer class and its associated functions and attributes, which are responsible for converting the source code into tokens.

#### Lexer.cpp

Contains the implementation of the lexical analyser. It reads the source code character by character, identifies keywords, identifiers, operators, literals and other language components, and converts them into tokens for the parser. This returns a vector of Token, which will then get analysed by Parser.cpp

#### main.cpp

main.cpp is the entry point into the program. It loads the test.txt file as the path and reads each line of the file, holding it as source. 
 
#### Parser.h

The header file for the parser. It declares the Parser class, including functions to parse each type of expression defined in Expr.h. 

#### Parser.cpp

The parser iterates through a vector of Tokens, and determines wach parsing function to use to return a specific type Expression. This expression is then stored in a vector, which Optimiser.cpp will then determine what expression to keep or remove. The parser also checks whether the code follow the grammar rules, and it is especially sensitive to undeclared variables. 

#### python.py
 
Python.py is the output file. During the code generation process, the codeGen.cpp functions write into this file.
This file is runnable using a python interpreter.
 
#### SymbolEntry.h

Defines the SymbolEntry structure used to store information about symbols declared within a scope. Each entry contains information namely the symbol name, type (Vnum, Vletters, and so on), kind (Class, Attribute, Normal variables, Objects and so on) and pointer to associated expression in the Parser's vector. These entries are used by the parser and symbol tables to track declarations and manage variable and function references.

#### Optimiser.h
#### Note: There maybe some edge cases where Optimiser removes wrong Expr. To see all the parsed code, remove the codes that runs Optimisers and its methods. 

The header file for the optimiser. It declares the Optimiser class and its associated functions and attributes used to identify and remove unreachable or unused code from the AST.

#### Optimiser.cpp

Contains the implementation of the optimiser. It analyses the Parsers vector of expressions to identify reachable scopes and used declarations, determining which parts of the program are actually required. Unreachable scopes or unused declarations are then removed before code generation, reducing unnecessary generated Python code.
 
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
- Each scope (funciton, class, conditional, loop) stores its own symbol table, and a pointer to its parent scope. If for example a function is called, the current scope perfrom a look up to find the declaration for that function, starting from current scope upt to the global scope. If it was unable to find anything, the Parser will throw an error, and stop the program from running. If there is some type mismatch (eg. Vnum a = f FUNCTION_Name but FUNCTION_Name returns a boolean type), it will also throw an error.

### Avoiding mathematical Errors
- The arithmetic functionality was implemented using extensive use of three address code.
- This was implemented in a manner that has extensive use of brackets depending on the calculation type (mulOP or addOP), ensuring the correctness of calculations. 

### Dead Code optimisation
- Dead code removal has been implemented for functions, classes and methods. RandomGibberish will automatically remove any functions or classes that are not reachable from the global scope. 
- For example, function B declared in a another function A in global scope. If function A is called, then the declaration for A is marked as 'reachable', and it will generate python code. However, inside the scope of function A, if function B is never called, the Optimiser will remove it from being parsed into python code.
- It ensures that if either of these have not been used after declaration, they will not be compiled and will not show up within the python.py file.
- This also works for variable declaration. If a variable is never used (not printed, not used in a function/class argument, the variable doesnt call a function/class declaration), Optimiser will remove it from being parsed.


## RandomGibberish work flow:
### Lexer:
From main.cpp, open and read from a .txt file (hard coded to be test.txt)

From main.cpp, call Lexer (Lexer.h, Lexer.cpp) to read through each char in the .txt file 

Lexer receives the content of the text file. Specific char or string will returns specific Token (Token.h) with a value (std::string) and a type (defined in TokenType.h)

Lexer reaches the end of the .txt file and returns a vector of Token objects (vector<Token>).

### Parser:
main.cpp calls the Parser (Parser.h, Parser.cpp) to examine vector<Token> returned by the Lexer. 

Parser sets the currentScope to GlobalScope. GlobalScope is also an expression, ScopeExpr (Expr.h). This means that it has a body which is a vector of Expr. The Parser starts parsing vector<Token> and add it to the body of GlobalScope. 

When the Parser see a specific combination of Token and calls a parse helper function

#### Normal Declaration: 
- Parser calls a PARSE_DECLARATION function (eg. ParseVarDec, ParseList) when seeing a declaration. The PARSE_DECLARATION function returns a specific Expr (Expr.h). 
- For example, with variable declaration "Vnum x = 10", the parser calls ParseVarDec() and returns VarDecExpr. 
- It then creates a new SymbolEntry (SymbolEntry.h) that specify: Name = x, Type = Vnum, Symbol Kind (SymK) = variable, Expression = VarDecExpr (A pointer points to the just created expression), Access = Public. 
- This new SymbolEntry is stored in the currentScope symbol table. The newly created Expr VarDecExpr is added to the Parser's vector of Expr

#### New Scope Declaration (eg. functions, classes, if, while):
- Note: ScopeExpr stores a vector of expression (similar to )
- Follows the same concept as above but returns ScopeExpr (eg. FunctionDecExpr (Expr.h)). For example, parsing the following function in the globalScope:
```
FUNCTION Vnum DECELERATE Vnum speed1 | Vnum x:
    speed1 = speed1
    leave 8
;

```
- Parser calls ParseFunction, which return FunctionDecExpr, which is a child of ScopeExpr.
- After the new Expr is created, the Parser has to parse the scope body of code and stores it in the newly created body of FunctionDecExpr.
- This is achived by setting currentScope to the newly created FunctionDecExpr, and set the ParentScope of FunctionDecExpr to Global Scope
- All the statements/Expression in the function body (like this one or the one in the section Normal Declaration) is then parsed into Expr and store in FunctionDecExpr Body. This means that you can declare another scope inside the scope of FunctionDecExpr, and currentScope will change accordingly to parse the new scope body.
- Since FunctionDecExpr is a ScopeExpr, it also contains a symbol table to record declared variables.
- After reaching the end of FunctionDecExpr, the Parser sets currentScope to globalScope again.
- The function Expr and SymbolEntry is then added to globalScope.

#### Symbol Table lookup (ScopeExpr::LookUP function in Expr.cpp):
- Before making a declaration (like the two example above) or calling something (eg. variables, functions, classes, objects), the Parser searches up the name. 
- It first checks  the currentScope symbol table. If it does not find anything, it then searches the ParentScope of currentScope, until it has finished searching globalScope. 
- If the Parser still fails to find a SymbolEntry, it throws an exeption, stoping the program from running.

### Inheritance/Object Symbol Table lookup (ClassDecExpr::LookUpLocal in Expr.cpp):
- When calling an object attribute (eg. obj->Name), the Parser first finds the class declaration of that object. Every class declaration has a pointer to its parent that it inherits attributes and method from. The parser then use search the current class, before moving onto its parent and so on. 
- If the Parser still fails to find a SymbolEntry, it throws an exeption, stoping the program from running.

## Optimiser: 
Optimisation features actually started in Parser.cpp with CheckUsage (Expr.h)

When the Parser identifies a loop or an 'if else' statement, both of which are ScopeExpr, it have currentScope record those expressions as 'used'. In other words, the parser tells currentScope to remember that those two expressions are marked as 'used' in currentScope scope. 

Similar process happens when the Parser identifies a function or class declaration. However, these two Expr are only recorded if they are called. 

This process also occurs when the Parser identifies VarDecExpr.
- If a variable is used in a print() or an function call argument 
- If the variable themself calls a function/method
- If the variable calls a object declaration, 

&rarr; The Parser tells the currentScope to mark that VarDecExpr as "used". 

In addition Assignments below also function similarily, but instead of the Parser telling the currentScope, it tells the VarDecExpr to mark another VarDecExpr
```
Vnum a = 10
Vnum b = a
```
In the example above, VarDecExpr for b marks the VarDecExpr of a.

main.cpp calls Optimiser(Optimiser.h, Optimiser.cpp) to mark reachable scopes (MarkReachableScope(globalScope.get()))
Starting from globalScope, Optimiser starts iterating through its body, which is a vector of Expr.

 When it sees an inner ScopeExpr, it checks with globalScope which is a ScopeExpr, to see if that ScopeExpr is marked as 'used' or not. 
- If that inner scope is not marked, the Optimiser do nothing.  
- If that inner scope is marked, the Optimiser calls MarkReachableScope(), but now with the inner scope instead of globalScope. It then mark that inner scope as 'reachable'. 

When any scope sees a VarDecExpr, it check within itself to see if that VarDecExpr is marked as 'used' or not. If that scope is 'reachable' then that VarDecExpr is also marked as 'reachable'.

Remember that VarDecExpr can mark other VarDecExpr as 'used'. Therefore if a VarDecExpr is marked as 'reachable', Optimiser also marks all its 'used' VarDecExpr as reachable.

After this process is completed void Sweep(globalScope) is performed to removed all the unreachable variables and functions.

## CodeGen
main.cpp receives the optimised globalScope and call CodeGen (CodeGen.h, CodeGen.cpp) to run its method GenCode by iterating through all Expr in the body of globalScope.

Each Expr triggers a differnt functionality of GenCode, and returns a string which is a line, or block of code in Python.

main.cpp then writes the resulting string to the python file, then save it.


