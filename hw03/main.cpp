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

    CPatchStr(shared_ptr<char[]> str) {
        m_head = new CPatch(str);
        m_tail = m_head;
        m_size = m_head->size();
    }

    CPatchStr(const CPatchStr &p) {
        m_size = p.m_size;
        CPatch **current_dst = &m_head;
        CPatch *current_src = p.m_head;
        while (current_src != nullptr) {
            (*current_dst) = new CPatch(current_src->m_patch.get());
            m_tail = (*current_dst);
            current_src = current_src->next();
            current_dst = &((*current_dst)->next());
        }
    }

    ~CPatchStr() {
        delete m_head;
    }

    CPatchStr &operator=(CPatchStr src) {
        swap(m_size, src.m_size);
        swap(m_head, src.m_head);
        swap(m_tail, src.m_tail);
        return *this;
    }

    // operator =
    CPatchStr subStr(size_t from,
                     size_t len) const {
        if (from > m_size || from + len > m_size) {
            throw out_of_range("Cannot create substring - out of range limits.");
        }
        if (len == 0) {
            return {""};
        }
        // find first
        auto [current_offset, current] = patch_at(from);

        size_t remaining_len = len;
        CPatchStr patch_str("");
        if (from != current_offset || (from + len < current_offset + current->size())) {
            // in this case just make first patch a substring
            auto substr = substring(from - current_offset,
                                    min(current->size() - (from - current_offset), remaining_len),
                                    current->m_patch.get());
            auto to_append = CPatch(substr);
            remaining_len -= to_append.size();
            patch_str.append(to_append);
        } else {
            // this whole chunk should be copied
            auto to_append = CPatch(current->m_patch);
            remaining_len -= to_append.size();
            patch_str.append(to_append);
        }
        // iterate until it is the last chunk

        while (from + len > current_offset + current->size()) {
            // get the next patch
            current_offset += current->size();
            current = current->next();
            // check if we have last patch to work with
            if (from + len < current_offset + current->size()) {
                // copy substring
                auto substr = substring(0, remaining_len, current->m_patch.get());
                auto to_append = CPatch(substr);
                remaining_len -= to_append.size();
                patch_str.append(to_append);
            } else {
                // just copy the whole chunk
                auto to_append = CPatch(current->m_patch);
                remaining_len -= to_append.size();
                patch_str.append(to_append);
            }
        }

        return patch_str;
    }

    static shared_ptr<char[]> substring(size_t offset, size_t len, const char *src) {
        char *buffer = new char[len + 1];
        for (size_t i = 0; i <= len; i++) {
            buffer[i] = src[i + offset];
        }
        buffer[len] = '\0';
        return shared_ptr<char[]>(buffer);
    }

    CPatchStr &append(const CPatchStr &src) {
        if (this == &src) {
            auto copy = src;
            return append(copy);
        }
        // current CPatchStr is empty string
        CPatch **dst_next;
        if (empty()) {
            delete m_head;
            m_head = nullptr;
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

    CPatchStr &append(CPatch &src) {
        if (src.empty()) {
            return *this;
        }
        if (empty()) {
            delete m_head;
            m_head = new CPatch(src.m_patch);
            m_size = m_head->size();
            m_tail = m_head;
        } else {
            m_tail->next() = new CPatch(src.m_patch);
            m_tail = m_tail->next();
            m_size += src.size();
        }
        return *this;
    }

    CPatchStr &prepend(const CPatchStr &src) {
        auto *head_tmp = m_head;
        auto *tail_tmp = m_tail;
        size_t size_tmp = m_size;

        m_head = new CPatch("");
        m_tail = m_head;
        m_size = 0;
        append(src);

        m_tail->next() = head_tmp;
        m_tail = tail_tmp;
        m_size += size_tmp;
        return *this;
    }

    static pair<shared_ptr<char[]>, shared_ptr<char[]>> split(const char *src, size_t offset) {
        char *left_part = new char[offset + 1];
        char *right_part = new char[strlen(src) - offset + 1];
        for (size_t i = 0; i < offset; i++) {
            left_part[i] = src[i];
        }
        left_part[offset] = '\0';
        for (size_t i = 0; i < strlen(src) - offset; i++) {
            right_part[i] = src[i + offset];
        }
        right_part[strlen(src) - offset] = '\0';

        return {shared_ptr<char[]>(left_part), shared_ptr<char[]>(right_part)};
    }

    size_t size() const {
        return m_size;
    }

    CPatchStr &insert(size_t pos,
                      const CPatchStr &src) {
        if (pos > m_size) {
            throw out_of_range("Insert position is out of bounds.");
        }

        if (pos == 0) {
            // just prepend
            prepend(src);
            return *this;
        } else if (pos == m_size - 1) {
            // just append
            append(src);
            return *this;
        }


        size_t current_offset = 0;
        auto [previous, current] = patch_at(pos, current_offset);


        if (pos == current_offset) {
            // use predecessor to append
            auto *tail_tmp = m_tail;
            previous->next() = nullptr;
            m_tail = previous;
            append(src);
            m_tail->next() = current;
            m_tail = tail_tmp;
        } else if (pos == current_offset + current->size()) {
            // append after current
            auto *next = current->next();
            current->next() = nullptr;
            auto *tail_tmp = m_tail;
            m_tail = current;
            append(src);
            m_tail->next() = next;
            m_tail = tail_tmp;
        } else {
            // split into two parts and insert between them using split
            auto [left, right] = split(current->m_patch.get(), pos - current_offset);
            auto *next = current->next();
            current->next() = nullptr;
            delete current;

            auto *tail_tmp = m_tail;

            if (previous == nullptr) {
                m_head = new CPatch(left);
                m_tail = m_head;
            } else {
                previous->next() = new CPatch(left);
                m_tail = previous->next();
            }
            append(src);
            m_tail->next() = new CPatch(right);
            m_tail->next()->next() = next;
            m_tail = tail_tmp;
        }

        return *this;
    }

    CPatchStr &remove(size_t from,
                      size_t len) {
        if (from > m_size || from + len > m_size) {
            throw out_of_range("Cannot remove - out of range.");
        }
        if (len == 0) {
            return *this;
        }

        // find first
        size_t current_offset = 0;
        auto [previous, current] = patch_at(from, current_offset);

        CPatch *left_end = nullptr;

        if (from != current_offset || (from + len < current_offset + current->size())) {
            // in this case we have to delete part of this chunk and that is all
            // deal with both sides then
            if (from == current_offset) {
                // current should be disconnected
                if (previous != nullptr) {
                    previous->next() = nullptr;
                    left_end = previous;
                } else {
                    m_head = nullptr;
                }
            } else {
                // get substring, connect it and leave the left end
                auto substr = substring(0, from - current_offset, current->m_patch.get());
                if (previous == nullptr) {
                    m_head = new CPatch(substr);
                    left_end = m_head;
                } else {
                    previous->next() = new CPatch(substr);
                    left_end = previous->next();
                }
                m_size += left_end->size();
            }

            if (from + len < current_offset + current->size()) {
                auto substr = substring(from + len - current_offset, current->size() - (from + len - current_offset),
                                        current->m_patch.get());
                auto *to_add = new CPatch(substr);
                if (left_end == nullptr) {
                    left_end = to_add;
                    m_head = to_add;
                } else {
                    left_end->next() = to_add;
                }
                m_size += to_add->size();
                to_add->next() = current->next();
                current->next() = nullptr;
                m_size -= current->size();
                delete current;
                return *this;

            } else if (from + len == current_offset + current->size()) {
                if (left_end == nullptr) {
                    left_end = current->next();
                    m_head = current->next();
                } else {
                    left_end->next() = current->next();

                }
                current->next() = nullptr;
                m_size -= current->size();
                delete current;
                return *this;
            }
        }

        while (current_offset + current->size() < from + len) {
            current_offset += current->size();
            auto *next = current->next();
            m_size -= current->size();
            current->next() = nullptr;
            delete current;
            current = next;
        }

        if (current_offset + current->size() == from + len) {
            if (left_end == nullptr) {
                left_end = current->next();
                m_head = left_end;
            } else {
                left_end->next() = current->next();
            }
            current->next() = nullptr;
            m_size -= current->size();
            delete current;
        } else {
            auto substr = substring(from + len - current_offset, current->size() + current_offset - from - len,
                                    current->m_patch.get());
            m_size -= current->size();
            auto *p = new CPatch(substr);
            m_size += p->size();
            if (left_end == nullptr) {
                left_end = p;
                m_head = left_end;
            } else {
                left_end->next() = p;

            }
            p->next() = current->next();
            current->next() = nullptr;
            if (current == m_tail) {
                m_tail = p;
            }
            delete current;
        }
        return *this;
    }

    pair<size_t, CPatch *> patch_at(size_t index) const {
        size_t current_offset = 0;
        auto [previous, current] = patch_at(index, current_offset);
        return {current_offset, current};
    }


    pair<CPatch *, CPatch *> patch_at(size_t index, size_t &offset) const {
        CPatch *previous = nullptr;
        CPatch *current = m_head;
        size_t current_offset = 0;
        while (current_offset + current->size() < index) {
            current_offset += current->size();
            previous = current;
            current = current->next();
        }
        offset = current_offset;
        return {previous, current};
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
    CPatchStr c(a);
    assert (stringMatch(c.toStr(), "test data"));
    auto substring = a.subStr(3, 5);
    CPatchStr d(a.subStr(3, 5));
    assert (stringMatch(d.toStr(), "t dat"));
    d.append(b);
    assert (stringMatch(d.toStr(), "t datfoo text"));
    d.append(b.subStr(3, 4));
    assert (stringMatch(d.toStr(), "t datfoo text tex"));
    c.append(d);
    assert (stringMatch(c.toStr(), "test datat datfoo text tex"));
    c.append(c);
    assert (stringMatch(c.toStr(), "test datat datfoo text textest datat datfoo text tex"));
    d.insert(2, c.subStr(6, 9));
    assert (stringMatch(d.toStr(), "t atat datfdatfoo text tex"));
    b = "abcdefgh";
    assert (stringMatch(b.toStr(), "abcdefgh"));
    assert (stringMatch(d.toStr(), "t atat datfdatfoo text tex"));
    assert (stringMatch(d.subStr(4, 8).toStr(), "at datfd"));
    assert (stringMatch(b.subStr(2, 6).toStr(), "cdefgh"));
    try {
        b.subStr(2, 7).toStr();
        assert ("Exception not thrown" == nullptr);
    }
    catch (const std::out_of_range &e) {
    }
    catch (...) {
        assert ("Invalid exception thrown" == nullptr);
    }
    a.remove(3, 5);
    assert (stringMatch(a.toStr(), "tesa"));

    // own tests
    CPatchStr s1 = CPatchStr();
    auto s2 = s1;
    assert(stringMatch(s1.toStr(), ""));
    assert(s1.size() == strlen(""));
    assert(stringMatch(s2.toStr(), ""));
    assert(s2.size() == strlen(""));
    s1.append(s2);
    assert(stringMatch(s1.toStr(), ""));
    assert(s1.size() == strlen(""));


    CPatchStr s3 = CPatchStr("Hello");
    s3.append(s1);
    assert(stringMatch(s3.toStr(), "Hello"));
    assert(s3.size() == strlen("Hello"));

    s1.append(s3);
    assert(stringMatch(s1.toStr(), "Hello"));
    assert(s1.size() == strlen("Hello"));

    assert(stringMatch(s1.subStr(0, 4).toStr(), "Hell"));
    assert(s1.subStr(0, 4).size() == strlen("Hell"));
    assert(stringMatch(s1.subStr(0, 5).toStr(), "Hello"));
    assert(s1.subStr(0, 5).size() == strlen("Hello"));

    assert(stringMatch(s1.subStr(1, 4).toStr(), "ello"));
    assert(s1.subStr(1, 4).size() == strlen("ello"));
    assert(stringMatch(s1.subStr(1, 3).toStr(), "ell"));
    assert(s1.subStr(1, 3).size() == strlen("ell"));

    CPatchStr s4 = CPatchStr(" there");
    CPatchStr s5 = CPatchStr(" world");
    s3.append(s4);
    s3.append(s5);
    s5 = s3;
    auto s6 = CPatchStr(s3);

    assert(stringMatch(s3.toStr(), "Hello there world"));
    assert(s3.size() == strlen("Hello there world"));
    assert(stringMatch(s5.toStr(), "Hello there world"));
    assert(s5.size() == strlen("Hello there world"));
    assert(stringMatch(s6.toStr(), "Hello there world"));
    assert(s6.size() == strlen("Hello there world"));

    assert(stringMatch(s3.subStr(0, 17).toStr(), "Hello there world"));
    assert(s3.subStr(0, 17).size() == strlen("Hello there world"));
    assert(stringMatch(s3.subStr(5, 6).toStr(), " there"));
    assert(s3.subStr(5, 6).size() == strlen(" there"));
    assert(stringMatch(s3.subStr(6, 5).toStr(), "there"));
    assert(s3.subStr(6, 5).size() == strlen("there"));
    assert(stringMatch(s3.subStr(6, 4).toStr(), "ther"));
    assert(s3.subStr(6, 4).size() == strlen("ther"));
    assert(stringMatch(s3.subStr(5, 5).toStr(), " ther"));
    assert(s3.subStr(5, 5).size() == strlen(" ther"));
    assert(stringMatch(s3.subStr(0, 0).toStr(), ""));
    assert(s3.subStr(0, 0).size() == strlen(""));
    assert(stringMatch(s3.subStr(0, 16).toStr(), "Hello there worl"));
    assert(s3.subStr(0, 16).size() == strlen("Hello there worl"));
    assert(stringMatch(s3.subStr(1, 15).toStr(), "ello there worl"));
    assert(s3.subStr(1, 15).size() == strlen("ello there worl"));
    assert(stringMatch(s3.subStr(0, 11).toStr(), "Hello there"));
    assert(s3.subStr(0, 11).size() == strlen("Hello there"));
    assert(stringMatch(s3.subStr(1, 10).toStr(), "ello there"));
    assert(s3.subStr(1, 10).size() == strlen("ello there"));
    assert(stringMatch(s3.subStr(4, 7).toStr(), "o there"));
    assert(s3.subStr(4, 7).size() == strlen("o there"));

    // testing insert
    s6 = CPatchStr("INJ");
    s3.insert(0, s6);
    assert(stringMatch(s3.toStr(), "INJHello there world"));
    assert(s3.size() == strlen("INJHello there world"));
    s3 = s5;
    s3.insert(16, s6);
    assert(stringMatch(s3.toStr(), "Hello there worldINJ"));
    assert(s3.size() == strlen("Hello there worldINJ"));
    s3 = s5;
    s3.insert(5, s6);
    assert(stringMatch(s3.toStr(), "HelloINJ there world"));
    assert(s3.size() == strlen("HelloINJ there world"));
    s3 = s5;
    s3.insert(11, s6);
    assert(stringMatch(s3.toStr(), "Hello thereINJ world"));
    assert(s3.size() == strlen("Hello thereINJ world"));
    s3 = s5;
    s3.insert(4, s6);
    assert(stringMatch(s3.toStr(), "HellINJo there world"));
    assert(s3.size() == strlen("HellINJo there world"));
    s3 = s5;
    s3.insert(10, s6);
    assert(stringMatch(s3.toStr(), "Hello therINJe world"));
    assert(s3.size() == strlen("Hello therINJe world"));
    s3 = s5;
    s3.insert(1, s6);
    assert(stringMatch(s3.toStr(), "HINJello there world"));
    assert(s3.size() == strlen("HINJello there world"));
    s3 = s5;
    s3.insert(7, s6);
    assert(stringMatch(s3.toStr(), "Hello tINJhere world"));
    assert(s3.size() == strlen("Hello tINJhere world"));

    s6.append(s6).append(s6);
    assert(stringMatch(s6.toStr(), "INJINJINJINJ"));
    assert(s6.size() == strlen("INJINJINJINJ"));

    s3 = s5;
    s3.insert(0, s6);
    assert(stringMatch(s3.toStr(), "INJINJINJINJHello there world"));
    assert(s3.size() == strlen("INJINJINJINJHello there world"));
    s3 = s5;
    s3.insert(16, s6);
    assert(stringMatch(s3.toStr(), "Hello there worldINJINJINJINJ"));
    assert(s3.size() == strlen("Hello there worldINJINJINJINJ"));
    s3 = s5;
    s3.insert(5, s6);
    assert(stringMatch(s3.toStr(), "HelloINJINJINJINJ there world"));
    assert(s3.size() == strlen("HelloINJINJINJINJ there world"));
    s3 = s5;
    s3.insert(11, s6);
    assert(stringMatch(s3.toStr(), "Hello thereINJINJINJINJ world"));
    assert(s3.size() == strlen("Hello thereINJINJINJINJ world"));
    s3 = s5;
    s3.insert(4, s6);
    assert(stringMatch(s3.toStr(), "HellINJINJINJINJo there world"));
    assert(s3.size() == strlen("HellINJINJINJINJo there world"));
    s3 = s5;
    s3.insert(10, s6);
    assert(stringMatch(s3.toStr(), "Hello therINJINJINJINJe world"));
    assert(s3.size() == strlen("Hello therINJINJINJINJe world"));
    s3 = s5;
    s3.insert(1, s6);
    assert(stringMatch(s3.toStr(), "HINJINJINJINJello there world"));
    assert(s3.size() == strlen("HINJINJINJINJello there world"));
    s3 = s5;
    s3.insert(7, s6);
    assert(stringMatch(s3.toStr(), "Hello tINJINJINJINJhere world"));
    assert(s3.size() == strlen("Hello tINJINJINJINJhere world"));

    s3 = s5;
    s3.remove(0, 0);
    assert(stringMatch(s3.toStr(), "Hello there world"));
    assert(s3.size() == strlen("Hello there world"));
    s3 = s5;
    s3.remove(4, 0);
    assert(stringMatch(s3.toStr(), "Hello there world"));
    assert(s3.size() == strlen("Hello there world"));
    s3 = s5;
    s3.remove(11, 0);
    assert(stringMatch(s3.toStr(), "Hello there world"));
    assert(s3.size() == strlen("Hello there world"));
    s3 = s5;
    s3.remove(7, 0);
    assert(stringMatch(s3.toStr(), "Hello there world"));
    assert(s3.size() == strlen("Hello there world"));

    s3 = s5;
    s3.remove(0, 2);
    assert(stringMatch(s3.toStr(), "llo there world"));
    assert(s3.size() == strlen("llo there world"));
    s3 = s5;
    s3.remove(1, 2);
    assert(stringMatch(s3.toStr(), "Hlo there world"));
    assert(s3.size() == strlen("Hlo there world"));

    s3 = s5;
    s3.remove(4, 2);
    assert(stringMatch(s3.toStr(), "Hellthere world"));
    assert(s3.size() == strlen("Hellthere world"));
    s3 = s5;
    s3.remove(0, 6);
//    assert(stringMatch(s3.toStr(), "there world"));
//    assert(s3.size() == strlen("there world"));
//
//
//    s3 = s5;
//    s3.remove(0, 4);
//    assert(stringMatch(s3.toStr(), "o there world"));
//    assert(s3.size() == strlen("o there world"));


    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
