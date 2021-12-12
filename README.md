# simple Arkanoid - a capstone project for C++ Nanodegree Program

As a capstone project for the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213) I have chosen to create a simple remake of classic game Arkanoid.
Therefore I have called the project simpleArkanoid.

REVIEW: remarks about SDL and aditional libraries

<img src="snake_game.gif"/>

## Dependencies for Running Locally

- cmake >= 3.16 (it is likely that the project should build using previous versions)
  - All OSes: [click here for installation instructions](https://cmake.org/install/)
- make >= 4.1 (Linux, Mac), 3.81 (Windows)
  - Linux: make is installed by default on most Linux distros
  - Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  - Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
- SDL2 >= 2.0
  - All installation instructions can be found [here](https://wiki.libsdl.org/Installation)
    > Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source.
- gcc/g++ >= 9.3 (it is likely that the project should build using previous versions)
  - Linux: gcc / g++ is installed by default on most Linux distros
  - Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  - Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./simpleArkanoid`.

## Overview of the code structure REVIEW:

## C++ CAPSTONE PROJECT RUBRICS SATISFACTION

According to my knowledge all README and Compiling and Testing criteria have been passed.

## Additional rubrics satisfaction

### Loops, Functions, I/O

1. CRITERION: The project demonstrates an understanding of C++ functions and control structures
   NOTE: A variety of control structures are used in the project.
   NOTE: The project code is clearly organized into functions.

   WHERE IN CODE: general code structure REVIEW:

   NOTE: functions in GMath, static functions in Randomizer

2. CRITERION: The project reads data from a file and process the data, or the program writes data to a file.
   NOTE: The project accepts input from a user as part of the necessary operation of the program.

   TODO: description

   TODO:
   WHERE IN CODE:

3. CRITERION: The project accepts user input and processes the input.
   NOTE: The project accepts input from a user as part of the necessary operation of the program.

TODO: description

TODO:
WHERE IN CODE:

### Object Oriented Programming

1. CRITERION: The project uses Object Oriented Programming techniques.
   NOTE: The project code is organized into classes with class attributes to hold the data, and class methods to perform tasks.

TODO: description

TODO:
WHERE IN CODE:

2. CRITERION: Classes use appropriate access specifiers for class members.
   NOTE: All class data members are explicitly specified as public, protected, or private.

TODO: description

TODO:
WHERE IN CODE:

3. CRITERION: Class constructors utilize member initialization lists.
   NOTE: All class members that are set to argument values are initialized through member initialization lists.

TODO: description

TODO:
WHERE IN CODE:

4. CRITERION: Classes abstract implementation details from their interfaces.
   NOTE: All class member functions document their effects, either through function names, comments, or formal documentation. Member functions do not change program state in undocumented ways.

TODO: description

TODO:
WHERE IN CODE:

5. CRITERION: Classes encapsulate behavior.
   NOTE: Appropriate data and functions are grouped into classes. Member data that is subject to an invariant is hidden from the user. State is accessed via member functions.

TODO: description

TODO:
WHERE IN CODE:

6. CRITERION: Classes follow an appropriate inheritance hierarchy.
   NOTE: Inheritance hierarchies are logical. Composition is used instead of inheritance when appropriate. Abstract classes are composed of pure virtual functions. Override functions are specified.

TODO: description

TODO:
WHERE IN CODE:

7. CRITERION: Overloaded functions allow the same function to operate on different parameters.
   NOTE: One function is overloaded with different signatures for the same function name.

TODO: description

TODO:
WHERE IN CODE:

9. CRITERION: Derived class functions override virtual base class functions.
   NOTE: One member function in an inherited class overrides a virtual base class member function.

TODO: description

TODO:
WHERE IN CODE:

10. CRITERION: Templates generalize functions in the project.
    NOTE: One function is declared with a template that allows it to accept a generic parameter.

TODO: description

TODO:
WHERE IN CODE:

### Memory Management

1. CRITERION: The project makes use of references in function declarations.
   NOTE: At least two variables are defined as references, or two functions use pass-by-reference in the project code.

TODO: description

TODO:
WHERE IN CODE:

2. CRITERION: The project uses destructors appropriately.
   NOTE: At least one class that uses unmanaged dynamically allocated memory, along with any class that otherwise needs to modify state upon the termination of an object, uses a destructor.

TODO: description

TODO:
WHERE IN CODE:

3. CRITERION: The project uses scope / Resource Acquisition Is Initialization (RAII) where appropriate.
   NOTE: The project follows the Resource Acquisition Is Initialization pattern where appropriate, by allocating objects at compile-time, initializing objects when they are declared, and utilizing scope to ensure their automatic destruction.

TODO: description

TODO:
WHERE IN CODE:

4. CRITERION: The project follows the Rule of 5.
   NOTE: For all classes, if any one of the copy constructor, copy assignment operator, move constructor, move assignment operator, and destructor are defined, then all of these functions are defined.

TODO: description

TODO:
WHERE IN CODE:

5. CRITERION: The project uses move semantics to move data, instead of copying it, where possible.
   NOTE: For classes with move constructors, the project returns objects of that class by value, and relies on the move constructor, instead of copying the object.

TODO: description

TODO:
WHERE IN CODE:

6. CRITERION: The project uses smart pointers instead of raw pointers.
   NOTE: The project uses at least one smart pointer: unique_ptr, shared_ptr, or weak_ptr. The project does not use raw pointers.

TODO: description

TODO:

// REMOVE?

## CC Attribution-ShareAlike 4.0 International

Shield: [![CC BY-SA 4.0][cc-by-sa-shield]][cc-by-sa]

This work is licensed under a
[Creative Commons Attribution-ShareAlike 4.0 International License][cc-by-sa].

[![CC BY-SA 4.0][cc-by-sa-image]][cc-by-sa]

[cc-by-sa]: http://creativecommons.org/licenses/by-sa/4.0/
[cc-by-sa-image]: https://licensebuttons.net/l/by-sa/4.0/88x31.png
[cc-by-sa-shield]: https://img.shields.io/badge/License-CC%20BY--SA%204.0-lightgrey.svg
