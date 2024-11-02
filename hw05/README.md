# Computer Equipment Inventory System in C++

This project implements a set of classes that simulate a company's computer equipment inventory. It focuses on class design using inheritance, polymorphism, and abstract methods to efficiently represent networks, computers, and their components such as CPUs, memory, and disks.

## Table of Contents

- [Problem Description](#problem-description)
- [Solution Overview](#solution-overview)
    - [Class Hierarchy](#class-hierarchy)
    - [Key Classes and Interfaces](#key-classes-and-interfaces)
- [Implementation Details](#implementation-details)
    - [CItem Base Class](#citem-base-class)
    - [CComponent Class](#ccomponent-class)
    - [CCPU Class](#ccpu-class)
    - [CMemory Class](#cmemory-class)
    - [CDisk Class](#cdisk-class)
    - [CComputer Class](#ccomputer-class)
    - [CNetwork Class](#cnetwork-class)
- [Usage Examples](#usage-examples)
- [Efficiency Considerations](#efficiency-considerations)
- [Compilation and Testing](#compilation-and-testing)
- [License](#license)

## Problem Description

The task is to design and implement classes to simulate a company's computer equipment inventory. Specifically, the system should store information about:

- **Networks (`CNetwork`)**
- **Computers (`CComputer`)**
- **Computer Components:**
    - **CPUs (`CCPU`)**
    - **Memory (`CMemory`)**
    - **Disks (`CDisk`)**

The implementation should utilize inheritance, polymorphism, and abstract methods to avoid code repetition and promote extensibility.

### Requirements:

- **CNetwork**:
    - Constructor with network name.
    - Method `addComputer` to add a computer to the network.
    - Method `findComputer` to find a computer by name.
    - Output operator to display the network hierarchy.

- **CComputer**:
    - Constructor with computer name.
    - Method `addComponent` to add components (CPU, Memory, Disk).
    - Method `addAddress` to add addresses (strings).
    - Output operator to display the computer's addresses and components.

- **CCPU**:
    - Constructor with the number of cores and frequency in MHz.

- **CMemory**:
    - Constructor with memory size in MiB.

- **CDisk**:
    - Constructor with disk type (`SSD` or `MAGNETIC`) and size in GiB.
    - Method `addPartition` to add disk partitions with size and identification.

**Note**: The system should display the inventory in a tree-like structure, and the implementation should avoid using `dynamic_cast`, `typeid`, or RTTI.

## Solution Overview

### Class Hierarchy

The classes are organized into a hierarchy to leverage inheritance and polymorphism:

- **CItem** (Abstract Base Class)
    - **CComponent** (Abstract Class)
        - **CCPU**
        - **CMemory**
        - **CDisk**
    - **CComputer**
    - **CNetwork**

### Key Classes and Interfaces

- **CItem**: An abstract base class that provides a common interface for printing items.
- **CComponent**: An abstract class derived from `CItem` for computer components.
- **CCPU**: Represents a CPU component.
- **CMemory**: Represents a memory component.
- **CDisk**: Represents a disk component, supports adding partitions.
- **CComputer**: Represents a computer, holds addresses and components.
- **CNetwork**: Represents a network, holds computers.

## Implementation Details

### CItem Base Class

An abstract class providing a virtual `print` method and an overloaded output operator `<<`:

```cpp
class CItem {
public:
    virtual ~CItem() = default;
    virtual void print(std::ostream& os, const std::string& prefix, const std::string& offset) const = 0;
    friend std::ostream& operator<<(std::ostream& os, const CItem& item) {
        item.print(os, "", "");
        return os;
    }
};
```

### CComponent Class

An abstract class derived from `CItem`, serving as a base for all components:

```cpp
class CComponent : public CItem {
public:
    virtual ~CComponent() = default;
    virtual CComponent* copy() const = 0;
    virtual CComponent& addPartition(int partition_size, const std::string& id) {
        // Default implementation does nothing.
        return *this;
    }
};
```

### CCPU Class

Represents a CPU component:

- **Attributes**:
    - `int m_cores`: Number of cores.
    - `int m_frequency`: Frequency in MHz.
- **Methods**:
    - Constructor with cores and frequency.
    - `void print(...) const` to display CPU details.
    - `CComponent* copy() const` for cloning.

```cpp
class CCPU : public CComponent {
public:
    CCPU(int cores, int frequency);
    void print(std::ostream& os, const std::string& prefix, const std::string& offset) const override;
    CComponent* copy() const override;
private:
    int m_cores;
    int m_frequency;
};
```

### CMemory Class

Represents a memory component:

- **Attributes**:
    - `int m_mem_size`: Memory size in MiB.
- **Methods**:
    - Constructor with memory size.
    - `void print(...) const` to display memory details.
    - `CComponent* copy() const` for cloning.

```cpp
class CMemory : public CComponent {
public:
    CMemory(int mem_size);
    void print(std::ostream& os, const std::string& prefix, const std::string& offset) const override;
    CComponent* copy() const override;
private:
    int m_mem_size;
};
```

### CDisk Class

Represents a disk component:

- **Attributes**:
    - `int m_disk_type`: Disk type (`SSD` or `MAGNETIC`).
    - `int m_disk_size`: Disk size in GiB.
    - `std::vector<std::pair<int, std::string>> m_partitions`: List of partitions.
- **Methods**:
    - Constructor with disk type and size.
    - `CDisk& addPartition(int partition_size, const std::string& id)` to add partitions.
    - `void print(...) const` to display disk and partition details.
    - `CComponent* copy() const` for cloning.

```cpp
class CDisk : public CComponent {
public:
    static constexpr int SSD = 1;
    static constexpr int MAGNETIC = 2;
    CDisk(int disk_type, int disk_size);
    CDisk& addPartition(int partition_size, const std::string& id) override;
    void print(std::ostream& os, const std::string& prefix, const std::string& offset) const override;
    CComponent* copy() const override;
private:
    int m_disk_type;
    int m_disk_size;
    std::vector<std::pair<int, std::string>> m_partitions;
};
```

### CComputer Class

Represents a computer:

- **Attributes**:
    - `std::string m_name`: Computer name.
    - `std::vector<std::string> m_addresses`: List of addresses.
    - `std::vector<CComponent*> m_components`: List of components.
- **Methods**:
    - Constructor with computer name.
    - Destructor to clean up components.
    - Copy constructor and assignment operator for deep copying.
    - `CComputer& addComponent(const CComponent& component)` to add components.
    - `CComputer& addAddress(const std::string& address)` to add addresses.
    - `void print(...) const` to display computer details.
    - `CComputer* copy() const` for cloning.
    - `std::string getName() const` to get the computer's name.

```cpp
class CComputer : public CItem {
public:
    CComputer(const std::string& name);
    CComputer(const CComputer& src);
    CComputer& operator=(CComputer src);
    ~CComputer();
    CComputer& addComponent(const CComponent& component);
    CComputer& addAddress(const std::string& address);
    void print(std::ostream& os, const std::string& prefix, const std::string& offset) const override;
    CComputer* copy() const;
    std::string getName() const;
private:
    std::string m_name;
    std::vector<std::string> m_addresses;
    std::vector<CComponent*> m_components;
};
```

### CNetwork Class

Represents a network:

- **Attributes**:
    - `std::string m_name`: Network name.
    - `std::vector<CComputer*> m_computers`: List of computers.
- **Methods**:
    - Constructor with network name.
    - Destructor to clean up computers.
    - Copy constructor and assignment operator for deep copying.
    - `CNetwork& addComputer(const CComputer& computer)` to add computers.
    - `CComputer* findComputer(const std::string& name)` to find a computer by name.
    - `void print(...) const` to display network details.

```cpp
class CNetwork : public CItem {
public:
    CNetwork(const std::string& name);
    CNetwork(const CNetwork& src);
    CNetwork& operator=(CNetwork src);
    ~CNetwork();
    CNetwork& addComputer(const CComputer& computer);
    CComputer* findComputer(const std::string& name);
    void print(std::ostream& os, const std::string& prefix, const std::string& offset) const override;
private:
    std::string m_name;
    std::vector<CComputer*> m_computers;
};
```

## Usage Examples

```cpp
#include <iostream>

int main() {
    CNetwork network("Company Network");

    network.addComputer(
        CComputer("server.company.com")
            .addAddress("192.168.1.1")
            .addComponent(CCPU(8, 3200))
            .addComponent(CMemory(16384))
            .addComponent(CDisk(CDisk::SSD, 512).addPartition(512, "/"))
    );

    network.addComputer(
        CComputer("workstation.company.com")
            .addAddress("192.168.1.2")
            .addComponent(CCPU(4, 2400))
            .addComponent(CMemory(8192))
            .addComponent(CDisk(CDisk::MAGNETIC, 1024)
                .addPartition(128, "System")
                .addPartition(896, "Data"))
    );

    std::cout << network << std::endl;

    // Find a computer and add a component
    CComputer* comp = network.findComputer("workstation.company.com");
    if (comp) {
        comp->addComponent(CMemory(8192)); // Add additional RAM
    }

    std::cout << network << std::endl;

    return 0;
}
```

**Sample Output**:

```
Network: Company Network
+-Host: server.company.com
| +-192.168.1.1
| +-CPU, 8 cores @ 3200MHz
| +-Memory, 16384 MiB
| \-SSD, 512 GiB
|   \-[0]: 512 GiB, /
\-Host: workstation.company.com
  +-192.168.1.2
  +-CPU, 4 cores @ 2400MHz
  +-Memory, 8192 MiB
  \-HDD, 1024 GiB
    +-[0]: 128 GiB, System
    \-[1]: 896 GiB, Data
```

## Efficiency Considerations

- **Inheritance and Polymorphism**: Utilized to reduce code duplication and enhance extensibility.
- **Avoiding RTTI and Type Casting**: The design avoids using `dynamic_cast` and `typeid`, relying instead on virtual functions and polymorphism.
- **Memory Management**:
    - Proper handling of copy constructors and assignment operators to ensure deep copying where necessary.
    - Destructors clean up dynamically allocated memory to prevent memory leaks.

## Compilation and Testing

Ensure you have a C++11 compatible compiler.

### Compilation

```bash
g++ -std=c++11 -o inventory main.cpp
```

### Running the Program

```bash
./inventory
```

### Testing

The provided code includes assertions to verify correctness. Ensure all assertions pass during testing.

## License

This project is licensed under the MIT License.

---

Feel free to explore and modify the code to suit your needs. If you have any questions or suggestions, please don't hesitate to contact me.