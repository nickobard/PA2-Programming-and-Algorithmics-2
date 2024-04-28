#ifndef __PROGTEST__

#include <cassert>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <algorithm>
#include <memory>
#include <functional>

#endif /* __PROGTEST__ */

using namespace std;

class CItem {
public:
    virtual ~CItem() = default;

    friend ostream &operator<<(ostream &os, const CItem &item) {
        item.print(os, "", "");
        return os;
    }

    virtual void print(ostream &os, const string &prefix, const string &offset) const = 0;
};

class CComponent : public CItem {
public:

    virtual ~CComponent() = default;

    virtual CComponent &addPartition(int partition_size, const string &id) {
        return *this;
    }

    virtual CComponent *copy() const = 0;

protected:

};

class CCPU : public CComponent {
public:
    CCPU(int cores, int frequency) : m_cores(cores), m_frequency(frequency) {

    }

    CComponent *copy() const override {
        return new CCPU(*this);
    }

    void print(ostream &os, const string &prefix, const string &offset) const override {
        os << offset << prefix;
        os << "CPU, " << m_cores << " cores @ " << m_frequency << "MHz" << endl;
    }

private:
    int m_cores;
    int m_frequency;
};

class CMemory : public CComponent {
public:
    CMemory(int mem_size) : m_mem_size(mem_size) {}

    CComponent *copy() const override {
        return new CMemory(*this);
    }

    void print(ostream &os, const string &prefix, const string &offset) const override {
        os << offset << prefix;
        os << "Memory, " << m_mem_size << " MiB" << endl;
    }

private:
    int m_mem_size;
};

class CDisk : public CComponent {
public:

    CDisk(int disk_type, int disk_size) : m_disk_type(disk_type), m_disk_size(disk_size) {}

    CComponent &addPartition(int partition_size, const string &id) override {
        m_partitions.emplace_back(partition_size, id);
        return *this;
    }

    CComponent *copy() const override {
        return new CDisk(*this);
    }

    void print(ostream &os, const string &prefix, const string &offset) const override {

        string disk_type;
        if (m_disk_type == SSD) {
            disk_type = "SSD";
        } else {
            disk_type = "HDD";
        }

        os << offset << prefix;
        os << disk_type << ", " << m_disk_size << " GiB" << endl;

        for (size_t i = 0; i < m_partitions.size(); i++) {
            if (prefix == "\\-") {
                os << offset + "  ";
            } else {
                os << offset + "| ";
            }
            if (i == m_partitions.size() - 1) {
                os << '\\';
            } else {
                os << '+';
            }
            os << "-[" << i << "]: " << m_partitions[i].first << " GiB, " << m_partitions[i].second << endl;
        }

    }

    constexpr static int SSD = 1;
    constexpr static int MAGNETIC = 2;

private:

    int m_disk_type;
    int m_disk_size;
    vector<pair<int, string>> m_partitions;
};


class CComputer : public CItem {
public:
    CComputer(const string &name) : m_name(name) {}

    CComputer(const CComputer &src) {
        m_name = src.m_name;
        m_addresses = src.m_addresses;
        for (const auto *component: src.m_components) {
            m_components.push_back(component->copy());
        }
    }

    CComputer &operator=(CComputer src) {
        if (this == &src) {
            return *this;
        }
        swap(m_name, src.m_name);
        swap(m_addresses, src.m_addresses);
        swap(m_components, src.m_components);
        return *this;
    }

    ~CComputer() {
        for (const auto *component: m_components) {
            delete component;
        }
    }

    string getName() const {
        return m_name;
    }

    CComputer *copy() const {
        return new CComputer(*this);
    }

    CComputer &addComponent(const CComponent &component) {
        m_components.push_back(component.copy());
        return *this;
    }

    CComputer &addAddress(const string &address) {
        m_addresses.push_back(address);
        return *this;
    }

    void print(ostream &os, const string &prefix, const string &offest) const override {

        os << prefix << "Host: " << m_name << endl;
        for (size_t i = 0; i < m_addresses.size(); i++) {
            os << offest;
            if ((i == m_addresses.size() - 1) && m_components.empty()) {
                os << '\\';
            } else {
                os << '+';
            }
            os << '-' << m_addresses[i] << endl;
        }

        for (size_t i = 0; i < m_components.size(); i++) {
            if (i == m_components.size() - 1) {
                m_components[i]->print(os, "\\-", offest);
            } else {
                m_components[i]->print(os, "+-", offest);
            }
        }

    }

private:
    string m_name;
    vector<string> m_addresses;
    vector<CComponent *> m_components;
};

