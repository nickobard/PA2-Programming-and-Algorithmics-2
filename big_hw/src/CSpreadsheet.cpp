//
// Created by bardanik on 11/04/24.
//

#include "CSpreadsheet.h"

CSpreadsheet::CSpreadsheet() {

}

bool CSpreadsheet::load(istream &is) {
    return false;
}

bool CSpreadsheet::save(ostream &os) const {
    return false;
}

bool CSpreadsheet::setCell(CPos pos, string contents) {
    m_cells.
    return false;
}

CValue CSpreadsheet::getValue(CPos pos) {
    return CValue();
}

void CSpreadsheet::copyRect(CPos dst, CPos src, int w, int h) {

}
