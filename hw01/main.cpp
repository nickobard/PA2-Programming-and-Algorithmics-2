#ifndef __PROGTEST__

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <utility>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <memory>

#endif /* __PROGTEST__ */

using namespace std;

struct Land {
    Land() {
        m_region_id.second = 0;
        m_order_id = 0;
    }

    Land(string region, size_t id, string city, string addr, size_t order_id) :
            m_region_id({std::move(region), id}),
            m_city_addr({std::move(city), std::move(addr)}),
            m_order_id(order_id) {}

    friend bool operator==(const Land &lhs, const Land &rhs);

    pair<string, size_t> m_region_id;
    pair<string, string> m_city_addr;
    string m_owner;
    string m_owner_lower_case;
    size_t m_order_id;
};

bool operator==(const Land &lhs, const Land &rhs) {
    return (lhs.m_region_id == rhs.m_region_id) ||
           (lhs.m_city_addr == rhs.m_city_addr);
}

class CIterator {
public:
    using iterator_dt = vector<Land *>;

    explicit CIterator(iterator_dt lands) : m_lands(std::move(lands)) {
        m_it = m_lands.cbegin();
    }

    bool atEnd() const {
        return m_it == m_lands.cend();
    }

    void next() {
        m_it++;
    }

    string city() const {
        return (*m_it)->m_city_addr.first;
    }

    string addr() const {
        return (*m_it)->m_city_addr.second;
    }

    string region() const {
        return (*m_it)->m_region_id.first;
    }

    unsigned id() const {
        return (*m_it)->m_region_id.second;
    }

    string owner() const {
        return (*m_it)->m_owner;
    }

private:
    iterator_dt::const_iterator m_it;
    const iterator_dt m_lands;
};

class CLandRegister {
public:
    ~CLandRegister() {
        for (const Land *l: m_lands_by_region_id) {
            delete l;
        }
    }

    bool add(const string &city,
             const string &addr,
             const string &region,
             unsigned int id) {

        auto key_reg_id = pair<string, size_t>(region, id);
        auto it_reg_id = lower_bound(m_lands_by_region_id.begin(), m_lands_by_region_id.end(),
                                     key_reg_id,
                                     [](const Land *land, const pair<string, size_t> &key) {
                                         return land->m_region_id < key;
                                     });
        if (it_reg_id != m_lands_by_region_id.end() && (*it_reg_id)->m_region_id == key_reg_id) {
            return false;
        }

        auto key_city_addr = pair<string, string>(city, addr);
        auto it_city_addr = lower_bound(m_lands_by_city_addr.begin(), m_lands_by_city_addr.end(),
                                        key_city_addr,
                                        [](const Land *land, const pair<string, string> &key) {
                                            return land->m_city_addr < key;
                                        });
        if (it_city_addr != m_lands_by_city_addr.end() && (*it_city_addr)->m_city_addr == key_city_addr) {
            return false;
        }

        Land *land = new Land(region, id, city, addr, m_order_counter++);
        m_lands_by_region_id.insert(it_reg_id, land);
        m_lands_by_city_addr.insert(it_city_addr, land);
        return true;
    }

    bool del(const string &city,
             const string &addr) {
        for (auto it = m_lands.begin(); it != m_lands.end(); it++) {
            if (it->m_city_addr == pair<string, string>(city, addr)) {
                m_lands.erase(it);
                return true;
            }
        }
        return false;
    }

    bool del(const string &region,
             unsigned int id) {
        for (auto it = m_lands.begin(); it != m_lands.end(); it++) {
            if (it->m_region_id == pair<string, size_t>(region, id)) {
                m_lands.erase(it);
                return true;
            }
        }
        return false;
    }

    bool getOwner(const string &city,
                  const string &addr,
                  string &owner) const {
        auto it = findByCityAndAddr(city, addr);
        if (it == m_lands_by_city_addr.cend()) {
            return false;
        }
        owner = (*it)->m_owner;
        return true;
    }

