# CBigInt: Arbitrary Precision Integer Class in C++

This project implements a class `CBigInt` that represents arbitrary size integers (both positive and negative) with (almost) unlimited range. The class supports basic arithmetic operations like addition and multiplication, as well as comparison operators. It also includes input and output stream operators for easy interaction.

## Table of Contents

- [Problem Description](#problem-description)
- [Solution Overview](#solution-overview)
    - [Class Design](#class-design)
    - [Constructors](#constructors)
    - [Operator Overloading](#operator-overloading)
    - [Arithmetic Operations](#arithmetic-operations)
    - [Comparison Operators](#comparison-operators)
    - [Input/Output Stream Operators](#inputoutput-stream-operators)
- [Implementation Details](#implementation-details)
    - [Internal Representation](#internal-representation)
    - [Addition](#addition)
    - [Subtraction](#subtraction)
    - [Multiplication](#multiplication)
    - [Handling Negative Numbers](#handling-negative-numbers)
- [Usage Examples](#usage-examples)
- [Efficiency Considerations](#efficiency-considerations)
- [Compilation and Execution](#compilation-and-execution)
- [License](#license)

## Problem Description

The goal is to implement a C++ class `CBigInt` that can handle integers of arbitrary size, overcoming the limitations of built-in types like `int` or `long long int` which have fixed sizes and limited ranges.

The class must support:

- **Constructors**:
    - Default constructor (initializes to zero).
    - Constructor from `int`.
    - Constructor from a C-style string representing a decimal number.
- **Assignment Operator**:
    - Assignment from another `CBigInt`, `int`, or C-style string.
- **Arithmetic Operators**:
    - Addition (`+`, `+=`) and multiplication (`*`, `*=`) with operands being any combination of `CBigInt`, `int`, or C-style string.
- **Comparison Operators**:
    - All relational operators (`<`, `<=`, `>`, `>=`, `==`, `!=`) supporting any combination of `CBigInt`, `int`, or C-style string.
- **Stream Operators**:
    - Output operator `<<` to display the number in decimal representation without leading zeros.
    - Input operator `>>` to read a number from an input stream, handling errors appropriately.

**Constraints and Notes**:

- The class should handle both positive and negative integers.
- The arithmetic operations should be implemented efficiently, especially for large numbers.
- For bonus tests, efficiency is critical, especially for multiplication of very large numbers (e.g., numbers with hundreds of thousands of digits).
- The implementation should avoid unnecessary copies and manage memory efficiently.
- Use of standard containers like `std::vector` and `std::string` is allowed.
- The `<<` operator should support manipulators like `std::hex` to display numbers in hexadecimal format (for bonus tests).

## Solution Overview

### Class Design

The `CBigInt` class encapsulates an arbitrary-size integer with the following main components:

- **Internal Representation**: A `std::string` to store the digits of the number in reverse order (least significant digit first).
- **Sign Indicator**: A `bool` flag to indicate whether the number is negative.

### Constructors

- **Default Constructor**: Initializes the number to zero.
- **Constructor from `int`**: Converts an `int` to `CBigInt`, handling the sign and storing the absolute value.
- **Constructor from C-style String**: Parses the input string to initialize the `CBigInt`. Throws `std::invalid_argument` if the string is not a valid representation of an integer.

### Operator Overloading

The class overloads various operators to support arithmetic and comparison operations between `CBigInt` and other types (`int`, `const char*`).

### Arithmetic Operations

- **Addition (`+`, `+=`)**: Supports addition between any combination of `CBigInt`, `int`, and `const char*`.
- **Multiplication (`*`, `*=`)**: Supports multiplication between any combination of `CBigInt`, `int`, and `const char*`.

### Comparison Operators

All relational operators are overloaded to compare `CBigInt` instances with `int` and `const char*` types.

### Input/Output Stream Operators

- **Output Operator (`<<`)**: Outputs the number to a stream in decimal format without leading zeros. Handles the sign appropriately.
- **Input Operator (`>>`)**: Reads a number from an input stream, parsing until a non-digit character is encountered. Sets the failbit if the input is invalid.

## Implementation Details

### Internal Representation

- The number is stored in a `std::string` called `m_num`, with digits in reverse order to simplify arithmetic operations (e.g., addition, multiplication).
- The sign is stored in a `bool` variable `m_is_negative`, where `true` indicates a negative number.

### Addition

- Implemented using a helper function `add(const string &bigger, const string &lesser)`.
- Performs digit-wise addition with carry, similar to manual addition.
- Operates on the reversed digit strings to simplify indexing.

### Subtraction

- Implemented using a helper function `subtract(const string &bigger, const string &lesser)`.
- Assumes that `bigger` represents the larger absolute value.
- Performs digit-wise subtraction with borrowing.

### Multiplication

- Implemented using a helper function `multiply(const string &multiplied, const string &multiplier)`.
- Uses the elementary school algorithm for multiplication.
- Multiplies each digit of the multiplier with the entire multiplied number and accumulates the results.
- Optimized by skipping multiplication with zero digits.

### Handling Negative Numbers

- The sign of the result is determined based on the signs of the operands.
- Addition and subtraction handle cases where operands have different signs.
- Multiplication sign is determined based on the rules:
    - Positive × Positive = Positive
    - Negative × Negative = Positive
    - Positive × Negative = Negative
    - Negative × Positive = Negative

## Usage Examples

```cpp
#include "CBigInt.h"
#include <iostream>
#include <cassert>

int main() {
    CBigInt a = 1234567890;
    CBigInt b("9876543210");
    CBigInt c = a + b;
    std::cout << "a + b = " << c << std::endl; // Output: a + b = 11111111100

    c = b - a;
    std::cout << "b - a = " << c << std::endl; // Output: b - a = 8641975320

    c = a * b;
    std::cout << "a * b = " << c << std::endl; // Output: a * b = 12193263111263526900

    CBigInt d;
    std::cout << "Enter a big integer: ";
    std::cin >> d;
    std::cout << "You entered: " << d << std::endl;

    assert(a < b);
    assert(c >= a);

    return 0;
}
```

## Efficiency Considerations

- **Addition and Subtraction**:
    - Implemented using simple digit-wise algorithms.
    - Efficient for numbers with a moderate number of digits.
- **Multiplication**:
    - Uses the basic multiplication algorithm, which is less efficient for very large numbers.
    - For bonus tests requiring multiplication of numbers with hundreds of thousands of digits, more advanced algorithms (e.g., Karatsuba or FFT-based multiplication) would be needed.
- **Memory Management**:
    - The use of `std::string` and standard library features helps manage memory efficiently.
    - Avoids unnecessary copies by using references and moving where appropriate.

## Compilation and Execution

To compile the code:

```bash
g++ -std=c++17 -o bigint main.cpp
```

Replace `main.cpp` with the name of your source file containing the `CBigInt` class and `main` function.

To run:

```bash
./bigint
```

## License

This project is licensed under the MIT License.

---

Feel free to explore and modify the code to suit your needs. If you have any questions or suggestions, please don't hesitate to contact me.