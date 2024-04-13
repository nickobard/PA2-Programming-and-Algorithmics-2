//
// Created by bardanik on 11/04/24.
//

#ifndef BARDANIK_CSPREADSHEET_H
#define BARDANIK_CSPREADSHEET_H

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
#include "../expression.h"

#include "CCell.h"


constexpr unsigned SPREADSHEET_CYCLIC_DEPS = 0x01;
constexpr unsigned SPREADSHEET_FUNCTIONS = 0x02;
constexpr unsigned SPREADSHEET_FILE_IO = 0x04;
constexpr unsigned SPREADSHEET_SPEED = 0x08;
constexpr unsigned SPREADSHEET_PARSER = 0x10;

constexpr size_t INITIAL_TABLE_SIZE = 1000;

class CSpreadsheet {
public:
    static unsigned capabilities() {
        return 0;
//        return SPREADSHEET_CYCLIC_DEPS | SPREADSHEET_FUNCTIONS | SPREADSHEET_FILE_IO | SPREADSHEET_SPEED |
//        SPREADSHEET_PARSER;
    }

    CSpreadsheet();

    bool load(istream &is);

    bool save(ostream &os) const;

    bool setCell(CPos pos,
                 string contents);

    CValue getValue(CPos pos);

    void copyRect(CPos dst,
                  CPos src,
                  int w = 1,
                  int h = 1);

private:
    bool cellOutOfRange(const CPos &pos) const;

    vector<vector<CCell>> m_cells;
    size_t m_rows;
    size_t m_cols;
};


#endif //BARDANIK_CSPREADSHEET_H