    bool getOwner(const string &region,
                  unsigned int id,
                  string &owner) const {
        auto it = findByRegionAndID(region, id);
        if (it == m_lands_by_region_id.cend()) {
            return false;
        }
        owner = (*it)->m_owner;
        return true;
    }

    bool newOwner(const string &city,
                  const string &addr,
                  const string &owner) {
        auto it = findByCityAndAddr(city, addr);
        if (it != m_lands_by_city_addr.end()) {
            string owner_lower_case = to_lower(owner);
            if ((*it)->m_owner_lower_case == owner_lower_case) {
                return false;
            }
            (*it)->m_owner = owner;
            (*it)->m_owner_lower_case = owner_lower_case;
            (*it)->m_order_id = m_order_counter++;
            return true;
        }
        return false;
    }

    bool newOwner(const string &region,
                  unsigned int id,
                  const string &owner) {
        auto it = findByRegionAndID(region, id);
        if (it != m_lands_by_region_id.end()) {
            string owner_lower_case = to_lower(owner);
            if ((*it)->m_owner_lower_case == owner_lower_case) {
                return false;
            }
            (*it)->m_owner = owner;
            (*it)->m_owner_lower_case = owner_lower_case;
            (*it)->m_order_id = m_order_counter++;
            return true;
        }
        return false;
    }

    size_t count(const string &owner) const {
        string lower_cased_owner = to_lower(owner);
        size_t count = 0;
        for (const Land *l: m_lands_by_region_id) {
            if (l->m_owner_lower_case == lower_cased_owner) {
                count++;
            }
        }
        return count;
    }

    CIterator listByAddr() const {
        return CIterator(m_lands_by_city_addr);
    }

    CIterator listByOwner(const string &owner) const {
        const string owner_lower = to_lower(owner);
        vector<Land *> by_owner;
        for (Land *l: m_lands_by_region_id) {
            if (l->m_owner_lower_case == owner_lower) {
                by_owner.push_back(l);
            }
        }
        sort(by_owner.begin(), by_owner.end(), [](const Land *lhs, const Land *rhs) {
            return lhs->m_order_id < rhs->m_order_id;
        });
        return CIterator(by_owner);
    }

private:

    static string to_lower(string str) {
        for (char &c: str) {
            c = (char) tolower(c);
        }
        return str;
    }

    vector<Land *>::iterator findByRegionAndID(const string &region, unsigned int id) {
        auto key = pair<string, size_t>(region, id);
        auto it_reg_id = lower_bound(m_lands_by_region_id.begin(), m_lands_by_region_id.end(),
                                     key,
                                     [](const Land *land, const pair<string, size_t> &key) {
                                         return land->m_region_id < key;
                                     });
        if (it_reg_id != m_lands_by_region_id.end() && (*it_reg_id)->m_region_id == key) {
            return it_reg_id;
        }
        return m_lands_by_region_id.end();
    }

    vector<Land *>::const_iterator findByRegionAndID(const string &region, unsigned int id) const {
        auto key = pair<string, size_t>(region, id);
        auto it_reg_id = lower_bound(m_lands_by_region_id.cbegin(), m_lands_by_region_id.cend(),
                                     key,
                                     [](const Land *land, const pair<string, size_t> &key) {
                                         return land->m_region_id < key;
                                     });
        if (it_reg_id != m_lands_by_region_id.end() && (*it_reg_id)->m_region_id == key) {
            return it_reg_id;
        }
        return m_lands_by_region_id.cend();
    }

    vector<Land *>::iterator findByCityAndAddr(const string &city, const string &addr) {
        auto key = pair<string, string>(city, addr);
        auto it_city_addr = lower_bound(m_lands_by_city_addr.begin(), m_lands_by_city_addr.end(),
                                        key,
                                        [](const Land *land, const pair<string, string> &key) {
                                            return land->m_city_addr < key;
                                        });
        if (it_city_addr != m_lands_by_city_addr.end() && (*it_city_addr)->m_city_addr == key) {
            return it_city_addr;
        }
        return m_lands_by_city_addr.end();
    }

