#ifndef __PROGTEST__

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <vector>
#include <set>
#include <list>
#include <map>
#include <array>
#include <deque>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <compare>
#include <algorithm>
#include <cassert>
#include <memory>
#include <iterator>
#include <functional>
#include <stdexcept>

using namespace std::literals;

class CDummy {
public:
    CDummy(char c)
            : m_C(c) {
    }

    bool operator==(const CDummy &other) const = default;

    friend std::ostream &operator<<(std::ostream &os,
                                    const CDummy &x) {
        return os << '\'' << x.m_C << '\'';
    }

private:
    char m_C;
};

#endif /* __PROGTEST__ */

#define TEST_EXTRA_INTERFACE

using namespace std;

template<typename T_>
class WeakSubstring {
public:
    WeakSubstring(const vector<T_> &string, size_t pos, size_t count) : m_data(string), m_pos(pos), m_count(count) {}


    friend bool operator==(const WeakSubstring &lhs, const WeakSubstring &rhs) {
        if (lhs.size() != rhs.size()) {
            return false;
        }
        for (auto lhs_beg = lhs.m_data.cbegin() + lhs.m_pos, rhs_beg = rhs.m_data.cbegin() + rhs.m_pos, end =
                lhs_beg + lhs.size(); lhs_beg < end; lhs_beg++, rhs_beg++) {
            if (*lhs_beg != *rhs_beg) {
                return false;
            }
        }
        return true;
    }

    size_t size() const {
        return m_count;
    }

private:

    const vector<T_> &m_data;

    size_t m_pos;
    size_t m_count;
};

template<typename T_>
class WeakSubstringFactory {
public:
    explicit WeakSubstringFactory(const vector<T_> &string) : m_data(string) {

    }

    WeakSubstring<T_> createSubstring(size_t pos, size_t count) {
        return WeakSubstring<T_>(m_data, pos, count);
    }

private:
    const vector<T_> &m_data;
};

template<typename T_>
class CSelfMatch {
public:
    CSelfMatch(initializer_list<T_> init) : m_data(init) {}

    template<typename Container>
    CSelfMatch(const Container &container) : m_data(container.begin(), container.end()) {}

    template<typename Iterator>
    CSelfMatch(Iterator begin, Iterator end) : m_data(begin, end) {}

    void push_back(const T_ &element) {
        m_data.push_back(element);
    }

    template<typename... Args>
    void push_back(Args &&...args) {
        (m_data.push_back(forward<Args>(args)), ...);
    }

    size_t sequenceLen(uint n) const {

        if (n == 1) {
            return m_data.size();
        }

        WeakSubstringFactory<T_> weakSubstringFactory(m_data);

        for (size_t seq_size = m_data.size() - n + 1; seq_size > 0; seq_size--) {
            list<pair<WeakSubstring<T_>, size_t>> sequences_counts;
            for (size_t pos = 0; pos < m_data.size() - seq_size + 1; pos++) {
                auto weak_string = weakSubstringFactory.createSubstring(pos, seq_size);
                auto element = find_if(sequences_counts.begin(), sequences_counts.end(),
                                       [&weak_string](const pair<WeakSubstring<T_>, size_t> &el) {
                                           return el.first == weak_string;
                                       });
                if (element == sequences_counts.end()) {
                    sequences_counts.emplace_back(weak_string, 1);
                } else {
                    element->second++;
                    if (element->second >= n) {
                        return seq_size;
                    }
                }
            }
        }
        return 0;
    }

