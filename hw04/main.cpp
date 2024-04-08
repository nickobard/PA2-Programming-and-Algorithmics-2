#ifndef __PROGTEST__

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <climits>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <array>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <functional>
#include <iterator>
#include <compare>

class CDate {
public:
    CDate(int y,
          int m,
          int d)
            : m_Y(y),
              m_M(m),
              m_D(d) {
    }

    std::strong_ordering operator<=>(const CDate &other) const = default;

    friend std::ostream &operator<<(std::ostream &os,
                                    const CDate &d) {
        return os << d.m_Y << '-' << d.m_M << '-' << d.m_D;
    }

private:
    int m_Y;
    int m_M;
    int m_D;
};

enum class ESortKey {
    NAME,
    BIRTH_DATE,
    ENROLL_YEAR
};
#endif /* __PROGTEST__ */

using namespace std;

string lower_case(const string &str) {
    string lower_cased;
    for (char c: str) {
        lower_cased.push_back((char) tolower(c));
    }
    return lower_cased;
}

multiset<string> multiset_split(const string &str) {
    multiset<string> m;
    istringstream iss(str);
    string word;
    while (iss >> word) {
        m.insert(word);
    }
    return m;
}

class CStudent {
public:
    CStudent(const std::string &name,
             const CDate &born,
             int enrolled) : m_id(0),
                             m_name(name), m_date(born), m_enrolled(enrolled) {}

    CStudent(const std::string &name,
             const CDate &born,
             int enrolled, long long id) : m_id(id),
                                           m_name(name), m_date(born), m_enrolled(enrolled), m_filter_name(
                    multiset_split(lower_case(name))) {}


    bool operator==(const CStudent &other) const {
        return m_name == other.m_name && m_date == other.m_date && m_enrolled == other.m_enrolled;
    }

    bool operator!=(const CStudent &other) const {
        return !(*this == other);
    }

    bool operator<(const CStudent &other) const {
        return tie(m_name, m_date, m_enrolled) < tie(other.m_name, other.m_date, other.m_enrolled);
    }

    struct ComparatorByID {
        bool operator()(const CStudent *lhs, const CStudent *rhs) const {
            return lhs->m_id < rhs->m_id;
        }
    };

    struct ComparatorByDate {
        bool operator()(const CStudent *lhs, const CStudent *rhs) const {
            return tie(lhs->m_date, lhs->m_enrolled, lhs->m_name) < tie(rhs->m_date, rhs->m_enrolled, rhs->m_name);
        }
    };

    struct ComparatorByEnrolled {
        bool operator()(const CStudent *lhs, const CStudent *rhs) const {
            return tie(lhs->m_enrolled, lhs->m_name, lhs->m_date) < tie(rhs->m_enrolled, rhs->m_name, rhs->m_date);
        }
    };

    struct ComparatorByFilterName {
        bool operator()(const CStudent *lhs, const CStudent *rhs) const {
            return tie(lhs->m_filter_name, lhs->m_name, lhs->m_date, lhs->m_enrolled) <
                   tie(rhs->m_filter_name, rhs->m_name, rhs->m_date, rhs->m_enrolled);
        }
    };

    struct DefaultComparator {
        bool operator()(const CStudent *lhs, const CStudent *rhs) const {
            return *lhs < *rhs;
        }
    };

    long long m_id;
    string m_name;
    CDate m_date;
    int m_enrolled;
    multiset<string> m_filter_name;
};

class CFilter {
public:
    CFilter() = default;

    CFilter &name(const std::string &name) {
        m_names_to_filter.push_back(multiset_split(lower_case(name)));
        return *this;
    }

    CFilter &bornBefore(const CDate &date) {
        if (m_born_before > date) {
            m_born_before = date;
        }
        return *this;
    }

    CFilter &bornAfter(const CDate &date) {
        if (m_born_after < date) {
            m_born_after = date;
        }
        return *this;
    }

    CFilter &enrolledBefore(int year) {
        if (m_enrolled_before > year) {
            m_enrolled_before = year;
        }
        return *this;
    }

    CFilter &enrolledAfter(int year) {
        if (m_enrolled_after < year) {
            m_enrolled_after = year;
        }
        return *this;
    }


private:

    CDate m_born_before = CDate(INT32_MAX, INT32_MAX, INT32_MAX);
    CDate m_born_after = CDate(0, 0, 0);
    int m_enrolled_before = INT32_MAX;
    int m_enrolled_after = 0;
    list<multiset<string>> m_names_to_filter;
};