class CNetwork : public CItem {
public:
    CNetwork(const string &name) : m_name(name) {}

    ~CNetwork() {
        for (const auto *computer: m_computers) {
            delete computer;
        }
    }

    CNetwork(const CNetwork &src) {
        m_name = src.m_name;
        for (const auto *computer: src.m_computers) {
            m_computers.push_back(computer->copy());
        }
    }

    CNetwork &operator=(CNetwork src) {
        if (this == &src) {
            return *this;
        }
        swap(m_name, src.m_name);
        swap(m_computers, src.m_computers);
        return *this;
    }

    CComputer *findComputer(const string &name) {
        for (auto *computer: m_computers) {
            if (computer->getName() == name) {
                return computer;
            }
        }
        return nullptr;
    }

    CNetwork &addComputer(const CComputer &computer) {
        m_computers.push_back(computer.copy());
        return *this;
    }


    void print(ostream &os, const string &prefix, const string &offest) const override {
        os << "Network: " << m_name << endl;
        for (size_t i = 0; i < m_computers.size(); i++) {
            if (i == m_computers.size() - 1) {
                m_computers[i]->print(os, "\\-", "  ");
            } else {
                m_computers[i]->print(os, "+-", "| ");
            }
        }
    }

private:
    string m_name;
    vector<CComputer *> m_computers;
};


#ifndef __PROGTEST__

template<typename T_>
std::string toString(const T_ &x) {
    std::ostringstream oss;
    oss << x;
    return oss.str();
}