    template<size_t N_>
    vector<array<size_t, N_>> findSequences() const {
        WeakSubstringFactory<T_> weakSubstringFactory(m_data);

        for (size_t seq_size = m_data.size() - N_ + 1; seq_size > 0; seq_size--) {

            list<shared_ptr<pair<WeakSubstring<T_>, vector<size_t>>>> sequences_positions;
            list<shared_ptr<pair<WeakSubstring<T_>, vector<size_t>>>> selected;
            for (size_t pos = 0; pos < m_data.size() - seq_size + 1; pos++) {
                auto weak_string = weakSubstringFactory.createSubstring(pos, seq_size);
                auto element = find_if(sequences_positions.begin(), sequences_positions.end(),
                                       [&weak_string](const shared_ptr<pair<WeakSubstring<T_>, vector<size_t>>> &el) {
                                           return el->first == weak_string;
                                       });
                if (element == sequences_positions.end()) {
                    auto new_element = shared_ptr<pair<WeakSubstring<T_>, vector<size_t>>>(
                            new pair<WeakSubstring<T_>, vector<size_t>>(weak_string, {pos}));
                    sequences_positions.push_back(new_element);
                    if (new_element->second.size() == N_) {
                        selected.push_back(new_element);
                    }
                } else {
                    (*element)->second.push_back(pos);
                    if ((*element)->second.size() == N_) {
                        selected.push_back(*element);
                    }
                }

            }
            if (!selected.empty()) {
                vector<array<size_t, N_>> combinations;
                for (const auto &element: selected) {
                    const vector<size_t> &positions = element->second;
                    auto element_combinations = getCombinations<N_>(positions, {}, 0, N_);
                    for (const auto &combination: element_combinations) {
                        combinations.push_back(combination);
                    }
                }
                return combinations;
            }
        }
        return {};
    }

private:

    template<size_t N_>
    static vector<array<size_t, N_>>
    getCombinations(const vector<size_t> &positions, array<size_t, N_> current, size_t start, size_t remains) {
        if (remains == 0) {
            return {current};
        }
        vector<array<size_t, N_>> combinations;
        auto end = positions.size() - remains;
        while (start <= end) {
            current[N_ - remains] = positions[start];
            auto sub_combinations = getCombinations(positions, current, start + 1, remains - 1);
            for (const auto &combination: sub_combinations) {
                combinations.push_back(combination);
            }
            start++;
        }
        return combinations;
    }

    vector<T_> m_data;
};

template<typename Container>
CSelfMatch(const Container &container) -> CSelfMatch<typename Container::value_type>;

template<typename Iterator>
CSelfMatch(Iterator begin, Iterator end) -> CSelfMatch<typename Iterator::value_type>;


#ifndef __PROGTEST__

template<size_t N_>
bool positionMatch(std::vector<std::array<size_t, N_>> a,
                   std::vector<std::array<size_t, N_>> b) {
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    return a == b;
}