    vector<Land *>::const_iterator findByCityAndAddr(const string &city, const string &addr) const {
        auto key = pair<string, string>(city, addr);
        auto it_city_addr = lower_bound(m_lands_by_city_addr.cbegin(), m_lands_by_city_addr.cend(),
                                        key,
                                        [](const Land *land, const pair<string, string> &key) {
                                            return land->m_city_addr < key;
                                        });
        if (it_city_addr != m_lands_by_city_addr.end() && (*it_city_addr)->m_city_addr == key) {
            return it_city_addr;
        }
        return m_lands_by_city_addr.cend();
    }

    vector<Land *> m_lands_by_region_id;
    vector<Land *> m_lands_by_city_addr;
    list<Land> m_lands;
    size_t m_order_counter = 0;
};

#ifndef __PROGTEST__

static void test0() {
    CLandRegister x;
    std::string owner;

    assert (x.add("Prague", "Thakurova", "Dejvice", 12345));
    assert (x.add("Prague", "Evropska", "Vokovice", 12345));
    assert (x.add("Prague", "Technicka", "Dejvice", 9873));
    assert (x.add("Plzen", "Evropska", "Plzen mesto", 78901));
    assert (x.add("Liberec", "Evropska", "Librec", 4552));
    CIterator i0 = x.listByAddr();
    assert (!i0.atEnd()
            && i0.city() == "Liberec"
            && i0.addr() == "Evropska"
            && i0.region() == "Librec"
            && i0.id() == 4552
            && i0.owner() == "");
    i0.next();
    assert (!i0.atEnd()
            && i0.city() == "Plzen"
            && i0.addr() == "Evropska"
            && i0.region() == "Plzen mesto"
            && i0.id() == 78901
            && i0.owner() == "");
    i0.next();
    assert (!i0.atEnd()
            && i0.city() == "Prague"
            && i0.addr() == "Evropska"
            && i0.region() == "Vokovice"
            && i0.id() == 12345
            && i0.owner() == "");
    i0.next();
    assert (!i0.atEnd()
            && i0.city() == "Prague"
            && i0.addr() == "Technicka"
            && i0.region() == "Dejvice"
            && i0.id() == 9873
            && i0.owner() == "");
    i0.next();
    assert (!i0.atEnd()
            && i0.city() == "Prague"
            && i0.addr() == "Thakurova"
            && i0.region() == "Dejvice"
            && i0.id() == 12345
            && i0.owner() == "");
    i0.next();
    assert (i0.atEnd());

    assert (x.count("") == 5);
    CIterator i1 = x.listByOwner("");
    assert (!i1.atEnd()
            && i1.city() == "Prague"
            && i1.addr() == "Thakurova"
            && i1.region() == "Dejvice"
            && i1.id() == 12345
            && i1.owner() == "");
    i1.next();
    assert (!i1.atEnd()
            && i1.city() == "Prague"
            && i1.addr() == "Evropska"
            && i1.region() == "Vokovice"
            && i1.id() == 12345
            && i1.owner() == "");
    i1.next();
    assert (!i1.atEnd()
            && i1.city() == "Prague"
            && i1.addr() == "Technicka"
            && i1.region() == "Dejvice"
            && i1.id() == 9873
            && i1.owner() == "");
    i1.next();
    assert (!i1.atEnd()
            && i1.city() == "Plzen"
            && i1.addr() == "Evropska"
            && i1.region() == "Plzen mesto"
            && i1.id() == 78901
            && i1.owner() == "");
    i1.next();
    assert (!i1.atEnd()
            && i1.city() == "Liberec"
            && i1.addr() == "Evropska"
            && i1.region() == "Librec"
            && i1.id() == 4552
            && i1.owner() == "");
    i1.next();
    assert (i1.atEnd());

    assert (x.count("CVUT") == 0);
    CIterator i2 = x.listByOwner("CVUT");
    assert (i2.atEnd());

    assert (x.newOwner("Prague", "Thakurova", "CVUT"));
    assert (x.newOwner("Dejvice", 9873, "CVUT"));
    assert (x.newOwner("Plzen", "Evropska", "Anton Hrabis"));
    assert (x.newOwner("Librec", 4552, "Cvut"));
    assert (x.getOwner("Prague", "Thakurova", owner) && owner == "CVUT");
    assert (x.getOwner("Dejvice", 12345, owner) && owner == "CVUT");
    assert (x.getOwner("Prague", "Evropska", owner) && owner == "");
    assert (x.getOwner("Vokovice", 12345, owner) && owner == "");
    assert (x.getOwner("Prague", "Technicka", owner) && owner == "CVUT");
    assert (x.getOwner("Dejvice", 9873, owner) && owner == "CVUT");
    assert (x.getOwner("Plzen", "Evropska", owner) && owner == "Anton Hrabis");
    assert (x.getOwner("Plzen mesto", 78901, owner) && owner == "Anton Hrabis");
    assert (x.getOwner("Liberec", "Evropska", owner) && owner == "Cvut");
    assert (x.getOwner("Librec", 4552, owner) && owner == "Cvut");
    CIterator i3 = x.listByAddr();
    assert (!i3.atEnd()
            && i3.city() == "Liberec"
            && i3.addr() == "Evropska"
            && i3.region() == "Librec"
            && i3.id() == 4552
            && i3.owner() == "Cvut");
    i3.next();
    assert (!i3.atEnd()
            && i3.city() == "Plzen"
            && i3.addr() == "Evropska"
            && i3.region() == "Plzen mesto"
            && i3.id() == 78901
            && i3.owner() == "Anton Hrabis");
    i3.next();
    assert (!i3.atEnd()
            && i3.city() == "Prague"
            && i3.addr() == "Evropska"
            && i3.region() == "Vokovice"
            && i3.id() == 12345
            && i3.owner() == "");
    i3.next();
    assert (!i3.atEnd()
            && i3.city() == "Prague"
            && i3.addr() == "Technicka"
            && i3.region() == "Dejvice"
            && i3.id() == 9873
            && i3.owner() == "CVUT");
    i3.next();
    assert (!i3.atEnd()
            && i3.city() == "Prague"
            && i3.addr() == "Thakurova"
            && i3.region() == "Dejvice"
            && i3.id() == 12345
            && i3.owner() == "CVUT");
    i3.next();
    assert (i3.atEnd());

    assert (x.count("cvut") == 3);
    CIterator i4 = x.listByOwner("cVuT");
    assert (!i4.atEnd()
            && i4.city() == "Prague"
            && i4.addr() == "Thakurova"
            && i4.region() == "Dejvice"
            && i4.id() == 12345
            && i4.owner() == "CVUT");
    i4.next();
    assert (!i4.atEnd()
            && i4.city() == "Prague"
            && i4.addr() == "Technicka"
            && i4.region() == "Dejvice"
            && i4.id() == 9873
            && i4.owner() == "CVUT");
    i4.next();
    assert (!i4.atEnd()
            && i4.city() == "Liberec"
            && i4.addr() == "Evropska"
            && i4.region() == "Librec"
            && i4.id() == 4552
            && i4.owner() == "Cvut");
    i4.next();
    assert (i4.atEnd());

    assert (x.newOwner("Plzen mesto", 78901, "CVut"));
    assert (x.count("CVUT") == 4);
    CIterator i5 = x.listByOwner("CVUT");
    assert (!i5.atEnd()
            && i5.city() == "Prague"
            && i5.addr() == "Thakurova"
            && i5.region() == "Dejvice"
            && i5.id() == 12345
            && i5.owner() == "CVUT");
    i5.next();
    assert (!i5.atEnd()
            && i5.city() == "Prague"
            && i5.addr() == "Technicka"
            && i5.region() == "Dejvice"
            && i5.id() == 9873
            && i5.owner() == "CVUT");
    i5.next();
    assert (!i5.atEnd()
            && i5.city() == "Liberec"
            && i5.addr() == "Evropska"
            && i5.region() == "Librec"
            && i5.id() == 4552
            && i5.owner() == "Cvut");
    i5.next();
    assert (!i5.atEnd()
            && i5.city() == "Plzen"
            && i5.addr() == "Evropska"
            && i5.region() == "Plzen mesto"
            && i5.id() == 78901
            && i5.owner() == "CVut");
    i5.next();
    assert (i5.atEnd());

    assert (x.del("Liberec", "Evropska"));
    assert (x.del("Plzen mesto", 78901));
    assert (x.count("cvut") == 2);
    CIterator i6 = x.listByOwner("cVuT");
    assert (!i6.atEnd()
            && i6.city() == "Prague"
            && i6.addr() == "Thakurova"
            && i6.region() == "Dejvice"
            && i6.id() == 12345
            && i6.owner() == "CVUT");
    i6.next();
    assert (!i6.atEnd()
            && i6.city() == "Prague"
            && i6.addr() == "Technicka"
            && i6.region() == "Dejvice"
            && i6.id() == 9873
            && i6.owner() == "CVUT");
    i6.next();
    assert (i6.atEnd());

    assert (x.add("Liberec", "Evropska", "Librec", 4552));
}

