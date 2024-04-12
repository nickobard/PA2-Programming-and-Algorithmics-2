//
// Created by bardanik on 11/04/24.
//

#ifndef BARDANIK_CCELL_H
#define BARDANIK_CCELL_H

#include "CPos.h"

using namespace literals;
using CValue = variant<monostate, double, string>;

class CCell {
public:
    CCell();
    CCell(const CPos &pos, const string &contents);
    pair<unsigned int, unsigned int> getCoords() const;

private:
    CPos m_pos;
    CValue m_value;
};

#endif //BARDANIK_CCELL_H
