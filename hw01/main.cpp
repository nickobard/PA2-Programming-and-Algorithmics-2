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
    Land() :
            m_id(0) {}

    Land(string region, unsigned int id, string city, string addr) :
            m_id(id),
            m_region(std::move(region)),
            m_city(std::move(city)),
            m_addr(std::move(addr)) {}

    friend bool operator==(const Land &lhs, const Land &rhs);

    unsigned int m_id;
    string m_region;
    string m_city;
    string m_addr;
    string m_owner;
    string m_owner_lower_case;
};

bool operator==(const Land &lhs, const Land &rhs) {
    return (lhs.m_region == rhs.m_region && lhs.m_id == rhs.m_id) ||
           (lhs.m_city == rhs.m_city && lhs.m_addr == rhs.m_addr);
}

class CIterator {
public:
    using iterator_dt = vector<Land>;

    CIterator(iterator_dt &lands) : m_lands(lands) {
        m_it = m_lands.begin();
    }

    bool atEnd() const {
        return m_it == m_lands.end();
    }

    void next() {
        m_it++;
    }

    string city() const {
        return m_it->m_city;
    }

    string addr() const {
        return m_it->m_addr;
    }

    string region() const {
        return m_it->m_region;
    }

    unsigned id() const {
        return m_it->m_id;
    }

    string owner() const {
        return m_it->m_owner;
    }

private:
    iterator_dt::iterator m_it;
    iterator_dt m_lands;
};

class CLandRegister {
public:
    bool add(const string &city,
             const string &addr,
             const string &region,
             unsigned int id) {

        if (landExists(city, addr, region, id)) {
            return false;
        }
        m_lands.emplace_back(region, id, city, addr);
        return true;
    }

    bool del(const string &city,
             const string &addr) {
        for (auto it = m_lands.begin(); it != m_lands.end(); it++) {
            if (it->m_city == city && it->m_addr == addr) {
                m_lands.erase(it);
                return true;
            }
        }
        return false;
    }

    bool del(const string &region,
             unsigned int id) {
        for (auto it = m_lands.begin(); it != m_lands.end(); it++) {
            if (it->m_region == region && it->m_id == id) {
                m_lands.erase(it);
                return true;
            }
        }
        return false;
    }

    bool getOwner(const string &city,
                  const string &addr,
                  string &owner) const {
        Land to_find;
        if (findByCityAndAddr(city, addr, to_find)) {
            owner = to_find.m_owner;
            return true;
        }
        return false;
    }

    bool getOwner(const string &region,
                  unsigned int id,
                  string &owner) const {
        Land to_find;
        if (findByRegionAndID(region, id, to_find)) {
            owner = to_find.m_owner;
            return true;
        }
        return false;
    }

    bool newOwner(const string &city,
                  const string &addr,
                  const string &owner) {
        Land to_find;
        if (findByCityAndAddr(city, addr, to_find)) {
            to_find.m_owner = owner;
            to_find.m_owner_lower_case = to_lower(owner);
            return true;
        }
        return false;
    }

    bool newOwner(const string &region,
                  unsigned int id,
                  const string &owner) {
        Land to_find;
        if (findByRegionAndID(region, id, to_find)) {
            to_find.m_owner = owner;
            to_find.m_owner_lower_case = to_lower(owner);
            return true;
        }
        return false;
    }

    size_t count(const string &owner) const {
        string lower_cased_owner = to_lower(owner);
        size_t count = 0;
        for (const Land &l: m_lands) {
            if (l.m_owner_lower_case == lower_cased_owner) {
                count++;
            }
        }
        return count;
    }

    CIterator listByAddr() const {
        vector<Land> by_address(m_lands.begin(), m_lands.end());
        sort(by_address.begin(), by_address.end(), [](const Land &lhs, const Land &rhs) {
            return tie(lhs.m_city, lhs.m_addr) < tie(rhs.m_city, rhs.m_addr);
        });
        return {by_address};
    }

    CIterator listByOwner(const string &owner) const {
        const string owner_lower = to_lower(owner);
        vector<Land> by_owner;
        for (const Land &l: m_lands) {
            if (l.m_owner_lower_case == owner_lower) {
                by_owner.push_back(l);
            }
        }
        return by_owner;
    }

private:

    static string to_lower(string str) {
        for (char &c: str) {
            c = (char) tolower(c);
        }
        return str;
    }

    bool findByRegionAndID(const string &region, unsigned int id, Land &land) {
        for (const Land &l: m_lands) {
            if (l.m_region == region && l.m_id == id) {
                land = l;
                return true;
            }
        }
        return false;
    }

    bool findByCityAndAddr(const string &city, const string &addr, Land &land) {
        for (const Land &l: m_lands) {
            if (l.m_city == city && l.m_addr == addr) {
                land = l;
                return true;
            }
        }
        return false;
    }

    bool findByRegionAndID(const string &region, unsigned int id, Land &land) const {
        for (const Land &l: m_lands) {
            if (l.m_region == region && l.m_id == id) {
                land = l;
                return true;
            }
        }
        return false;
    }

    bool findByCityAndAddr(const string &city, const string &addr, Land &land) const {
        for (const Land &l: m_lands) {
            if (l.m_city == city && l.m_addr == addr) {
                land = l;
                return true;
            }
        }
        return false;
    }

    bool landExists(const string &city,
                    const string &addr,
                    const string &region,
                    unsigned int id) {
        for (const Land &l: m_lands) {
            if (l.m_city == city && l.m_addr == city) {
                return true;
            }
            // or if
            if (l.m_region == region && l.m_id == id) {
                return true;
            }
        }
        return false;
    }

    list<Land> m_lands;
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
