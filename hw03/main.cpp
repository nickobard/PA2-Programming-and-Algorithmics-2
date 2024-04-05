#ifndef __PROGTEST__

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <memory>
#include <stdexcept>

#endif /* __PROGTEST__ */

using namespace std;

struct CPatch {
    CPatch() : m_next(nullptr) {

    }


    CPatch(const char *str) : m_next(nullptr) {
        size_t length = strlen(str);
        char *patch = new char[length + 1];
        strcpy(patch, str);
        m_patch = shared_ptr<char[]>(patch);
    }

    CPatch(shared_ptr<char[]> &patch) {
        m_patch = patch;
        m_next = nullptr;
    }

    ~CPatch() {
        delete m_next;
    }

    size_t size() const {
        return strlen(m_patch.get());
    }

    bool empty() const {
        return size() == 0;
    }

    CPatch *&next() {
        return m_next;
    }

    CPatch *m_next;
    shared_ptr<char[]> m_patch;

};


class CPatchStr {
public:
    CPatchStr() {
        m_head = new CPatch("");
        m_tail = m_head;
        m_size = 0;
    }

    CPatchStr(const char *str) {
        m_head = new CPatch(str);
        m_tail = m_head;
        m_size = m_head->size();
    }

    ~CPatchStr() {
        delete m_head;
    }

    // copy constructor
    // destructor 
    // operator =
    CPatchStr subStr(size_t from,
                     size_t len) const {
        return CPatchStr("");
    }

    CPatchStr &append(const CPatchStr &src) {
        // current CPatchStr is empty string
        CPatch **dst_next;
        if (empty()) {
            dst_next = &m_head;
        } else {
            dst_next = &(m_tail->next());
        }

        CPatch *const *src_current = &(src.m_head);

        while (*src_current != nullptr) {
            if ((*src_current)->empty()) { // it is pointless to copy empty string patch
                src_current = &((*src_current)->next());
                continue;
            }

            auto *to_append = new CPatch((*src_current)->m_patch);

            *dst_next = to_append;
            m_tail = to_append;
            dst_next = &(to_append->next());
            m_size += (*src_current)->size();
            src_current = &((*src_current)->next());

        }
        return *this;
    }

    CPatchStr &insert(size_t pos,
                      const CPatchStr &src) {
        return *this;
    }

    CPatchStr &remove(size_t from,
                      size_t len) {
        return *this;
    }

    char *toStr() const {
        auto *current = m_head;
        char *str = new char[m_size + 1];
        size_t current_pos = 0;
        while (current != nullptr) {
            const char *src = current->m_patch.get();
            size_t src_size = current->size();
            for (size_t i = 0; i < src_size; i++) {
                str[current_pos++] = src[i];
            }
            current = current->next();
        }
        str[current_pos] = 0;
        return str;
    }

private:

    bool empty() const {
        return m_size == 0;
    }

    CPatch *m_head;
    CPatch *m_tail;
    size_t m_size;
};


#ifndef __PROGTEST__

bool stringMatch(char *str,
                 const char *expected) {
    bool res = std::strcmp(str, expected) == 0;
    delete[] str;
    return res;
}

int main() {
    char tmpStr[100];

    CPatchStr a("test");
    assert (stringMatch(a.toStr(), "test"));
    std::strncpy(tmpStr, " da", sizeof(tmpStr) - 1);
    a.append(tmpStr);
    assert (stringMatch(a.toStr(), "test da"));
    std::strncpy(tmpStr, "ta", sizeof(tmpStr) - 1);
    a.append(tmpStr);
    assert (stringMatch(a.toStr(), "test data"));
    std::strncpy(tmpStr, "foo text", sizeof(tmpStr) - 1);
    CPatchStr b(tmpStr);
    assert (stringMatch(b.toStr(), "foo text"));
//    CPatchStr c(a);
//    assert (stringMatch(c.toStr(), "test data"));
//    CPatchStr d(a.subStr(3, 5));
//    assert (stringMatch(d.toStr(), "t dat"));
//    d.append(b);
//    assert (stringMatch(d.toStr(), "t datfoo text"));
//    d.append(b.subStr(3, 4));
//    assert (stringMatch(d.toStr(), "t datfoo text tex"));
//    c.append(d);
//    assert (stringMatch(c.toStr(), "test datat datfoo text tex"));
//    c.append(c);
//    assert (stringMatch(c.toStr(), "test datat datfoo text textest datat datfoo text tex"));
//    d.insert(2, c.subStr(6, 9));
//    assert (stringMatch(d.toStr(), "t atat datfdatfoo text tex"));
//    b = "abcdefgh";
//    assert (stringMatch(b.toStr(), "abcdefgh"));
//    assert (stringMatch(d.toStr(), "t atat datfdatfoo text tex"));
//    assert (stringMatch(d.subStr(4, 8).toStr(), "at datfd"));
//    assert (stringMatch(b.subStr(2, 6).toStr(), "cdefgh"));
//    try {
//        b.subStr(2, 7).toStr();
//        assert ("Exception not thrown" == nullptr);
//    }
//    catch (const std::out_of_range &e) {
//    }
//    catch (...) {
//        assert ("Invalid exception thrown" == nullptr);
//    }
//    a.remove(3, 5);
//    assert (stringMatch(a.toStr(), "tesa"));
    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