int main() {
    CNetwork n("FIT network");
    n.addComputer(
            CComputer("progtest.fit.cvut.cz").
                    addAddress("147.32.232.142").
                    addComponent(CCPU(8, 2400)).
                    addComponent(CCPU(8, 1200)).
                    addComponent(CDisk(CDisk::MAGNETIC, 1500).
                    addPartition(50, "/").
                    addPartition(5, "/boot").
                    addPartition(1000, "/var")).
                    addComponent(CDisk(CDisk::SSD, 60).
                    addPartition(60, "/data")).
                    addComponent(CMemory(2000)).
                    addComponent(CMemory(2000))).
            addComputer(
            CComputer("courses.fit.cvut.cz").
                    addAddress("147.32.232.213").
                    addComponent(CCPU(4, 1600)).
                    addComponent(CMemory(4000)).
                    addComponent(CDisk(CDisk::MAGNETIC, 2000).
                    addPartition(100, "/").
                    addPartition(1900, "/data"))).
            addComputer(
            CComputer("imap.fit.cvut.cz").
                    addAddress("147.32.232.238").
                    addComponent(CCPU(4, 2500)).
                    addAddress("2001:718:2:2901::238").
                    addComponent(CMemory(8000)));
    string toCmp = "Network: FIT network\n"
                   "+-Host: progtest.fit.cvut.cz\n"
                   "| +-147.32.232.142\n"
                   "| +-CPU, 8 cores @ 2400MHz\n"
                   "| +-CPU, 8 cores @ 1200MHz\n"
                   "| +-HDD, 1500 GiB\n"
                   "| | +-[0]: 50 GiB, /\n"
                   "| | +-[1]: 5 GiB, /boot\n"
                   "| | \\-[2]: 1000 GiB, /var\n"
                   "| +-SSD, 60 GiB\n"
                   "| | \\-[0]: 60 GiB, /data\n"
                   "| +-Memory, 2000 MiB\n"
                   "| \\-Memory, 2000 MiB\n"
                   "+-Host: courses.fit.cvut.cz\n"
                   "| +-147.32.232.213\n"
                   "| +-CPU, 4 cores @ 1600MHz\n"
                   "| +-Memory, 4000 MiB\n"
                   "| \\-HDD, 2000 GiB\n"
                   "|   +-[0]: 100 GiB, /\n"
                   "|   \\-[1]: 1900 GiB, /data\n"
                   "\\-Host: imap.fit.cvut.cz\n"
                   "  +-147.32.232.238\n"
                   "  +-2001:718:2:2901::238\n"
                   "  +-CPU, 4 cores @ 2500MHz\n"
                   "  \\-Memory, 8000 MiB\n";
//    cout << toString(n);
//    cout << toCmp;
    assert (toString(n) ==
            "Network: FIT network\n"
            "+-Host: progtest.fit.cvut.cz\n"
            "| +-147.32.232.142\n"
            "| +-CPU, 8 cores @ 2400MHz\n"
            "| +-CPU, 8 cores @ 1200MHz\n"
            "| +-HDD, 1500 GiB\n"
            "| | +-[0]: 50 GiB, /\n"
            "| | +-[1]: 5 GiB, /boot\n"
            "| | \\-[2]: 1000 GiB, /var\n"
            "| +-SSD, 60 GiB\n"
            "| | \\-[0]: 60 GiB, /data\n"
            "| +-Memory, 2000 MiB\n"
            "| \\-Memory, 2000 MiB\n"
            "+-Host: courses.fit.cvut.cz\n"
            "| +-147.32.232.213\n"
            "| +-CPU, 4 cores @ 1600MHz\n"
            "| +-Memory, 4000 MiB\n"
            "| \\-HDD, 2000 GiB\n"
            "|   +-[0]: 100 GiB, /\n"
            "|   \\-[1]: 1900 GiB, /data\n"
            "\\-Host: imap.fit.cvut.cz\n"
            "  +-147.32.232.238\n"
            "  +-2001:718:2:2901::238\n"
            "  +-CPU, 4 cores @ 2500MHz\n"
            "  \\-Memory, 8000 MiB\n");
    CNetwork x = n;
    assert(toString(x) == toString(n));
    auto c = x.findComputer("imap.fit.cvut.cz");
//    cout << toString(*c) << endl;
    assert (toString(*c) ==
            "Host: imap.fit.cvut.cz\n"
            "+-147.32.232.238\n"
            "+-2001:718:2:2901::238\n"
            "+-CPU, 4 cores @ 2500MHz\n"
            "\\-Memory, 8000 MiB\n");
    c->addComponent(CDisk(CDisk::MAGNETIC, 1000).
            addPartition(100, "system").
            addPartition(200, "WWW").
            addPartition(700, "mail"));
    assert (toString(x) ==
            "Network: FIT network\n"
            "+-Host: progtest.fit.cvut.cz\n"
            "| +-147.32.232.142\n"
            "| +-CPU, 8 cores @ 2400MHz\n"
            "| +-CPU, 8 cores @ 1200MHz\n"
            "| +-HDD, 1500 GiB\n"
            "| | +-[0]: 50 GiB, /\n"
            "| | +-[1]: 5 GiB, /boot\n"
            "| | \\-[2]: 1000 GiB, /var\n"
            "| +-SSD, 60 GiB\n"
            "| | \\-[0]: 60 GiB, /data\n"
            "| +-Memory, 2000 MiB\n"
            "| \\-Memory, 2000 MiB\n"
            "+-Host: courses.fit.cvut.cz\n"
            "| +-147.32.232.213\n"
            "| +-CPU, 4 cores @ 1600MHz\n"
            "| +-Memory, 4000 MiB\n"
            "| \\-HDD, 2000 GiB\n"
            "|   +-[0]: 100 GiB, /\n"
            "|   \\-[1]: 1900 GiB, /data\n"
            "\\-Host: imap.fit.cvut.cz\n"
            "  +-147.32.232.238\n"
            "  +-2001:718:2:2901::238\n"
            "  +-CPU, 4 cores @ 2500MHz\n"
            "  +-Memory, 8000 MiB\n"
            "  \\-HDD, 1000 GiB\n"
            "    +-[0]: 100 GiB, system\n"
            "    +-[1]: 200 GiB, WWW\n"
            "    \\-[2]: 700 GiB, mail\n");
    assert (toString(n) ==
            "Network: FIT network\n"
            "+-Host: progtest.fit.cvut.cz\n"
            "| +-147.32.232.142\n"
            "| +-CPU, 8 cores @ 2400MHz\n"
            "| +-CPU, 8 cores @ 1200MHz\n"
            "| +-HDD, 1500 GiB\n"
            "| | +-[0]: 50 GiB, /\n"
            "| | +-[1]: 5 GiB, /boot\n"
            "| | \\-[2]: 1000 GiB, /var\n"
            "| +-SSD, 60 GiB\n"
            "| | \\-[0]: 60 GiB, /data\n"
            "| +-Memory, 2000 MiB\n"
            "| \\-Memory, 2000 MiB\n"
            "+-Host: courses.fit.cvut.cz\n"
            "| +-147.32.232.213\n"
            "| +-CPU, 4 cores @ 1600MHz\n"
            "| +-Memory, 4000 MiB\n"
            "| \\-HDD, 2000 GiB\n"
            "|   +-[0]: 100 GiB, /\n"
            "|   \\-[1]: 1900 GiB, /data\n"
            "\\-Host: imap.fit.cvut.cz\n"
            "  +-147.32.232.238\n"
            "  +-2001:718:2:2901::238\n"
            "  +-CPU, 4 cores @ 2500MHz\n"
            "  \\-Memory, 8000 MiB\n");
    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
