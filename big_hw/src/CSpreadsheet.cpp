//
// Created by bardanik on 11/04/24.
//

#include "CSpreadsheet.h"


bool CSpreadsheet::load(istream &is) {
    return false;
}

bool CSpreadsheet::save(ostream &os) const {
    return false;
}

bool CSpreadsheet::setCell(CPos pos, string contents) {
    auto [row, col] = pos.getCoords();
    auto row_element = m_cells.find(row);
    if (row_element == m_cells.end()) {
        m_cells.insert({row, {{col, CCell(contents)}}});
        return true;
    }
    auto col_element = row_element->second.find(col);
    if (col_element == row_element->second.end()) {
        row_element->second.insert({col, CCell(contents)});
        return true;
    }
    col_element->second = CCell(contents);
    return true;
}

CValue CSpreadsheet::getValue(CPos pos) {
    auto [row, col] = pos.getCoords();
    auto row_element = m_cells.find(row);
    if (row_element == m_cells.end()) {
        return {};
    }
    auto col_element = row_element->second.find(col);
    if (col_element == row_element->second.end()) {
        return {};
    }
    return col_element->second.getValue(*this);
}

void CSpreadsheet::copyRect(CPos dst, CPos src, int w, int h) {

}

