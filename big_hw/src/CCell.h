//
// Created by bardanik on 11/04/24.
//

#ifndef BARDANIK_CCELL_H
#define BARDANIK_CCELL_H

#include "CPos.h"

using namespace literals;
using CValue = variant<monostate, double, string>;

enum class CellType{
    DOUBLE,
    STRING,
    EXPRESSION
};

class CCell {
public:
    CCell() = default;

    explicit CCell(const string &contents);

    CValue getValue() const;

private:

    CValue m_value;
    CellType m_type;

};

#endif //BARDANIK_CCELL_H
