# Spreadsheet Processor in C++

This project implements a spreadsheet processor that allows operations with cells such as setting values, computing expressions, copying ranges, detecting cyclic dependencies, and saving/loading the spreadsheet. The implementation focuses on class design, utilizing polymorphism and appropriate object-oriented programming principles.

## Table of Contents

- [Problem Description](#problem-description)
    - [Key Requirements](#key-requirements)
- [Solution Overview](#solution-overview)
    - [Class Structure](#class-structure)
    - [Key Features](#key-features)
- [Implementation Details](#implementation-details)
    - [CSpreadsheet Class](#cspreadsheet-class)
    - [CPos Class](#cpos-class)
    - [CCell Class](#ccell-class)
    - [Expression Evaluation](#expression-evaluation)
    - [Function Support](#function-support)
    - [Cycle Detection](#cycle-detection)
    - [Saving and Loading](#saving-and-loading)
- [Usage Examples](#usage-examples)
- [Compilation and Testing](#compilation-and-testing)
    - [Compilation](#compilation)
    - [Running Tests](#running-tests)
- [Files Structure](#files-structure)
- [License](#license)

## Problem Description

The task is to implement a class (or a set of classes) that functions as a spreadsheet processor. The spreadsheet should support the following functionalities:

- **Cell Operations**: Setting cell values, computing values based on expressions, and copying cells.
- **Expression Evaluation**: Cells can contain numbers, strings, or formulas that compute values based on other cells.
- **Cyclic Dependency Detection**: The system should detect cyclic dependencies between cells to prevent infinite loops.
- **Function Support**: Implement functions like `sum`, `count`, `min`, `max`, `countval`, and `if`.
- **Saving and Loading**: The spreadsheet can be saved to and loaded from a file.

### Key Requirements

- **Classes**:
    - `CSpreadsheet`: The main class representing the spreadsheet.
    - `CPos`: Represents the position of a cell in the spreadsheet.
    - `CValue`: Represents the value of a cell (undefined, number, or string).
    - `CExpressionBuilder`: An abstract class used by the provided expression parser.
- **Expression Parsing**: Use the provided `parseExpression()` function for parsing expressions in cells.
- **Cell Identification**: Cells are identified using standard spreadsheet notation (e.g., `A1`, `B2`), with support for absolute and relative references.
- **Functionality**:
    - Support for numerical literals, string literals, cell references, ranges, functions, and operators.
    - Correct handling of operator precedence and associativity.

## Solution Overview

### Class Structure

The solution is organized into several classes, each responsible for a specific part of the spreadsheet functionality:

- **`CSpreadsheet`**: Manages the overall spreadsheet, including cells and their interactions.
- **`CPos`**: Handles cell positions, parsing, and validation of cell identifiers.
- **`CCell`**: Represents individual cells in the spreadsheet, including their content and value.
- **`CValue`**: A type-safe union (using `std::variant`) that can hold different types of cell values.
- **Expression Evaluation Classes**: Classes for parsing and evaluating expressions, including support for abstract syntax trees (AST).

### Key Features

- **Cell Content Management**: Ability to set and get cell contents, whether they are literals or expressions.
- **Expression Evaluation**: Efficient evaluation of cell expressions using AST to avoid re-parsing.
- **Function Support**: Implements required spreadsheet functions (`sum`, `count`, `min`, `max`, `countval`, `if`).
- **Cycle Detection**: Detects and handles cyclic dependencies between cells to prevent infinite recursion.
- **Copying Cells**: Supports copying ranges of cells with proper adjustment of relative and absolute references.
- **Saving and Loading**: Ability to save the spreadsheet to a file and load it back, with error handling for corrupted files.

## Implementation Details

### CSpreadsheet Class

Represents the spreadsheet and provides methods to manipulate it.

- **Methods**:
    - `setCell(CPos pos, std::string contents)`: Sets the content of a cell at the given position.
    - `getValue(CPos pos)`: Retrieves the value of a cell, evaluating expressions if necessary.
    - `copyRect(CPos dst, CPos src, int w, int h)`: Copies a rectangular block of cells from `src` to `dst`.
    - `save(std::ostream& os)`: Saves the spreadsheet to an output stream.
    - `load(std::istream& is)`: Loads the spreadsheet from an input stream.

**Example**:

```cpp
CSpreadsheet sheet;
sheet.setCell(CPos("A1"), "42");
sheet.setCell(CPos("A2"), "=A1 * 2");
CValue value = sheet.getValue(CPos("A2")); // Should be 84
```

### CPos Class

Handles the parsing and validation of cell positions.

- **Constructor**: Accepts a string like `"A1"` and parses it into row and column indices.
- **Methods**:
    - `getCoords()`: Returns the numerical row and column indices.
    - `isValid()`: Checks if the position is valid.

### CCell Class

Represents a single cell in the spreadsheet.

- **Types of Cells**:
    - **LiteralCell**: Contains a literal value (number or string).
    - **ExpressionCell**: Contains an expression that needs to be evaluated.
- **Methods**:
    - `getValue(CSpreadsheet& spreadsheet)`: Evaluates the cell's value.
    - `copy()`: Creates a deep copy of the cell.

### Expression Evaluation

Expressions in cells are parsed and converted into an Abstract Syntax Tree (AST) using the provided `parseExpression()` function and a custom `CExpressionBuilder` subclass.

- **AST Nodes**:
    - **ValueNode**: Represents a literal value.
    - **BinaryOperationNode**: Represents binary operations like addition, subtraction, etc.
    - **UnaryOperationNode**: Represents unary operations like negation.
    - **FunctionNode**: Represents function calls.
- **Evaluation**:
    - Nodes are evaluated recursively.
    - Variables and cell references are resolved during evaluation.

### Function Support

The following functions are implemented:

- `sum(range)`: Sums all numeric values in the specified range.
- `count(range)`: Counts the number of defined cells in the range.
- `min(range)`: Finds the minimum numeric value in the range.
- `max(range)`: Finds the maximum numeric value in the range.
- `countval(value, range)`: Counts occurrences of a value in the range.
- `if(cond, ifTrue, ifFalse)`: Evaluates a condition and returns one of two values.

### Cycle Detection

To prevent infinite loops caused by cyclic dependencies, the implementation includes cycle detection:

- **Cycle Detection Visitor**: Traverses the dependencies and throws an exception if a cycle is detected.
- **Handling Cycles**: When a cycle is detected, the cell's value is set to undefined.

### Saving and Loading

The spreadsheet can be saved to and loaded from streams:

- **Saving**:
    - Serializes the spreadsheet's data, including cell contents and positions.
    - Handles special characters and ensures data integrity.
- **Loading**:
    - Parses the input stream to reconstruct the spreadsheet.
    - Validates the data and handles errors gracefully.

## Usage Examples

```cpp
#include "CSpreadsheet.h"

int main() {
    CSpreadsheet sheet;

    // Setting cells with literals
    sheet.setCell(CPos("A1"), "10");
    sheet.setCell(CPos("A2"), "20");

    // Setting a cell with an expression
    sheet.setCell(CPos("A3"), "=A1 + A2");

    // Getting cell values
    CValue val = sheet.getValue(CPos("A3")); // Should be 30

    // Using functions
    sheet.setCell(CPos("A4"), "=sum(A1:A3)");
    CValue sumVal = sheet.getValue(CPos("A4")); // Should be 60

    // Detecting cycles
    sheet.setCell(CPos("B1"), "=B2");
    sheet.setCell(CPos("B2"), "=B1");
    CValue cycleVal = sheet.getValue(CPos("B1")); // Undefined due to cycle

    // Saving to a file
    std::ofstream outFile("spreadsheet.dat");
    sheet.save(outFile);
    outFile.close();

    // Loading from a file
    CSpreadsheet loadedSheet;
    std::ifstream inFile("spreadsheet.dat");
    loadedSheet.load(inFile);
    inFile.close();

    return 0;
}
```

## Compilation and Testing

### Compilation

Ensure you have a C++21 compatible compiler. The project uses standard C++ libraries and provided `libexpression_parser.a` static library.



### Running Tests

Tests are provided in the `tests` directory, using assertions to verify functionality.

```cpp
#include "Tester.h"

int main() {
    Tester::runAll();
    return 0;
}
```

Run the compiled test executable:

```bash
./spreadsheet_tests
```

## Files Structure

```
.
├── assets
│   ├── combine.sh
│   ├── expression.h
│   ├── .gitignore
│   ├── progtest.cpp
│   ├── progt.sh
│   └── template.cpp
├── CMakeLists.txt
├── .gitignore
├── main.cpp
├── src
│   ├── CSpreadsheet.cpp
│   ├── CSpreadsheet.h
│   ├── ExpressionBuilders
│   │   ├── ASTNodes
│   │   │   ├── BinaryOperationNode.cpp
│   │   │   ├── BinaryOperationNode.h
│   │   │   ├── CASTNode.cpp
│   │   │   ├── CASTNode.h
│   │   │   ├── FunctionNode.cpp
│   │   │   ├── FunctionNode.h
│   │   │   ├── RelationalOperationNode.cpp
│   │   │   ├── RelationalOperationNode.h
│   │   │   ├── UnaryOperationNode.cpp
│   │   │   └── UnaryOperationNode.h
│   │   ├── CASTExpressionBuilder.cpp
│   │   ├── CASTExpressionBuilder.h
│   │   ├── CExprBuilder.h
│   │   └── CycleDetectionVisitor
│   │       ├── CCycleDetectionVisitor.cpp
│   │       └── CCycleDetectionVisitor.h
│   ├── InputOutputUtilities
│   │   ├── CLoader.cpp
│   │   └── CLoader.h
│   └── SpreadsheetStructure
│       ├── CCell.cpp
│       ├── CCell.h
│       ├── CPos.cpp
│       ├── CPos.h
│       ├── CRange.cpp
│       └── CRange.h
├── tests
│   └── Tester.h
└── x86_64-linux-gnu
    └── libexpression_parser.a

10 directories, 36 files

```

## License

This project is licensed under the MIT License.

---

Feel free to explore and modify the code to suit your needs. If you have any questions or suggestions, please don't hesitate to contact the author.