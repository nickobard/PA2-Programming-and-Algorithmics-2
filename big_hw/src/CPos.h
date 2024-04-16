//
// Created by bardanik on 11/04/24.
//
#ifndef BARDANIK_CPOS_H
#define BARDANIK_CPOS_H

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <climits>
#include <cfloat>
#include <cassert>
#include <cmath>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <string>
#include <array>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <stack>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <functional>
#include <iterator>
#include <stdexcept>
#include <variant>
#include <optional>
#include <compare>
#include <charconv>
#include <span>
#include <utility>

using namespace std;

class CPos {
public:
    explicit CPos(string_view str);

    CPos(int row, int col);

    /**
     * @return row, column pair
     */
    pair<int, int> getCoords() const;

    static pair<int, int> getOffset(const CPos &src, const CPos &dst);

    void shiftPos(const pair<int, int> &shift);

private:

    void splitPositionAndParse(const string &position);

    static int convertColLabelToNumber(const string &col_label);

    int m_row;
    bool m_absolute_row;
    int m_col;
    bool m_absolute_col;

    string m_col_label;

};

#endif //BARDANIK_CPOS_H
