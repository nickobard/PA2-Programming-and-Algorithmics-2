#ifndef __PROGTEST__

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cstdint>
#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include <memory>
#include <functional>
#include <compare>
#include <stdexcept>

#endif /* __PROGTEST__ */

using namespace std;

class CBigInt {
public:
    // default constructor
    // copying/assignment/destruction
    // int constructor
    CBigInt(int number = 0) {
        if (number < 0) {
            m_is_negative = true;
            number = -number;
        } else {
            m_is_negative = false;
        }
        m_num = to_string(number);
        reverse(m_num.begin(), m_num.end());

    }

    // string constructor
    CBigInt(const char *str) {
        string result;
        size_t i = 0;
        if (str[i] == '-') {
            m_is_negative = true;
            i++;
        } else {
            m_is_negative = false;
        }
        bool leading_zeros = true;
        for (; i < strlen(str); i++) {
            if (leading_zeros && str[i] == '0') {
                continue;
            }
            if (isdigit(str[i])) {
                leading_zeros = false;
                result.push_back(str[i]);
            } else {
                throw invalid_argument("");
            }
        }

        m_num = result;
        reverse(m_num.begin(), m_num.end());
    }

    CBigInt(string num, const bool is_negative) : m_num(std::move(num)), m_is_negative(is_negative) {}

    // operator +, any combination {CBigInt/int/string} + {CBigInt/int/string}
    CBigInt operator+(const CBigInt &that) {
        if (positive() && that.positive()) {
            if (*this < that) { // that is bigger
                return {add(that.m_num, m_num), false};
            } else { // this is bigger
                return {add(m_num, that.m_num), false};
            }
        } else if (negative() && that.negative()) {
            if (abs(*this) < abs(that)) { // that is bigger in absolutes
                return {add(that.m_num, m_num), true};
            } else { // this is bigger
                return {add(m_num, that.m_num), true};
            }
        } else if (negative() && that.positive()) {
            if (abs(*this) == that) {
                return {"0", false};
            } else if (abs(*this) < that) { // that is bigger in absolutes
                return {subtract(that.m_num, m_num), false};
            } else { // this is bigger
                return {subtract(m_num, that.m_num), true};
            }
        } else { // this positive and that negative
            if (*this == abs(that)) {
                return {"0", false};
            } else if (*this < abs(that)) { // that is bigger in absolutes
                return {subtract(that.m_num, m_num), true};
            } else { // this is bigger
                return {subtract(m_num, that.m_num), false};
            }
        }
    }

    // operator *, any combination {CBigInt/int/string} * {CBigInt/int/string}
    CBigInt operator*(const CBigInt &that) {
        if ((positive() && that.positive()) || (negative() && that.negative())) {
            return {multiply(m_num, that.m_num), false};
        }
        string result = multiply(m_num, that.m_num);
        if (result == "0") {
            return {result, false};
        }
        return {multiply(m_num, that.m_num), true};
    }

    CBigInt &operator*=(const CBigInt &that) {
        *this = *this * that;
        return *this;
    }

    // operator +=, any of {CBigInt/int/string}
    CBigInt &operator+=(const CBigInt &that) {
        *this = *this + that;
        return *this;
    }

    // operator *=, any of {CBigInt/int/string}
    // comparison operators, any combination {CBigInt/int/string} {<,<=,>,>=,==,!=} {CBigInt/int/string}
    bool operator==(const CBigInt &that) {
        return m_num == that.m_num && m_is_negative == that.m_is_negative;
    }

    bool operator<(const CBigInt &that) {
        if (positive() && that.positive()) {
            if (m_num.size() == that.m_num.size()) {
                string this_str = this->m_num;
                string that_str = that.m_num;
                reverse(this_str.begin(), this_str.end());
                reverse(that_str.begin(), that_str.end());
                return this_str < that_str;
            }
            return m_num.size() < that.m_num.size();
        } else if (negative() && that.negative()) {
            if (m_num.size() == that.m_num.size()) {
                string this_str = this->m_num;
                string that_str = that.m_num;
                reverse(this_str.begin(), this_str.end());
                reverse(that_str.begin(), that_str.end());
                return this_str > that_str;
            }
            return m_num.size() > that.m_num.size();
        } else if (negative() && that.positive())
            return true;
        else
            return false;
    }

    bool operator<=(const CBigInt &that) {
        return *this == that || *this < that;
    }

