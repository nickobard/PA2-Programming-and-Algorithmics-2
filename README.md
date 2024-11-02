# C++ Programming Assignments Repository

This repository contains a collection of programming assignments implemented in C++. The projects cover various topics such as data structures, algorithms, object-oriented programming, and more. Each assignment focuses on solving a specific problem, ranging from implementing custom data structures to building a spreadsheet processor.

## Table of Contents

- [Projects Overview](#projects-overview)
    - [Big Homework: Spreadsheet Processor](#big-homework-spreadsheet-processor)
    - [hw01: Real Estate Cadastre Database Implementation](#hw01-real-estate-cadastre-database-implementation)
    - [hw02: Arbitrary Precision Integer Class (CBigInt)](#hw02-arbitrary-precision-integer-class-cbigint)
    - [hw03: Efficient String Manipulation with String Patches (CPatchStr)](#hw03-efficient-string-manipulation-with-string-patches-cpatchstr)
    - [hw04: Student Management System](#hw04-student-management-system)
    - [hw05: Computer Equipment Inventory System](#hw05-computer-equipment-inventory-system)
    - [hw06: Self-Matching Subsequence Finder](#hw06-self-matching-subsequence-finder)
- [File Structure](#file-structure)
- [License](#license)

## Projects Overview

### Big Homework: Spreadsheet Processor

- **Directory**: [big_hw](big_hw)
- **Description**: Implements a spreadsheet processor that allows operations with cells such as setting values, computing expressions, copying ranges, detecting cyclic dependencies, and saving/loading the spreadsheet. The implementation focuses on class design, utilizing polymorphism and appropriate object-oriented programming principles.
- **Details**: See the [README.md](big_hw/README.md) in the project directory for more information.

### hw01: Real Estate Cadastre Database Implementation

- **Directory**: [hw01](hw01)
- **Description**: Implements a simplified real estate cadastre database that stores information about lands and their owners. Provides efficient methods for adding, deleting, and querying lands and owners.
- **Details**: See the [README.md](hw01/README.md) in the project directory for more information.

### hw02: Arbitrary Precision Integer Class (CBigInt)

- **Directory**: [hw02](hw02)
- **Description**: Implements a class `CBigInt` that represents arbitrary size integers with almost unlimited range. Supports basic arithmetic operations like addition and multiplication, as well as comparison operators.
- **Details**: See the [README.md](hw02/README.md) in the project directory for more information.

### hw03: Efficient String Manipulation with String Patches (CPatchStr)

- **Directory**: [hw03](hw03)
- **Description**: Implements a class `CPatchStr` that simulates a string composed of multiple parts (substrings or patches). Optimizes operations like concatenation, insertion, and deletion by reducing the need for copying and memory allocation.
- **Details**: See the [README.md](hw03/README.md) in the project directory for more information.

### hw04: Student Management System

- **Directory**: [hw04](hw04)
- **Description**: Implements classes for managing student records in a study department. Allows adding, removing, searching, and suggesting students based on various criteria.
- **Details**: See the [README.md](hw04/README.md) in the project directory for more information.

### hw05: Computer Equipment Inventory System

- **Directory**: [hw05](hw05)
- **Description**: Simulates a company's computer equipment inventory using class design, inheritance, polymorphism, and abstract methods. Represents networks, computers, and their components such as CPUs, memory, and disks.
- **Details**: See the [README.md](hw05/README.md) in the project directory for more information.

### hw06: Self-Matching Subsequence Finder

- **Directory**: [hw06](hw06)
- **Description**: Implements a template class `CSelfMatch` that searches for contiguous subsequences occurring multiple times within a given sequence. Designed to work with any sequence type.
- **Details**: See the [README.md](hw06/README.md) in the project directory for more information.

## File Structure

```
.
├── big_hw
│   ├── assets
│   ├── CMakeLists.txt
│   ├── main.cpp
│   ├── README.md
│   ├── src
│   ├── tests
│   └── x86_64-linux-gnu
├── hw01
│   ├── main.cpp
│   └── README.md
├── hw02
│   ├── main.cpp
│   └── README.md
├── hw03
│   ├── example.png
│   ├── main.cpp
│   └── README.md
├── hw04
│   ├── main.cpp
│   └── README.md
├── hw05
│   ├── main.cpp
│   └── README.md
├── hw06
│   ├── main.cpp
│   └── README.md
└── .gitignore
```

## License

This project is licensed under the MIT License.

---

Feel free to explore and modify the code to suit your needs. Contributions and suggestions are welcome.