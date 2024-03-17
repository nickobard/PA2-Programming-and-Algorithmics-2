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
    bool positive() const {
        return !m_is_negative;
    }

    bool negative() const {
        return m_is_negative;
    }

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
    CBigInt(const string &str) {
        string result;
        size_t i = 0;
        if (str[i] == '-') {
            m_is_negative = true;
            i++;
        } else {
            m_is_negative = false;
        }
        for (i; i < str.size(); i++) {
            if (str[i] == '0')
                continue;
            if (isdigit(str[i]))
                result.push_back(str[i]);
            throw invalid_argument("");
        }

        m_num = result;
    }

    CBigInt(string num, const bool is_negative) : m_num(std::move(num)), m_is_negative(is_negative) {}

    // operator +, any combination {CBigInt/int/string} + {CBigInt/int/string}
    CBigInt operator+(const CBigInt &that) {
        if (positive() && that.positive()) {
            if (that.m_num.size() > m_num.size()) { // that is bigger
                return {add(that.m_num, m_num), false};
            } else { // this is bigger
                return {add(m_num, that.m_num), false};
            }
        }
        return {};
    }

    // operator *, any combination {CBigInt/int/string} * {CBigInt/int/string}
    CBigInt operator*(const CBigInt &that) {
        if (positive() && that.positive()) {
            return {multiply(m_num, that.m_num), false};
        }
        return {};
    }

    CBigInt &operator*=(const CBigInt &that) {
        *this = *this + that;
        return *this;
    }

    // operator +=, any of {CBigInt/int/string}
    CBigInt &operator+=(const CBigInt &that) {
        *this = *this + that;
        return *this;
    }

    // operator *=, any of {CBigInt/int/string}
    // comparison operators, any combination {CBigInt/int/string} {<,<=,>,>=,==,!=} {CBigInt/int/string}
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

//private:
    static string multiply(const string &multiplied, const string &multiplier) {
        string result;
        for (size_t i = 0; i < multiplier.size(); i++) {
            int carry = 0;
            string to_add = string(i, '0');
            for (size_t j = 0; j < multiplied.size(); j++) {
                for (size_t pad = 0; pad < j; pad++) {
                    to_add.push_back('0');
                }
                int product = carry;
                product += ((multiplier[i] - '0') * (multiplied[j] - '0'));
                carry = product / 10;
                to_add.push_back((char) (product % 10 + '0'));
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
            sum += (bigger[i] - '0') + (lesser[i] - '0');
            carry = sum / 10;
            result.push_back((char) (sum % 10 + '0'));
        }
        while (carry) {
            int sum = carry;
            sum += (bigger[i] - '0');
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

//static bool equalHex(const CBigInt &x, const char val[]) {
//    return true; // hex output is needed for bonus tests only
//    // std::ostringstream oss;
//    // oss << std::hex << x;
//    // return oss . str () == val;
//}

int main() {
    CBigInt a, b;
    std::istringstream is;
    a = 10;
    a += 20;
    assert (equal(a, "30"));
    a *= 5;
//  assert ( equal ( a, "150" ) );
//  b = a + 3;
//  assert ( equal ( b, "153" ) );
//  b = a * 7;
//  assert ( equal ( b, "1050" ) );
//  assert ( equal ( a, "150" ) );
//  assert ( equalHex ( a, "96" ) );
//
//  a = 10;
//  a += -20;
//  assert ( equal ( a, "-10" ) );
//  a *= 5;
//  assert ( equal ( a, "-50" ) );
//  b = a + 73;
//  assert ( equal ( b, "23" ) );
//  b = a * -7;
//  assert ( equal ( b, "350" ) );
//  assert ( equal ( a, "-50" ) );
//  assert ( equalHex ( a, "-32" ) );
//
//  a = "12345678901234567890";
//  a += "-99999999999999999999";
//  assert ( equal ( a, "-87654321098765432109" ) );
//  a *= "54321987654321987654";
//  assert ( equal ( a, "-4761556948575111126880627366067073182286" ) );
//  a *= 0;
//  assert ( equal ( a, "0" ) );
//  a = 10;
//  b = a + "400";
//  assert ( equal ( b, "410" ) );
//  b = a * "15";
//  assert ( equal ( b, "150" ) );
//  assert ( equal ( a, "10" ) );
//  assert ( equalHex ( a, "a" ) );
//
//  is . clear ();
//  is . str ( " 1234" );
//  assert ( is >> b );
//  assert ( equal ( b, "1234" ) );
//  is . clear ();
//  is . str ( " 12 34" );
//  assert ( is >> b );
//  assert ( equal ( b, "12" ) );
//  is . clear ();
//  is . str ( "999z" );
//  assert ( is >> b );
//  assert ( equal ( b, "999" ) );
//  is . clear ();
//  is . str ( "abcd" );
//  assert ( ! ( is >> b ) );
//  is . clear ();
//  is . str ( "- 758" );
//  assert ( ! ( is >> b ) );
//  a = 42;
//  try
//  {
//    a = "-xyz";
//    assert ( "missing an exception" == nullptr );
//  }
//  catch ( const std::invalid_argument & e )
//  {
//    assert ( equal ( a, "42" ) );
//  }
//
//  a = "73786976294838206464";
//  assert ( equal ( a, "73786976294838206464" ) );
//  assert ( equalHex ( a, "40000000000000000" ) );
//  assert ( a < "1361129467683753853853498429727072845824" );
//  assert ( a <= "1361129467683753853853498429727072845824" );
//  assert ( ! ( a > "1361129467683753853853498429727072845824" ) );
//  assert ( ! ( a >= "1361129467683753853853498429727072845824" ) );
//  assert ( ! ( a == "1361129467683753853853498429727072845824" ) );
//  assert ( a != "1361129467683753853853498429727072845824" );
//  assert ( ! ( a < "73786976294838206464" ) );
//  assert ( a <= "73786976294838206464" );
//  assert ( ! ( a > "73786976294838206464" ) );
//  assert ( a >= "73786976294838206464" );
//  assert ( a == "73786976294838206464" );
//  assert ( ! ( a != "73786976294838206464" ) );
//  assert ( a < "73786976294838206465" );
//  assert ( a <= "73786976294838206465" );
//  assert ( ! ( a > "73786976294838206465" ) );
//  assert ( ! ( a >= "73786976294838206465" ) );
//  assert ( ! ( a == "73786976294838206465" ) );
//  assert ( a != "73786976294838206465" );
//  a = "2147483648";
//  assert ( ! ( a < -2147483648 ) );
//  assert ( ! ( a <= -2147483648 ) );
//  assert ( a > -2147483648 );
//  assert ( a >= -2147483648 );
//  assert ( ! ( a == -2147483648 ) );
//  assert ( a != -2147483648 );
//  a = "-12345678";
//  assert ( ! ( a < -87654321 ) );
//  assert ( ! ( a <= -87654321 ) );
//  assert ( a > -87654321 );
//  assert ( a >= -87654321 );
//  assert ( ! ( a == -87654321 ) );
//  assert ( a != -87654321 );

    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