    bool operator!=(const CBigInt &that) {
        return !(*this == that);
    }

    bool operator>(const CBigInt &that) {
        return !(*this <= that);
    }

    bool operator>=(const CBigInt &that) {
        return !(*this < that);
    }

    // output operator <<
    friend ostream &operator<<(ostream &os, CBigInt num) {
        string to_print = num.m_num;
        reverse(to_print.begin(), to_print.end());
        if (num.negative()) {
            os << '-';
        }
        os << to_print;
        return os;
    }

    // input operator >>
    friend istream &operator>>(istream &is, CBigInt &num) {
        string input;
        bool whitespace_sequence = true;

        while (is) {
            char peeked = (char) is.peek();
            if (whitespace_sequence) {
                if (isspace(peeked)) {
                    is.get();
                } else if (peeked == '-') {
                    is.get();
                    if (!is) {
                        is.setstate(ios::failbit);
                        return is;
                    }
                    input.push_back(peeked);

                    char next = (char) is.peek();
                    if (!isdigit(next)) {
                        is.setstate(ios::failbit);
                        return is;
                    }
                    input.push_back(peeked);
                    input.push_back(next);
                    is.get();
                    whitespace_sequence = false;
                } else if (isdigit(peeked)) {
                    whitespace_sequence = false;
                    input.push_back(peeked);
                    is.get();
                } else {
                    is.setstate(ios::failbit);
                    return is;
                }
            } else {
                if (!isdigit(peeked)) {
                    num = CBigInt(input.data());
                    return is;
                }
                input.push_back(peeked);
                is.get();
            }

        }
        if (is.eof() && !input.empty()) {
            num = CBigInt(input.data());
            return is;
        } else {
            is.setstate(ios::failbit);
            return is;
        }
    }

private:

    bool positive() const {
        return !m_is_negative;
    }

    bool negative() const {
        return m_is_negative;
    }

    static CBigInt abs(const CBigInt &bigint) {
        return {bigint.m_num, false};
    }

    static string subtract(const string &bigger, const string &lesser) {
        string result;
        int carry = 0;
        size_t i;
        for (i = 0; i < bigger.size(); i++) {
            int sum = carry;
            if (i < lesser.size()) {
                sum += (bigger[i] - '0') - (lesser[i] - '0');
            } else {
                sum += (bigger[i] - '0');
            }
            carry = sum / 10;
            result.push_back((char) (sum % 10 + '0'));
        }
        while (carry != 0) {
            int sum = carry;
            sum += (bigger[i] - '0');
            carry = sum / 10;
            result.push_back((char) (sum % 10 + '0'));
        }
        return result;
    }

    static string multiply(const string &multiplied, const string &multiplier) {
        string result = "0";
        for (size_t i = 0; i < multiplier.size(); i++) {
            if (multiplier[i] == '0')
                continue;
            int carry = 0;
            string to_add = string(i, '0');
            for (size_t j = 0; j < multiplied.size(); j++) {
                int product = carry;
                product += ((multiplier[i] - '0') * (multiplied[j] - '0'));
                carry = product / 10;
                to_add.push_back((char) (product % 10 + '0'));
            }
            while (carry) {
                to_add.push_back((char) (carry % 10 + '0'));
                carry = carry / 10;
            }
            if (result.size() > to_add.size()) {
                result = add(result, to_add);
            } else {
                result = add(to_add, result);
            }
        }
        return result;
    }

    static string add(const string &bigger, const string &lesser) {
        string result;
        int carry = 0;
        size_t i;
        for (i = 0; i < bigger.size(); i++) {
            int sum = carry;
            if (i < lesser.size()) {
                sum += (bigger[i] - '0') + (lesser[i] - '0');
            } else {
                sum += (bigger[i] - '0');
            }
            carry = sum / 10;
            result.push_back((char) (sum % 10 + '0'));
        }
        while (carry != 0) {
            int sum = carry;
            carry = sum / 10;
            result.push_back((char) (sum % 10 + '0'));
        }
        return result;
    }

    string m_num;
    bool m_is_negative;
};

#ifndef __PROGTEST__

static bool equal(const CBigInt &x, const char val[]) {
    std::ostringstream oss;
    oss << x;
    return oss.str() == val;
}

