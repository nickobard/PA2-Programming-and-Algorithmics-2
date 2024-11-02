# Real Estate Cadastre Database Implementation

This project implements a simplified real estate cadastre database in C++, encapsulated within the `CLandRegister` class. The database stores information about lands and their owners, providing efficient methods for adding, deleting, and querying lands and owners.

## Table of Contents

- [Problem Description](#problem-description)
- [Solution Overview](#solution-overview)
    - [Data Structures](#data-structures)
    - [Class `CLandRegister` Methods](#class-clandregister-methods)
    - [Class `CIterator`](#class-citerator)
- [Efficiency Considerations](#efficiency-considerations)
- [Usage](#usage)
- [Dependencies](#dependencies)
- [Compilation and Execution](#compilation-and-execution)

## Problem Description

The goal is to implement a class `CLandRegister` that functions as a database for a real estate cadastre. Each land record contains the following information:

- **Region** (`region`): The cadastral territory (string).
- **ID** (`id`): The land number within the cadastral territory (non-negative integer).
- **City** (`city`): The city where the land is located (string).
- **Address** (`addr`): The address of the land (string).
- **Owner** (`owner`): The name of the land's owner (string).

A land can be uniquely identified either by the pair `(region, id)` or the pair `(city, addr)`. Ownership is assigned to exactly one owner, and a single owner can own multiple lands. Newly added lands are initially owned by the state (represented by an empty string).

The cadastre must support the following operations:

- Add and delete lands.
- Assign and change land ownership.
- Query the owner of a land.
- Count the number of lands owned by a specific owner.
- List all lands sorted by address.
- List all lands owned by a specific owner in the order they were acquired.

## Solution Overview

### Data Structures

To achieve efficient operations, the following data structures are used:

- **Vectors**: Dynamic arrays (`std::vector`) are used to store and manage the collections of lands and owners.
- **Custom Sorting and Searching**: Binary search (`std::lower_bound`) is employed on sorted vectors to achieve logarithmic time complexity for searches.
- **String Handling**: Owner names are stored in both their original form and in lowercase to allow case-insensitive comparisons.

### Class `CLandRegister` Methods

#### Constructor and Destructor

- **Constructor**: Initializes an empty land register.
- **Destructor**: Cleans up dynamically allocated memory.

#### `add(const string &city, const string &addr, const string &region, unsigned int id)`

Adds a new land to the register. Returns `true` if successful, `false` if a land with the same `(city, addr)` or `(region, id)` already exists.

#### `del(const string &city, const string &addr)` and `del(const string &region, unsigned int id)`

Deletes a land identified by either `(city, addr)` or `(region, id)`. Returns `true` if the land was deleted, `false` if it did not exist.

#### `getOwner(const string &city, const string &addr, string &owner)` and `getOwner(const string &region, unsigned int id, string &owner)`

Retrieves the owner of a land. Returns `true` if successful, `false` if the land does not exist. The owner's name is returned exactly as it was input, preserving case sensitivity.

#### `newOwner(const string &city, const string &addr, const string &owner)` and `newOwner(const string &region, unsigned int id, const string &owner)`

Assigns a new owner to a land. Returns `true` if successful, `false` if the land does not exist or is already owned by the specified owner.

#### `count(const string &owner)`

Returns the number of lands owned by the specified owner, using case-insensitive comparison for the owner's name.

#### `listByAddr()`

Returns a `CIterator` to traverse all lands, sorted by city and address.

#### `listByOwner(const string &owner)`

Returns a `CIterator` to traverse all lands owned by the specified owner, in the order they were acquired.

### Class `CIterator`

The `CIterator` class provides methods to traverse a collection of lands:

- `bool atEnd() const`: Checks if the iterator has reached the end.
- `void next()`: Moves to the next land.
- `string city() const`: Returns the city of the current land.
- `string addr() const`: Returns the address of the current land.
- `string region() const`: Returns the region of the current land.
- `unsigned id() const`: Returns the ID of the current land.
- `string owner() const`: Returns the owner of the current land.

## Efficiency Considerations

- **Search Operations**: By maintaining sorted vectors and using binary search, search operations such as `getOwner`, `newOwner`, and `del` achieve logarithmic time complexity.
- **Ownership Tracking**: Lands are grouped by owners to facilitate efficient counting and listing of owned lands.
- **Memory Management**: The implementation avoids unnecessary copying and leverages pointers and references where appropriate.
- **Ordering Lands**: An order ID is assigned to each land upon acquisition to maintain the sequence in which lands were acquired by an owner.

## Usage

To use the `CLandRegister` class:

1. Include the header file containing the class definitions.
2. Create an instance of `CLandRegister`.
3. Use the provided methods to manage lands and ownership information.

Example:

```cpp
#include "CLandRegister.h"

int main() {
    CLandRegister register;
    register.add("Prague", "Main Street 1", "RegionA", 1001);
    register.add("Brno", "Second Street 2", "RegionB", 2002);

    string owner;
    if (register.getOwner("Prague", "Main Street 1", owner)) {
        cout << "Owner: " << owner << endl;
    }

    register.newOwner("Prague", "Main Street 1", "Alice");

    size_t count = register.count("Alice");
    cout << "Alice owns " << count << " lands." << endl;

    CIterator it = register.listByAddr();
    while (!it.atEnd()) {
        cout << it.city() << ", " << it.addr() << " owned by " << it.owner() << endl;
        it.next();
    }

    return 0;
}
```

## Dependencies

- Standard C++ libraries:
    - `<string>`
    - `<vector>`
    - `<algorithm>`
    - Other standard headers as required.

## Compilation and Execution

To compile the code:

```bash
g++ -std=c++11 -o land_register main.cpp
```

Replace `main.cpp` with the name of your source file containing the `CLandRegister` and `CIterator` classes.

To run:

```bash
./land_register
```

Ensure that your `main` function includes test cases or interactive prompts to utilize the `CLandRegister` class.

Feel free to explore and modify the code to suit your needs. If you have any questions or suggestions, please don't hesitate to contact me.