static void test1() {
    CLandRegister x;
    std::string owner;

    assert (x.add("Prague", "Thakurova", "Dejvice", 12345));
    assert (x.add("Prague", "Evropska", "Vokovice", 12345));
    assert (x.add("Prague", "Technicka", "Dejvice", 9873));
    assert (!x.add("Prague", "Technicka", "Hradcany", 7344));
    assert (!x.add("Brno", "Bozetechova", "Dejvice", 9873));
    assert (!x.getOwner("Prague", "THAKUROVA", owner));
    assert (!x.getOwner("Hradcany", 7343, owner));
    CIterator i0 = x.listByAddr();
    assert (!i0.atEnd()
            && i0.city() == "Prague"
            && i0.addr() == "Evropska"
            && i0.region() == "Vokovice"
            && i0.id() == 12345
            && i0.owner() == "");
    i0.next();
    assert (!i0.atEnd()
            && i0.city() == "Prague"
            && i0.addr() == "Technicka"
            && i0.region() == "Dejvice"
            && i0.id() == 9873
            && i0.owner() == "");
    i0.next();
    assert (!i0.atEnd()
            && i0.city() == "Prague"
            && i0.addr() == "Thakurova"
            && i0.region() == "Dejvice"
            && i0.id() == 12345
            && i0.owner() == "");
    i0.next();
    assert (i0.atEnd());

    assert (x.newOwner("Prague", "Thakurova", "CVUT"));
    assert (!x.newOwner("Prague", "technicka", "CVUT"));
    assert (!x.newOwner("prague", "Technicka", "CVUT"));
    assert (!x.newOwner("dejvice", 9873, "CVUT"));
    assert (!x.newOwner("Dejvice", 9973, "CVUT"));
    assert (!x.newOwner("Dejvice", 12345, "CVUT"));
    assert (x.count("CVUT") == 1);
    CIterator i1 = x.listByOwner("CVUT");
    assert (!i1.atEnd()
            && i1.city() == "Prague"
            && i1.addr() == "Thakurova"
            && i1.region() == "Dejvice"
            && i1.id() == 12345
            && i1.owner() == "CVUT");
    i1.next();
    assert (i1.atEnd());

    assert (!x.del("Brno", "Technicka"));
    assert (!x.del("Karlin", 9873));
    assert (x.del("Prague", "Technicka"));
    assert (!x.del("Prague", "Technicka"));
    assert (!x.del("Dejvice", 9873));
}

int main(void) {
    test0();
    test1();
    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