class CSort {
public:
    CSort() = default;

    bool operator()(const CStudent &lhs, const CStudent &rhs) const {
        auto current_key = m_keys.cbegin();
        if (current_key == m_keys.end()) {
            return compare_by_id(lhs, rhs);
        }
        return compare(lhs, rhs, current_key);
    }

    bool
    compare(const CStudent &lhs, const CStudent &rhs, list<pair<ESortKey, bool>>::const_iterator current_key) const {
        auto [key, ascending] = *current_key;
        bool left_term_result;
        if (ascending) {
            if (key == ESortKey::NAME) {
                left_term_result = lhs.m_name < rhs.m_name;
            } else if (key == ESortKey::BIRTH_DATE) {
                left_term_result = lhs.m_date < rhs.m_date;
            } else {
                left_term_result = lhs.m_enrolled < rhs.m_enrolled;
            }
        } else {
            if (key == ESortKey::NAME) {
                left_term_result = lhs.m_name > rhs.m_name;
            } else if (key == ESortKey::BIRTH_DATE) {
                left_term_result = lhs.m_date > rhs.m_date;
            } else {
                left_term_result = lhs.m_enrolled > rhs.m_enrolled;
            }
        }
        if (left_term_result) {
            return true;
        }


        bool equal;
        if (key == ESortKey::NAME) {
            equal = lhs.m_name == rhs.m_name;
        } else if (key == ESortKey::BIRTH_DATE) {
            equal = lhs.m_date == rhs.m_date;
        } else {
            equal = lhs.m_enrolled == rhs.m_enrolled;
        }

        if (!equal) {
            return false;
        }

        current_key++;
        if (current_key == m_keys.cend()) {
            return compare_by_id(lhs, rhs);
        }
        return compare(lhs, rhs, current_key);
    }

    static bool
    compare_by_id(const CStudent &lhs, const CStudent &rhs) {
        return lhs.m_id < rhs.m_id;
    }

    CSort &addKey(ESortKey key,
                  bool ascending) {
        m_keys.emplace_back(key, ascending);
        return *this;
    }

private:
    list<pair<ESortKey, bool>> m_keys;
};

class CStudyDept {
public:
    CStudyDept() = default;

    ~CStudyDept() {
        for (auto *student: m_students_by_date) {
            delete student;
        }
    }

    bool addStudent(const CStudent &x) {
        auto *to_find = &(const_cast<CStudent &>(x));
        auto iter = m_students_by_date.find(to_find);
        if (iter != m_students_by_date.end()) {
            return false;
        }
        auto *to_insert = new CStudent(x.m_name, x.m_date, x.m_enrolled, student_id_counter++);
        m_students_by_id.insert(to_insert);
        m_students_by_date.insert(to_insert);
        m_students_by_enrolled.insert(to_insert);
        m_students_by_filter_name.insert((to_insert));
        return true;
    }

    bool delStudent(const CStudent &x) {
        auto *to_find = &(const_cast<CStudent &>(x));
        auto iter = m_students_by_date.find(to_find);
        if (iter == m_students_by_date.end()) {
            // not found
            return false;
        }
        auto to_delete = *iter;
        m_students_by_id.erase(to_delete);
        m_students_by_date.erase(to_delete);
        m_students_by_enrolled.erase(to_delete);
        m_students_by_filter_name.erase(to_delete);
        delete to_delete;
        return true;
    }

    std::list<CStudent> search(const CFilter &flt,
                               const CSort &sortOpt) const {
        list<CStudent> l;
        for (auto *student: m_students_by_id) {
            l.emplace_back(student->m_name, student->m_date, student->m_enrolled);
        }
        l.sort(sortOpt);
        return l;
    }

    std::set<std::string> suggest(const std::string &name) const;

private:
    static long long student_id_counter;
    set<CStudent *, CStudent::ComparatorByID> m_students_by_id;
    set<CStudent *, CStudent::ComparatorByDate> m_students_by_date;
    set<CStudent *, CStudent::ComparatorByEnrolled> m_students_by_enrolled;
    set<CStudent *, CStudent::ComparatorByFilterName> m_students_by_filter_name;
};

long long CStudyDept::student_id_counter = 0;

#ifndef __PROGTEST__

