# AoC 2018 day 19

My solution for Advent of Code 2018, day 19. The puzzle description is at
https://adventofcode.com/2018/day/19

This project contains:

* A virtual machine implementation for the AoC instruction set (bulk of the code)
* A more optimized solution for part 2 in C++

Project structure:

| Directory | Description                            |
|-----------|----------------------------------------|
| src/      | VM source files                        |
| test/     | VM unit test files                     |
| data/     | Puzzle input program files             |
| extra/    | Optimized solution for part 2          |

Some features:

* Saves the state of the CPU on SIGINT (Ctrl+C) if the `--state` command line argument is used
* Can continue execution from a state that was saved earlier
* **Register 0** can be set with a command line flag
* The core functionality is unit tested

# Building the project

The project was developed on Ubuntu 18.04 LTS and the required packages can
be found straight from its distro repository.

Requirements:

* CMake 3.10 (might work with older but it's untested)
* Compiler with C++17 support (tested with GCC 7.3)
* Google test framework
* Boost program options library

After you've installed all the requirements, compile the project with:

    mkdir build
    cd build
    cmake ..
    make

After running `make`, you should be able to run unit tests with `ctest`
(or `ctest -V` for more verbose output).

The program executable itself is `build/src/AocRun`.

# Part 2 solution

The algorithm in `program.aoc` is very inefficient. It takes about
two seconds to complete the inner loop once, and the inner loop is repeated
around 10,000,000 times. A rough back-of-the-envelope calculation indicates
that it could take thousands of hours to complete the calculation,
so it was necessary to reverse engineer `program.aoc` to create a more efficient algorithm to solve the problem.

I began the reverse engineering process by creating an annotated version 
of the puzzle input program (`data/program-annotated.aoc`):

    # Annotated version of the program for AoC 2018 day 19
    #
    # Notation for annotations:
    #   X   = direct value X
    #   [X] = register X
    #   jmp = absolute jump
    #   jmr = relative jump, e.g. "jmr 3" jumps to [ip] + 3

                #ip 3

                # Jump to L1 where the target value is initialized in [4]
    start:      addi 3 16 3     # jmp L1

                # Program starts here after initialization
    L4:         seti 1 2 1      # [1] = 1
    L3:         seti 1 1 2      # [2] = 1

                # [5] = [1] * [2]
                # if [5] == [4] then [0] += [1]
                # [2]++
    L2:         mulr 1 2 5      # [5] = [1] * [2]
                eqrr 5 4 5      # [5] = [5] == [4]
                addr 5 3 3      # jmr [5] + 1
                addi 3 1 3      # jmp L5
                addr 1 0 0      # [0] += [1]
    L5:         addi 2 1 2      # [2]++

                # if [2] <= [4] then jump L2
                # else [1]++
                gtrr 2 4 5      # [5] = [2] > [4]
                addr 3 5 3      # jmr [5] + 1
                seti 2 3 3      # jmp L2
                addi 1 1 1      # [1]++

                # if [1] <= [4] then jump to L3
                # else EXIT
                gtrr 1 4 5      # [5] = [1] > [4]
                addr 5 3 3      # jmr [5] + 1
                seti 1 6 3      # jmp L3
                mulr 3 3 3      # EXIT

                # L1 and onward is run only once.
                # Set [4] to 10551361 and jump to L4 (assuming [0] == 1)
    L1:         addi 4 2 4      # [4] += 2
                mulr 4 4 4      # [4] *= [4]
                mulr 3 4 4      # [4] *= 19
                muli 4 11 4     # [4] *= 11
                addi 5 5 5      # [5] += 5
                mulr 5 3 5      # [5] *= 22
                addi 5 15 5     # [5] += 15
                addr 4 5 4      # [4] += [5]
                addr 3 0 3      # jmr [0] + 1
                seti 0 6 3      # jmp L4
                setr 3 5 5      # [5] = 27
                mulr 5 3 5      # [5] *= 28
                addr 3 5 5      # [5] += 29
                mulr 3 5 5      # [5] *= 30
                muli 5 14 5     # [5] *= 14
                mulr 5 3 5      # [5] *= 32
                addr 4 5 4      # [4] += 10550400
                seti 0 5 0      # [0] = 0      # [4] == 10551361
                seti 0 1 3      # jmp L4

With the annotations, it was easier to understand what the program does
at low level. The intermediary steps I took while reverse engineering
the program can be seen in `extra/pseudo1` through `extra/pseudo5`.

The rough flow of the program is as follows:

* Immediately jump to L1, where the *target value* is generated to **register 4** (generates a much larger value when register 0 is set to 1 for part 2 of the puzzle)
* Jump back to L4, which initializes **two counters** in **registers 1 and 2** (I'll call them *x* and *y*)
* Loop through all pairs (*x*, *y*), where *x* and *y* are in [1, *target value*]
* If *x* * *y* == *target value*, then add *x* to **register 0**
* Exit when the outer loop counter *x* reaches *target value*

Or in other words: the program finds the sum of all the divisors for the *target value*.

The algorithm in pseudocode:

    target = 10551361
    sum = 0

    loop from x=1 to x=target:
        loop from y=1 to y=target:
            if x * y = target:
                sum = sum + x

We can see that the algorithm is O(N²).

By using division remainder (`%`), we can optimize:

    loop from x=1 to x=target:
        if target % x = 0:
            sum = sum + x

This new algorithm is O(N) and runs very fast, but there is still room
for optimization:

    loop from x=1 to x=sqrt(target):
        if target % x = 0:
            sum = sum + x
            if x*x != target:
                sum = sum + (target/x)

This solution is implemented in `extra/aoc19-optimized.cpp` and completes in a few milliseconds.
