# Student Management System in C++

This project implements classes for managing student records in a study department. The system allows adding, removing, searching, and suggesting students based on various criteria. It utilizes efficient data structures and algorithms to handle large datasets effectively.

## Table of Contents

- [Problem Description](#problem-description)
- [Solution Overview](#solution-overview)
    - [Class Design](#class-design)
    - [Data Structures Used](#data-structures-used)
- [Implementation Details](#implementation-details)
    - [CStudent Class](#cstudent-class)
    - [CFilter Class](#cfilter-class)
    - [CSort Class](#csort-class)
    - [CStudyDept Class](#cstudydept-class)
- [Usage Examples](#usage-examples)
- [Efficiency Considerations](#efficiency-considerations)
- [Compilation and Testing](#compilation-and-testing)
- [License](#license)

## Problem Description

The task is to implement a student management system that can:

- **Add** and **remove** students from the database.
- **Search** for students based on filters like name, date of birth, and year of enrollment.
- **Sort** search results according to specified criteria.
- Provide a **suggestion** feature that helps in autocompleting student names based on partial input.

Each student is uniquely identified by a combination of their **name**, **date of birth**, and **year of enrollment**.

### Key Requirements

- **CStudent Class**: Represents a student with name, date of birth, and year of enrollment.
- **CFilter Class**: Used to filter students during search based on various criteria.
- **CSort Class**: Specifies sorting criteria for the search results.
- **CStudyDept Class**: Represents the student database, providing methods to add, remove, search, and suggest students.

## Solution Overview

### Class Design

The solution involves the following primary classes:

1. **CStudent**: Stores student information and provides comparison operators.
2. **CFilter**: Builds filtering criteria for searching students.
3. **CSort**: Defines sorting preferences for search results.
4. **CStudyDept**: Manages the student database and provides methods for adding, deleting, searching, and suggesting students.

### Data Structures Used

- **std::set**: Used to store students in sorted order for efficient search and retrieval.
- **std::list**: Used for storing and manipulating lists of students during filtering and sorting.
- **std::unordered_set / std::unordered_map**: Not used in this solution to avoid complications with custom hash functions.

## Implementation Details

### CStudent Class

Represents a student with:

- **Attributes**:
    - `std::string m_name`: Student's name.
    - `CDate m_date`: Date of birth.
    - `int m_enrolled`: Year of enrollment.
    - `long long m_id`: Unique identifier for registration order.
    - `std::multiset<std::string> m_filter_name`: Lowercased and split name for filtering.
    - `std::set<std::string> m_suggest_name`: Lowercased and split name for suggestion.

- **Operators**:
    - `operator==` and `operator!=`: Compare students based on name, date of birth, and year of enrollment.
    - `operator<`: Defines ordering for storage in `std::set`.

- **Special Comparators**:
    - `ComparatorByID`: Compares students based on their unique ID.
    - `DefaultComparator`: Compares students based on name, date of birth, and enrollment year.

### CFilter Class

Builds filtering criteria using:

- **Methods**:
    - `name(const std::string&)`: Adds a name criterion. Multiple calls accumulate criteria.
    - `bornBefore(const CDate&)`: Filters students born before a given date.
    - `bornAfter(const CDate&)`: Filters students born after a given date.
    - `enrolledBefore(int)`: Filters students enrolled before a given year.
    - `enrolledAfter(int)`: Filters students enrolled after a given year.
    - `filter(const set<CStudent*, ComparatorByID>&)`: Applies the filter to a set of students.

- **Attributes**:
    - Date and enrollment year bounds for filtering.
    - List of name criteria.

### CSort Class

Defines sorting preferences:

- **Methods**:
    - `addKey(ESortKey, bool)`: Adds a sorting key with ascending or descending order.
    - `operator()`: Comparison operator used for sorting.

- **Attributes**:
    - `std::list<std::pair<ESortKey, bool>> m_keys`: List of sorting criteria.

- **Comparison Logic**:
    - Compares students based on added keys in order.
    - If keys are equal, falls back to registration order (`m_id`).

### CStudyDept Class

Manages the student database:

- **Methods**:
    - `addStudent(const CStudent&)`: Adds a student if not already present.
    - `delStudent(const CStudent&)`: Removes a student if present.
    - `search(const CFilter&, const CSort&)`: Returns a list of students matching the filter, sorted according to `CSort`.
    - `suggest(const std::string&)`: Suggests student names based on partial input.

- **Attributes**:
    - `std::set<CStudent*, DefaultComparator> m_students`: Stores students for quick insertion and lookup.
    - `std::set<CStudent*, ComparatorByID> m_students_by_id`: Maintains students sorted by registration order.
    - `static long long student_id_counter`: Counter to assign unique IDs to students.

## Usage Examples

```cpp
#include "CStudyDept.h"

int main() {
    CStudyDept studyDept;

    // Adding students
    studyDept.addStudent(CStudent("John Doe", CDate(1990, 5, 20), 2010));
    studyDept.addStudent(CStudent("Jane Smith", CDate(1991, 8, 15), 2011));

    // Searching students born after 1989
    CFilter filter;
    filter.bornAfter(CDate(1989, 12, 31));

    CSort sort;
    sort.addKey(ESortKey::NAME, true); // Sort by name ascending

    auto results = studyDept.search(filter, sort);
    for (const auto& student : results) {
        std::cout << student.m_name << " - " << student.m_enrolled << std::endl;
    }

    // Suggesting names
    auto suggestions = studyDept.suggest("Jane");
    for (const auto& name : suggestions) {
        std::cout << name << std::endl; // Output: Jane Smith
    }

    return 0;
}
```

## Efficiency Considerations

- **Data Structures**:
    - Used `std::set` for efficient insertion, deletion, and retrieval.
    - Students are stored in sorted order to facilitate quick searches.

- **Filtering**:
    - The `filter` method in `CFilter` applies all criteria efficiently, minimizing the number of students to consider.

- **Sorting**:
    - Sorting is performed using the `std::list::sort` method with a custom comparator, achieving O(n log n) complexity.

- **Memory Management**:
    - Proper use of pointers and dynamic memory to avoid memory leaks.
    - Destructor in `CStudyDept` cleans up allocated memory.

- **Avoided Linear Time Operations**:
    - Insertions and deletions are faster than linear time due to the use of `std::set`.
    - Suggestions and filtering avoid unnecessary iterations by leveraging efficient data structures.

## Compilation and Testing

### Compilation

Ensure you have a C++11 compatible compiler. Compile the code using:

```bash
g++ -std=c++11 -o student_management main.cpp
```

### Running Tests

The provided `main` function includes assertions that serve as basic tests. To run the tests:

```bash
./student_management
```

If all assertions pass, the program will exit successfully. Otherwise, it will terminate at the failed assertion.

## License

This project is licensed under the MIT License.

---

Feel free to explore and modify the code to suit your needs. If you have any questions or suggestions, please don't hesitate to contact me.