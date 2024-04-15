//
// Created by bardanik on 11/04/24.
//

#ifndef BARDANIK_CCELL_H
#define BARDANIK_CCELL_H

#include "CASTExpressionBuilder.h"
#include "CStackExpressionBuilder.h"

using namespace literals;
using CValue = variant<monostate, double, string>;

enum class CellType{
    DOUBLE,
    STRING,
    EXPRESSION
};

class CSpreadsheet;

class CCell {
public:
    CCell(const CCell& src);
    CCell& operator=(const CCell & src);
    ~CCell();

    explicit CCell(const string &contents);

    CValue getValue(CSpreadsheet &spreadsheet);

private:

    CValue m_value;
    CellType m_type;
    CASTNode * m_root;
    pair<int, int> m_shift;
};

#endif //BARDANIK_CCELL_H