int main(void) {
    CStudyDept x0;
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) == CStudent("James Bond", CDate(1980, 4, 11), 2010));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("James Bond", CDate(1980, 4, 11), 2010)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("Peter Peterson", CDate(1980, 4, 11), 2010));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) ==
              CStudent("Peter Peterson", CDate(1980, 4, 11), 2010)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("James Bond", CDate(1997, 6, 17), 2010));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) == CStudent("James Bond", CDate(1997, 6, 17), 2010)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("James Bond", CDate(1980, 4, 11), 2016));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) == CStudent("James Bond", CDate(1980, 4, 11), 2016)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("Peter Peterson", CDate(1980, 4, 11), 2016));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) ==
              CStudent("Peter Peterson", CDate(1980, 4, 11), 2016)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("Peter Peterson", CDate(1997, 6, 17), 2010));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) ==
              CStudent("Peter Peterson", CDate(1997, 6, 17), 2010)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("James Bond", CDate(1997, 6, 17), 2016));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) == CStudent("James Bond", CDate(1997, 6, 17), 2016)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("Peter Peterson", CDate(1997, 6, 17), 2016));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) ==
              CStudent("Peter Peterson", CDate(1997, 6, 17), 2016)));
    assert (x0.addStudent(CStudent("John Peter Taylor", CDate(1983, 7, 13), 2014)));
    assert (x0.addStudent(CStudent("John Taylor", CDate(1981, 6, 30), 2012)));
    assert (x0.addStudent(CStudent("Peter Taylor", CDate(1982, 2, 23), 2011)));
    assert (x0.addStudent(CStudent("Peter John Taylor", CDate(1984, 1, 17), 2017)));
    assert (x0.addStudent(CStudent("James Bond", CDate(1981, 7, 16), 2013)));
    assert (x0.addStudent(CStudent("James Bond", CDate(1982, 7, 16), 2013)));
    assert (x0.addStudent(CStudent("James Bond", CDate(1981, 8, 16), 2013)));
    assert (x0.addStudent(CStudent("James Bond", CDate(1981, 7, 17), 2013)));
    assert (x0.addStudent(CStudent("James Bond", CDate(1981, 7, 16), 2012)));
    assert (x0.addStudent(CStudent("Bond James", CDate(1981, 7, 16), 2013)));


    auto res = x0.search(CFilter(), CSort());

    assert (x0.search(CFilter(), CSort()) == (std::list<CStudent>
            {
                    CStudent("John Peter Taylor", CDate(1983, 7, 13), 2014),
                    CStudent("John Taylor", CDate(1981, 6, 30), 2012),
                    CStudent("Peter Taylor", CDate(1982, 2, 23), 2011),
                    CStudent("Peter John Taylor", CDate(1984, 1, 17), 2017),
                    CStudent("James Bond", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1982, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 8, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 17), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2012),
                    CStudent("Bond James", CDate(1981, 7, 16), 2013)
            }));
    assert (x0.search(CFilter(), CSort().addKey(ESortKey::NAME, true)) == (std::list<CStudent>
            {
                    CStudent("Bond James", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1982, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 8, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 17), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2012),
                    CStudent("John Peter Taylor", CDate(1983, 7, 13), 2014),
                    CStudent("John Taylor", CDate(1981, 6, 30), 2012),
                    CStudent("Peter John Taylor", CDate(1984, 1, 17), 2017),
                    CStudent("Peter Taylor", CDate(1982, 2, 23), 2011)
            }));
    assert (x0.search(CFilter(), CSort().addKey(ESortKey::NAME, false)) == (std::list<CStudent>
            {
                    CStudent("Peter Taylor", CDate(1982, 2, 23), 2011),
                    CStudent("Peter John Taylor", CDate(1984, 1, 17), 2017),
                    CStudent("John Taylor", CDate(1981, 6, 30), 2012),
                    CStudent("John Peter Taylor", CDate(1983, 7, 13), 2014),
                    CStudent("James Bond", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1982, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 8, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 17), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2012),
                    CStudent("Bond James", CDate(1981, 7, 16), 2013)
            }));
    assert (x0.search(CFilter(),
                      CSort().addKey(ESortKey::ENROLL_YEAR, false).addKey(ESortKey::BIRTH_DATE, false).addKey(
                              ESortKey::NAME, true)) == (std::list<CStudent>
            {
                    CStudent("Peter John Taylor", CDate(1984, 1, 17), 2017),
                    CStudent("John Peter Taylor", CDate(1983, 7, 13), 2014),
                    CStudent("James Bond", CDate(1982, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 8, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 17), 2013),
                    CStudent("Bond James", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2012),
                    CStudent("John Taylor", CDate(1981, 6, 30), 2012),
                    CStudent("Peter Taylor", CDate(1982, 2, 23), 2011)
            }));
    multiset<string> m1 = {"john", "ryba", "ryba"};
    multiset<string> m2 = {"ryba", "john", "ryba"};
    cout << (m1 == m2) << endl;

