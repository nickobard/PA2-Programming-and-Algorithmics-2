//
// Created by bardanik on 11/04/24.
//

#include "CSpreadsheet.h"

CSpreadsheet::CSpreadsheet() : m_rows(INITIAL_TABLE_SIZE), m_cols(INITIAL_TABLE_SIZE) {
    m_cells.resize(m_rows);
    for (auto &row: m_cells) {
        row.resize(m_cols);
    }
}

bool CSpreadsheet::load(istream &is) {
    return false;
}

bool CSpreadsheet::save(ostream &os) const {
    return false;
}

bool CSpreadsheet::setCell(CPos pos, string contents) {
    auto [row, col] = pos.getCoords();
    if (m_rows <= row) {
        m_cells.resize((row / 100 + 1) * 100);
        m_rows = m_cells.size();
    }

    if (m_cols <= col) {
        size_t new_cols_size = (col / 100 + 1) * 100;
        for (auto &table_row: m_cells) {
            table_row.resize(new_cols_size);
        }
        m_cols = new_cols_size;
    }

    m_cells[row][col] = CCell(contents);
    return false;
}

CValue CSpreadsheet::getValue(CPos pos) {
    return CValue();
}

void CSpreadsheet::copyRect(CPos dst, CPos src, int w, int h) {

}
