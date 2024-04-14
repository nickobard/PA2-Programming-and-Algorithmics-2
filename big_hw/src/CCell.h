//
// Created by bardanik on 11/04/24.
//

#ifndef BARDANIK_CCELL_H
#define BARDANIK_CCELL_H

#include "CPos.h"
#include "CASTNode.h"

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
    CCell() = default;
    ~CCell();

    explicit CCell(const string &contents);

    CValue getValue(const CSpreadsheet &spreadsheet);

private:

    CValue m_value;
    CellType m_type;
    CASTNode * m_root;
};

#endif //BARDANIK_CCELL_H