int main() {
    CSelfMatch<char> x0("aaaaaaaaaaa"s);
//    vector<int> ints = {1, 2, 3, 4, 5};
//    WeakSubstring<int> s1(ints, 0, 2);
//    WeakSubstring<int> s2(ints, 0, 2);
//    assert(!("12"s < "12"s));
//    assert(!(s1 < s2));
//    WeakSubstring<int> s3(ints, 1, 2);
//    assert("12"s < "23"s);
//    assert(s1 < s3);

    assert (x0.sequenceLen(2) == 10);
    auto result = x0.findSequences<2>();
    assert (positionMatch(x0.findSequences<2>(), std::vector<std::array<size_t, 2> >{{0, 1}}));
    CSelfMatch<char> x1("abababababa"s);
    assert (x1.sequenceLen(2) == 9);
    assert (positionMatch(x1.findSequences<2>(), std::vector<std::array<size_t, 2> >{{0, 2}}));
    CSelfMatch<char> x2("abababababab"s);
    assert (x2.sequenceLen(2) == 10);
    assert (positionMatch(x2.findSequences<2>(), std::vector<std::array<size_t, 2> >{{0, 2}}));
    CSelfMatch<char> x3("aaaaaaaaaaa"s);
    assert (x3.sequenceLen(3) == 9);
    assert (positionMatch(x3.findSequences<3>(), std::vector<std::array<size_t, 3> >{{0, 1, 2}}));
    CSelfMatch<char> x4("abababababa"s);
    assert (x4.sequenceLen(3) == 7);
    assert (positionMatch(x4.findSequences<3>(), std::vector<std::array<size_t, 3> >{{0, 2, 4}}));
    CSelfMatch<char> x5("abababababab"s);
    assert (x5.sequenceLen(3) == 8);
    assert (positionMatch(x5.findSequences<3>(), std::vector<std::array<size_t, 3> >{{0, 2, 4}}));
    CSelfMatch<char> x6("abcdXabcd"s);
    assert (x6.sequenceLen(1) == 9);
    assert (positionMatch(x6.findSequences<1>(), std::vector<std::array<size_t, 1> >{{0}}));
    CSelfMatch<char> x7("abcdXabcd"s);
    assert (x7.sequenceLen(2) == 4);
    assert (positionMatch(x7.findSequences<2>(), std::vector<std::array<size_t, 2> >{{0, 5}}));
    CSelfMatch<char> x8("abcdXabcdeYabcdZabcd"s);
    assert (x8.sequenceLen(2) == 4);
    result = x8.findSequences<2>();
    assert (positionMatch(x8.findSequences<2>(), std::vector<std::array<size_t, 2> >{{0,  5},
                                                                                     {0,  11},
                                                                                     {0,  16},
                                                                                     {5,  11},
                                                                                     {5,  16},
                                                                                     {11, 16}}));
    CSelfMatch<char> x9("abcdXabcdYabcd"s);
    assert (x9.sequenceLen(3) == 4);
    assert (positionMatch(x9.findSequences<3>(), std::vector<std::array<size_t, 3> >{{0, 5, 10}}));
    CSelfMatch<char> x10("abcdefghijklmn"s);
    assert (x10.sequenceLen(2) == 0);
    assert (positionMatch(x10.findSequences<2>(), std::vector<std::array<size_t, 2> >{}));
    CSelfMatch<char> x11("abcXabcYabcZdefXdef"s);
    assert (x11.sequenceLen(2) == 3);
    assert (positionMatch(x11.findSequences<2>(), std::vector<std::array<size_t, 2> >{{0,  4},
                                                                                      {0,  8},
                                                                                      {4,  8},
                                                                                      {12, 16}}));
    CSelfMatch<int> x12{1, 2, 3, 1, 2, 4, 1, 2};
    assert (x12.sequenceLen(2) == 2);
    assert (positionMatch(x12.findSequences<2>(), std::vector<std::array<size_t, 2> >{{0, 3},
                                                                                      {0, 6},
                                                                                      {3, 6}}));
    assert (x12.sequenceLen(3) == 2);
    assert (positionMatch(x12.findSequences<3>(), std::vector<std::array<size_t, 3> >{{0, 3, 6}}));
    std::initializer_list<CDummy> init13{'a', 'b', 'c', 'd', 'X', 'a', 'b', 'c', 'd', 'Y', 'a', 'b', 'c', 'd'};
    CSelfMatch<CDummy> x13(init13.begin(), init13.end());
    assert (x13.sequenceLen(2) == 4);
    assert (positionMatch(x13.findSequences<2>(), std::vector<std::array<size_t, 2> >{{0, 5},
                                                                                      {0, 10},
                                                                                      {5, 10}}));
    std::initializer_list<int> init14{1, 2, 1, 1, 2, 1, 0, 0, 1, 2, 1, 0, 1, 2, 0, 1, 2, 0, 1, 1, 1, 2, 0, 2, 0, 1, 2,
                                      1, 0};
    CSelfMatch<int> x14(init14.begin(), init14.end());
    assert (x14.sequenceLen(2) == 5);
    assert (positionMatch(x14.findSequences<2>(), std::vector<std::array<size_t, 2> >{{11, 14},
                                                                                      {7,  24}}));
    std::initializer_list<int> init15{1, 2, 1, 1, 2, 1, 0, 0, 1, 2, 1, 0, 1, 2, 0, 1, 2, 0, 1, 1, 1, 2, 0, 2, 0, 1, 2,
                                      1, 0};
    CSelfMatch<int> x15(init15.begin(), init15.end());
    assert (x15.sequenceLen(3) == 4);
    assert (positionMatch(x15.findSequences<3>(), std::vector<std::array<size_t, 3> >{{3, 8, 25}}));
#ifdef TEST_EXTRA_INTERFACE
    CSelfMatch y0("aaaaaaaaaaa"s);
    assert (y0.sequenceLen(2) == 10);

    std::string s1("abcd");
    CSelfMatch y1(s1.begin(), s1.end());
    assert (y1.sequenceLen(2) == 0);

    CSelfMatch y2(""s);
    y2.push_back('a', 'b', 'c', 'X');
    y2.push_back('a');
    y2.push_back('b', 'c');
    assert (y2.sequenceLen(2) == 3);
#endif /* TEST_EXTRA_INTERFACE */
    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
