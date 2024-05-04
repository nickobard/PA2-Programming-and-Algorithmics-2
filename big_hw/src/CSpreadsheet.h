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

#include "SpreadsheetStructure/CCell.h"
#include "InputOutputUtilities/CLoader.h"

constexpr unsigned SPREADSHEET_CYCLIC_DEPS = 0x01;
constexpr unsigned SPREADSHEET_FUNCTIONS = 0x02;
constexpr unsigned SPREADSHEET_FILE_IO = 0x04;
constexpr unsigned SPREADSHEET_SPEED = 0x08;
constexpr unsigned SPREADSHEET_PARSER = 0x10;


class CSpreadsheet {
public:
    static unsigned capabilities() {
        return 0;
//        return SPREADSHEET_CYCLIC_DEPS | SPREADSHEET_FUNCTIONS | SPREADSHEET_FILE_IO | SPREADSHEET_SPEED |
//        SPREADSHEET_PARSER;
    }

    CSpreadsheet() = default;

    CSpreadsheet(const CSpreadsheet &src);

    CSpreadsheet &operator=(const CSpreadsheet &src);

    bool load(istream &is);

    bool save(ostream &os) const;

    bool setCell(CPos pos,
                 string contents);

    CValue getValue(CPos pos);

    CValue getValue(CPos pos, CCycleDetectionVisitor &visitor);

    void copyRect(CPos dst,
                  CPos src,
                  int w = 1,
                  int h = 1);

    static bool setCell(Cells &cells, const CPos &pos, shared_ptr<CCell> &cell);

private:


    Cells copyCellsAndShift(const CPos &src, const CPos &dst, int w = 1, int h = 1);

    void deleteCells(const CPos &dst, int w = 1, int h = 1);

    void pasteCells(const Cells &cells);

    Cells m_cells;

};


#endif //BARDANIK_CSPREADSHEET_H
