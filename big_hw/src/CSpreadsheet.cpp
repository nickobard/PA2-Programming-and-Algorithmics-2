//
// Created by bardanik on 11/04/24.
//

#include "CSpreadsheet.h"

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
    Cells cells_shifted_copy = copyCellsAndShift(src, dst, w, h);
    deleteCells(dst, w, h);
    pasteCells(cells_shifted_copy);
}

Cells CSpreadsheet::copyCellsAndShift(const CPos &src, const CPos &dst, int w, int h) {
    Cells cells_shifted_copy;
    auto [src_row, src_col] = src.getCoords();
    auto offset = CPos::getOffset(src, dst);

    auto row_beg = m_cells.lower_bound(src_row);
    auto row_end = m_cells.upper_bound(src_row + h - 1);

    while (row_beg != row_end) {
        auto col_beg = row_beg->second.lower_bound(src_col);
        auto col_end = row_beg->second.upper_bound(src_col + w - 1);
        while (col_beg != col_end) {
            auto cell_copy = shared_ptr<CCell>(col_beg->second->copy());
            cell_copy->shift(offset);

            CPos new_pos = {row_beg->first, col_beg->first};
            new_pos.shiftPos(offset);

            setCell(cells_shifted_copy, new_pos, cell_copy);

            col_beg++;
        }
        row_beg++;
    }
    return cells_shifted_copy;
}

void CSpreadsheet::deleteCells(const CPos &dst, int w, int h) {
    auto [dst_row, dst_col] = dst.getCoords();
    auto row_beg = m_cells.lower_bound(dst_row);
    auto row_end = m_cells.upper_bound(dst_row + h - 1);
    while (row_beg != row_end) {
        auto col_beg = row_beg->second.lower_bound(dst_col);
        auto col_end = row_beg->second.upper_bound(dst_col + w - 1);
        row_beg->second.erase(col_beg, col_end);
        if (row_beg->second.empty()) {
            row_beg = m_cells.erase(row_beg);
        } else {
            row_beg++;
        }
    }
}

void CSpreadsheet::pasteCells(const Cells &cells) {
    for (auto &row: cells) {
        auto row_pos = row.first;
        auto &columns = row.second;
        for (auto &col: columns) {
            auto col_pos = col.first;
            auto cell = col.second;
            setCell(m_cells, {row_pos, col_pos}, cell);
        }
    }
}

