//
// Created by bardanik on 11/04/24.
//
#include "CCell.h"

CCell::CCell(const CPos &pos, const string &contents) : m_pos(pos), m_value(contents) {

}

pair<unsigned int, unsigned int> CCell::getCoords() const {
    return m_pos.getCoords();
}

CCell::CCell() {

}
