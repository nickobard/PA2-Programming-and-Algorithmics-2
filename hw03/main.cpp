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
        auto *current = m_head;
        size_t current_offset = 0;
        size_t remaining_len = len;
        while (current_offset + current->size() < from) {
            current_offset += current->size();
            current = current->next();
        }

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

    CPatchStr &insert(size_t pos,
                      const CPatchStr &src) {
        if (pos < 0 || pos > m_size) {
            throw out_of_range("Insert position is out of bounds.");
        }

        if (pos == 0) {
            // just prepend
            prepend(src);
        } else if (pos == m_size) {
            // just append
            append(src);
        }


        CPatch *pred = nullptr;
        CPatch *current = m_head;
        size_t current_offset = 0;
        while (current_offset + current->size() < pos) {
            current_offset += current->size();
            pred = current;
            current = current->next();
        }

        if (pos == current_offset) {
            // use predecessor to append
            auto *tail_tmp = m_tail;
            pred->next() = nullptr;
            m_tail = pred;
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
            current->next() = next;
            m_tail = tail_tmp;
        } else {
            // split into two parts and insert between them using split
            auto [left, right] = split(current->m_patch.get(), pos - current_offset);
            auto *next = current->next();
            current->next() = nullptr;
            delete current;

            auto *tail_tmp = m_tail;

            pred->next() = new CPatch(left);
            m_tail = pred->next();
            append(src);
            m_tail->next() = new CPatch(right);
            m_tail->next()->next() = next;
            m_tail = tail_tmp;
        }

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
    CPatchStr c(a);
    assert (stringMatch(c.toStr(), "test data"));
    auto substring = a.subStr(3, 5);
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
