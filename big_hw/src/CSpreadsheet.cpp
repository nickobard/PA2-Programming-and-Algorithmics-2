//
// Created by bardanik on 11/04/24.
//

#include "CSpreadsheet.h"


CSpreadsheet::CSpreadsheet(const CSpreadsheet &src) {
    for (const auto &row_element: src.m_cells) {
        int row = row_element.first;
        for (const auto &col_element: row_element.second) {
            int col = col_element.first;
            shared_ptr<CCell> copy = shared_ptr<CCell>(col_element.second->copy());
            setCell(m_cells, {row, col}, copy);
        }
    }
}

CSpreadsheet &CSpreadsheet::operator=(CSpreadsheet src) {
    swap(m_cells, src.m_cells);
    return *this;
}


bool CSpreadsheet::load(istream &is) {
    CLoader loader(is);
    Cells loaded;
    bool ok = loader.load(loaded);
    if (!ok) {
        return false;
    }
    m_cells = loaded;
    return true;
}

bool CSpreadsheet::save(ostream &os) const {
    CLoader loader(os);
    return loader.save(m_cells);
}

bool CSpreadsheet::setCell(CPos pos, string contents) {
    CCell *cell = CCell::createCell(contents);
    auto to_set = shared_ptr<CCell>(cell);
    return setCell(m_cells, pos, to_set);
}


bool CSpreadsheet::setCell(Cells &cells, const CPos &pos, shared_ptr<CCell> &cell) {
    auto [row, col] = pos.getCoords();
    auto row_element = cells.find(row);
    if (row_element == cells.end()) {
        cells.insert({row, {{col, cell}}});
        return true;
    }
    auto col_element = row_element->second.find(col);
    if (col_element == row_element->second.end()) {
        row_element->second.insert({col, cell});
        return true;
    }
    col_element->second = cell;
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
    try {
        CCycleDetectionVisitor visitor;
        return col_element->second->getValue(*this, visitor);
    } catch (CCycleDetectedException &e) {
        return {};
    }
}


CValue CSpreadsheet::getValue(CPos pos, CCycleDetectionVisitor &visitor) {
    auto [row, col] = pos.getCoords();
    auto row_element = m_cells.find(row);
    if (row_element == m_cells.end()) {
        return {};
    }
    auto col_element = row_element->second.find(col);
    if (col_element == row_element->second.end()) {
        return {};
    }
    return col_element->second->getValue(*this, visitor);
}


void CSpreadsheet::copyRect(CPos dst, CPos src, int w, int h) {
    CRange range(*this);
    range.select(src, w, h);
    range.paste(dst);
}

Cells &CSpreadsheet::getCells() {
    return m_cells;
}


