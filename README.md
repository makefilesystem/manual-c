Manual-C Language
Manual-C is a simple, user-friendly programming language designed for quick scripting and prototyping. This README provides an overview of the syntax and features of Manual-C.

Table of Contents
Installation
Getting Started
Syntax Overview
Flags
Variables
Functions
Comments
Examples
Authors
Installation
To use Manual-C, simply clone this repository and run manual-c.exe

Getting Started
Once you run the program, you'll be greeted with a menu. You can choose to enter code, view available commands, see the syntax, learn about the authors, or quit the program.

Syntax Overview
Flags
Flags must be declared at the beginning of your code:

#use-flag <mk>
#use-flag <string>
These flags enable specific features required for your code.

Variables
You can declare variables using the following syntax:

plaintext
Копіювати код
varName = value;
For string variables, wrap the value in double quotes:

greeting = "Hello, World!";
For integer variables, simply assign a number:

number = 42;
Functions
The main function to output text is mkline(). The syntax is as follows:

mkline(argument);
argument can be a string, a variable, or a combination of both.

Concatenation
You can concatenate strings and variables using the + operator:

mkline("Hello, " + name);
mkline("Value: " + number);
Comments
Single-line comments can be added using //:

// This is a comment
Examples
Here’s a simple example to illustrate the usage of Manual-C:


#use-flag <mk>
#use-flag <string>

greeting = "Hello, World!";
number = 42;

mkline(greeting);
mkline("The answer is: " + number);
Expected Output
Hello, World!
The answer is: 42
Authors
This project was developed by mkfs.ext4.
