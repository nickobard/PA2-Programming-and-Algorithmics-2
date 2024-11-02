# Self-Matching Subsequence Finder in C++

This project implements a template class `CSelfMatch` that searches for contiguous subsequences occurring multiple times within a given sequence. It's designed to work with any sequence type, making it applicable for tasks like data compression or plagiarism detection.

## Table of Contents

- [Problem Description](#problem-description)
- [Solution Overview](#solution-overview)
    - [Key Features](#key-features)
    - [Class Interface](#class-interface)
- [Implementation Details](#implementation-details)
    - [Handling Generic Sequences](#handling-generic-sequences)
    - [Naive Algorithm Approach](#naive-algorithm-approach)
    - [Weak Substring Representation](#weak-substring-representation)
    - [Combination Generation](#combination-generation)
- [Usage Examples](#usage-examples)
- [Efficiency Considerations](#efficiency-considerations)
- [Compilation and Testing](#compilation-and-testing)
- [License](#license)

## Problem Description

The task involves creating a class template that can:

1. **Find the length of the longest contiguous subsequence that occurs at least N times in a given sequence.**

2. **Find all combinations of occurrence positions for the longest subsequence that occurs at least N times.**

The sequence can be of any type (characters, integers, pairs, etc.), and the class should handle general sequences.

### Examples:

- For the sequence `ababab` and N=1, the longest subsequence is `ababab` (length 6).
- For N=2, the longest subsequence is `abab` (length 4) at positions 0 and 2.
- For N=3, the longest subsequence is `ab` (length 2) at positions 0, 2, and 4.

## Solution Overview

### Key Features

- **Generic Implementation**: Works with any element type `T_` that supports copying, moving, and equality comparison.
- **Flexible Initialization**: Can be initialized with an initializer list, any container with forward iterators, or a pair of iterators.
- **Dynamic Sequence Building**: Optional `push_back` method to add elements after initialization.
- **Longest Subsequence Length**: `sequenceLen(n)` method finds the length of the longest subsequence occurring at least `n` times.
- **Occurrence Combinations**: `findSequences<N>()` method finds all combinations of positions for the longest subsequence occurring at least `N` times.

### Class Interface

```cpp
template<typename T_>
class CSelfMatch {
public:
    // Constructors
    CSelfMatch(std::initializer_list<T_> init);
    template<typename Container>
    CSelfMatch(const Container& container);
    template<typename Iterator>
    CSelfMatch(Iterator begin, Iterator end);

    // Optional push_back method
    void push_back(const T_& element);
    template<typename... Args>
    void push_back(Args&&... args);

    // Methods
    size_t sequenceLen(uint n) const;
    template<size_t N_>
    std::vector<std::array<size_t, N_>> findSequences() const;

private:
    std::vector<T_> m_data;
};
```

## Implementation Details

### Handling Generic Sequences

- **Element Requirements**: The element type `T_` must support copying, moving, destruction, and equality comparison (`==` and `!=`).
- **Data Storage**: The sequence is stored internally in a `std::vector<T_>`.

### Naive Algorithm Approach

- **sequenceLen(n)**:
    - Iterates over possible subsequence lengths from the maximum possible down to 1.
    - For each length, slides a window over the sequence to extract all possible subsequences.
    - Counts the occurrences of each unique subsequence.
    - Returns the length of the first subsequence found that occurs at least `n` times.

- **findSequences<N>()**:
    - Similar to `sequenceLen(n)` but stores the positions of each occurrence.
    - When a subsequence has at least `N_` occurrences, generates all combinations of these positions.
    - Returns a vector of arrays containing these position combinations.

### Weak Substring Representation

- **Purpose**: Avoids copying subsequences by referencing the original data.
- **Implementation**: `WeakSubstring` class holds a reference to the original data, start position, and length.
- **Comparison**: Overloaded `operator==` to compare subsequences efficiently without copying.

### Combination Generation

- **Recursive Function**: Generates all combinations of positions for subsequences with sufficient occurrences.
- **Template Parameter**: Uses the template parameter `N_` to determine the size of combinations.

## Usage Examples

```cpp
#include <iostream>
#include "CSelfMatch.h"

int main() {
    // Initialize with a string
    CSelfMatch<char> matcher("ababab"s);

    // Find the length of the longest subsequence occurring at least 2 times
    size_t len = matcher.sequenceLen(2);
    std::cout << "Longest subsequence length occurring at least 2 times: " << len << std::endl;

    // Find all position combinations for subsequences of length len occurring at least 2 times
    auto sequences = matcher.findSequences<2>();
    for (const auto& positions : sequences) {
        std::cout << "Positions: ";
        for (size_t pos : positions) {
            std::cout << pos << " ";
        }
        std::cout << std::endl;
    }

    // Using push_back to add elements
    CSelfMatch<int> intMatcher;
    intMatcher.push_back(1, 2, 3, 1, 2, 3);
    size_t intLen = intMatcher.sequenceLen(2);
    std::cout << "Longest subsequence length for integers occurring at least 2 times: " << intLen << std::endl;

    return 0;
}
```

**Sample Output**:

```
Longest subsequence length occurring at least 2 times: 4
Positions: 0 2
Longest subsequence length for integers occurring at least 2 times: 3
```

## Efficiency Considerations

- **Time Complexity**:
    - The naive algorithm has a time complexity of O(N^2) for `sequenceLen` and higher for `findSequences`, where N is the sequence length.
    - Suitable for short sequences and small values of N.

- **Memory Usage**:
    - Stores subsequences and their positions, which can consume significant memory for large sequences.

- **Limitations**:
    - Not efficient for large sequences or high values of N.
    - Does not implement advanced algorithms like suffix trees or hashing.

- **Possible Improvements**:
    - Implement more efficient algorithms for handling large datasets.
    - Use data structures like suffix arrays or rolling hashes to reduce time complexity.

## Compilation and Testing

Ensure you have a C++17 compatible compiler.

### Compilation

```bash
g++ -std=c++17 -o self_match main.cpp
```

### Running Tests

The provided `main` function includes assertions and examples to verify correctness. To run the tests:

```bash
./self_match
```

If all assertions pass, the program will execute successfully.

## License

This project is licensed under the MIT License.

---

Feel free to explore and modify the code to suit your needs. Contributions and suggestions are welcome.