//    assert (x0.search(CFilter().name("james bond"),
//                      CSort().addKey(ESortKey::ENROLL_YEAR, false).addKey(ESortKey::BIRTH_DATE, false).addKey(
//                              ESortKey::NAME, true)) == (std::list<CStudent>
//            {
//                    CStudent("James Bond", CDate(1982, 7, 16), 2013),
//                    CStudent("James Bond", CDate(1981, 8, 16), 2013),
//                    CStudent("James Bond", CDate(1981, 7, 17), 2013),
//                    CStudent("Bond James", CDate(1981, 7, 16), 2013),
//                    CStudent("James Bond", CDate(1981, 7, 16), 2013),
//                    CStudent("James Bond", CDate(1981, 7, 16), 2012)
//            }));
//    assert (x0.search(CFilter().bornAfter(CDate(1980, 4, 11)).bornBefore(CDate(1983, 7, 13)).name("John Taylor").name(
//            "james BOND"), CSort().addKey(ESortKey::ENROLL_YEAR, false).addKey(ESortKey::BIRTH_DATE, false).addKey(
//            ESortKey::NAME, true)) == (std::list<CStudent>
//            {
//                    CStudent("James Bond", CDate(1982, 7, 16), 2013),
//                    CStudent("James Bond", CDate(1981, 8, 16), 2013),
//                    CStudent("James Bond", CDate(1981, 7, 17), 2013),
//                    CStudent("Bond James", CDate(1981, 7, 16), 2013),
//                    CStudent("James Bond", CDate(1981, 7, 16), 2013),
//                    CStudent("James Bond", CDate(1981, 7, 16), 2012),
//                    CStudent("John Taylor", CDate(1981, 6, 30), 2012)
//            }));
//    assert (x0.search(CFilter().name("james"), CSort().addKey(ESortKey::NAME, true)) == (std::list<CStudent>
//            {
//            }));
//    assert (x0.suggest("peter") == (std::set<std::string>
//            {
//                    "John Peter Taylor",
//                    "Peter John Taylor",
//                    "Peter Taylor"
//            }));
//    assert (x0.suggest("bond") == (std::set<std::string>
//            {
//                    "Bond James",
//                    "James Bond"
//            }));
//    assert (x0.suggest("peter joHn") == (std::set<std::string>
//            {
//                    "John Peter Taylor",
//                    "Peter John Taylor"
//            }));
//    assert (x0.suggest("peter joHn bond") == (std::set<std::string>
//            {
//            }));
//    assert (x0.suggest("pete") == (std::set<std::string>
//            {
//            }));
//    assert (x0.suggest("peter joHn PETER") == (std::set<std::string>
//            {
//                    "John Peter Taylor",
//                    "Peter John Taylor"
//            }));
//    assert (!x0.addStudent(CStudent("James Bond", CDate(1981, 7, 16), 2013)));
//    assert (x0.delStudent(CStudent("James Bond", CDate(1981, 7, 16), 2013)));
//    assert (x0.search(CFilter().bornAfter(CDate(1980, 4, 11)).bornBefore(CDate(1983, 7, 13)).name("John Taylor").name(
//            "james BOND"), CSort().addKey(ESortKey::ENROLL_YEAR, false).addKey(ESortKey::BIRTH_DATE, false).addKey(
//            ESortKey::NAME, true)) == (std::list<CStudent>
//            {
//                    CStudent("James Bond", CDate(1982, 7, 16), 2013),
//                    CStudent("James Bond", CDate(1981, 8, 16), 2013),
//                    CStudent("James Bond", CDate(1981, 7, 17), 2013),
//                    CStudent("Bond James", CDate(1981, 7, 16), 2013),
//                    CStudent("James Bond", CDate(1981, 7, 16), 2012),
//                    CStudent("John Taylor", CDate(1981, 6, 30), 2012)
//            }));
//    assert (!x0.delStudent(CStudent("James Bond", CDate(1981, 7, 16), 2013)));
    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