static bool equalHex(const CBigInt &x, const char val[]) {
    return true; // hex output is needed for bonus tests only
    // std::ostringstream oss;
    // oss << std::hex << x;
    // return oss . str () == val;
}

int main() {
    CBigInt a, b;
    std::istringstream is;
    a = 10;
    a += 20;
    assert (equal(a, "30"));
    a *= 5;
    assert (equal(a, "150"));
    b = a + 3;
    assert (equal(b, "153"));
    b = a * 7;
    assert (equal(b, "1050"));
    assert (equal(a, "150"));
    assert (equalHex(a, "96"));

    a = 10;
    a += -20;
    assert (equal(a, "-10"));
    a *= 5;
    assert (equal(a, "-50"));
    b = a + 73;
    assert (equal(b, "23"));
    b = a * -7;
    assert (equal(b, "350"));
    assert (equal(a, "-50"));
    assert (equalHex(a, "-32"));

    a = "12345678901234567890";
    a += "-99999999999999999999";
    assert (equal(a, "-87654321098765432109"));

    // own tests
    CBigInt c = a;
    CBigInt tmp = c * 0;
    assert(equal(tmp, "0"));
    assert(equal(c * 1, "-87654321098765432109"));
    assert(equal(c * 2, "-175308642197530864218"));
    assert(equal(c * 4, "-350617284395061728436"));
    assert(equal(c * 54, "-4733333339333333333886"));
    assert(equal(c * 654, "-57325925998592592599286"));
    assert(equal(c * 7654, "-670906173689950617362286"));
    assert(equal(c * 87654, "-7683251861591185186082286"));
    assert(equal(c * 987654, "-86572140850480074084182286"));
    tmp = c * 14;
    assert(equal(tmp, "-1227160495382716049526"));
    assert(equal(c * 21987654, "-1927312883924554148373182286"));
    assert(equal(c * 321987654, "-28223609213554183781073182286"));


    a *= "54321987654321987654";
    assert (equal(a, "-4761556948575111126880627366067073182286"));
    a *= 0;
    assert (equal(a, "0"));
    a = 10;
    b = a + "400";
    assert (equal(b, "410"));
    b = a * "15";
    assert (equal(b, "150"));
    assert (equal(a, "10"));
    assert (equalHex(a, "a"));

    is.clear();
    is.str(" 1234");
    assert (is >> b);
    assert (equal(b, "1234"));
    is.clear();
    is.str(" 12 34");
    assert (is >> b);
    assert (equal(b, "12"));
    is.clear();
    is.str("999z");
    assert (is >> b);
    assert (equal(b, "999"));
    is.clear();
    is.str("abcd");
    assert (!(is >> b));
    is.clear();
    is.str("- 758");
    assert (!(is >> b));
    a = 42;
    try {
        a = "-xyz";
        assert ("missing an exception" == nullptr);
    }
    catch (const std::invalid_argument &e) {
        assert (equal(a, "42"));
    }

    a = "73786976294838206464";
    assert (equal(a, "73786976294838206464"));
    assert (equalHex(a, "40000000000000000"));
    assert (a < "1361129467683753853853498429727072845824");
    assert (a <= "1361129467683753853853498429727072845824");
    assert (!(a > "1361129467683753853853498429727072845824"));
    assert (!(a >= "1361129467683753853853498429727072845824"));
    assert (!(a == "1361129467683753853853498429727072845824"));
    assert (a != "1361129467683753853853498429727072845824");
    assert (!(a < "73786976294838206464"));
    assert (a <= "73786976294838206464");
    assert (!(a > "73786976294838206464"));
    assert (a >= "73786976294838206464");
    assert (a == "73786976294838206464");
    assert (!(a != "73786976294838206464"));
    assert (a < "73786976294838206465");
    assert (a <= "73786976294838206465");
    assert (!(a > "73786976294838206465"));
    assert (!(a >= "73786976294838206465"));
    assert (!(a == "73786976294838206465"));
    assert (a != "73786976294838206465");
    a = "2147483648";
    assert (!(a < -2147483648));
    assert (!(a <= -2147483648));
    assert (a > -2147483648);
    assert (a >= -2147483648);
    assert (!(a == -2147483648));
    assert (a != -2147483648);
    a = "-12345678";
    assert (!(a < -87654321));
    assert (!(a <= -87654321));
    assert (a > -87654321);
    assert (a >= -87654321);
    assert (!(a == -87654321));
    assert (a != -87654321);

    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
