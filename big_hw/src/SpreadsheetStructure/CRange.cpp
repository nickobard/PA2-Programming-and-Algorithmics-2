//
// Created by bardanik on 05/05/24.
//

#include "../CSpreadsheet.h"

CRange::CRange(Cells &cells) : m_cells(cells), m_selection({}), m_w(1), m_h(1) {

}

void CRange::select(const CPos &src, int w, int h) {
    m_w = w, m_h = h;
    auto [row, col] = src.getCoords();

    auto row_beg = m_cells.lower_bound(row);
    auto row_end = m_cells.upper_bound(row + m_h - 1);

    while (row_beg != row_end) {
        auto col_beg = row_beg->second.lower_bound(col);
        auto col_end = row_beg->second.upper_bound(col + m_w - 1);
        while (col_beg != col_end) {
            pair<int, int> coords = {row_beg->first, col_beg->first};
            m_selection.emplace_back(coords, col_beg->second);
            col_beg++;
        }
        row_beg++;
    }

}

void CRange::select(const CPos &from, const CPos &to) {
    throw exception();
}

void CRange::paste(const CPos &dst) {
    deleteCells(dst);
    auto offset = CPos::getOffset(m_selection_position, dst);
    shiftSelection(offset);
    pasteCells();
}

void CRange::deleteCells(const CPos &dst) {
    auto [dst_row, dst_col] = dst.getCoords();
    auto row_beg = m_cells.lower_bound(dst_row);
    auto row_end = m_cells.upper_bound(dst_row + m_h - 1);
    while (row_beg != row_end) {
        auto col_beg = row_beg->second.lower_bound(dst_col);
        auto col_end = row_beg->second.upper_bound(dst_col + m_w - 1);
        row_beg->second.erase(col_beg, col_end);
        if (row_beg->second.empty()) {
            row_beg = m_cells.erase(row_beg);
        } else {
            row_beg++;
        }
    }
}

void CRange::shiftSelection(const pair<int, int> &offset) {
    for (auto &[coords, cell]: m_selection) {
        if (cell.unique()) {
            cell->shift(offset);
        } else {
            cell = shared_ptr<CCell>(cell->copy());
            cell->shift(offset);
        }
    }
}

void CRange::pasteCells() {
    for (auto &[coords, cell]: m_selection) {
        auto [row, col] = coords;
        CSpreadsheet::setCell(m_cells, CPos(row, col), cell);
    }
}

CValue CRange::sum() const {
    return CValue();
}

CValue CRange::count() const {
    return CValue();
}

CValue CRange::min() const {
    return CValue();
}

CValue CRange::max() const {
    return CValue();
}

CValue CRange::countval(const CValue &reference_value) const {
    return CValue();
}
