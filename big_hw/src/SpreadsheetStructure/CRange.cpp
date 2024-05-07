//
// Created by bardanik on 05/05/24.
//

#include "../CSpreadsheet.h"
#include "CRange.h"


CRange::CRange(CSpreadsheet &spreadsheet) : m_spreadsheet(spreadsheet), m_selection({}), m_w(1), m_h(1) {

}

void CRange::select(const CPos &src, int w, int h) {
    m_selection_position = src;
    m_w = w, m_h = h;
    auto [row, col] = src.getCoords();

    auto row_beg = m_spreadsheet.getCells().lower_bound(row);
    auto row_end = m_spreadsheet.getCells().upper_bound(row + m_h - 1);

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
    auto [row_offset, col_offset] = CPos::getOffset(from, to);
    int h = row_offset + 1, w = col_offset + 1;
    select(from, w, h);

}

void CRange::paste(const CPos &dst) {
    deleteCells(dst);
    auto offset = CPos::getOffset(m_selection_position, dst);
    shiftSelection(offset);
    pasteCells();
}

void CRange::deleteCells(const CPos &dst) {
    auto [dst_row, dst_col] = dst.getCoords();
    auto row_beg = m_spreadsheet.getCells().lower_bound(dst_row);
    auto row_end = m_spreadsheet.getCells().upper_bound(dst_row + m_h - 1);
    while (row_beg != row_end) {
        auto col_beg = row_beg->second.lower_bound(dst_col);
        auto col_end = row_beg->second.upper_bound(dst_col + m_w - 1);
        row_beg->second.erase(col_beg, col_end);
        if (row_beg->second.empty()) {
            row_beg = m_spreadsheet.getCells().erase(row_beg);
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
        coords.first += offset.first;
        coords.second += offset.second;
    }
}

void CRange::pasteCells() {
    for (auto &[coords, cell]: m_selection) {
        auto [row, col] = coords;
        CSpreadsheet::setCell(m_spreadsheet.getCells(), CPos(row, col), cell);
    }
}

vector<CValue> CRange::evaluate(CCycleDetectionVisitor &visitor) {
    vector<CValue> values;
    for (auto &[coords, cell]: m_selection) {
        auto value = cell->getValue(m_spreadsheet, visitor);
        values.emplace_back(value);
    }
    return values;
}

pair<string, string> CRange::splitRange(const string &range) {
    string first, second;
    bool is_first_current = true;
    for (char c: range) {
        if (is_first_current) {
            if (c == ':') {
                is_first_current = false;
                continue;
            }
            first.push_back(c);
        } else {
            second.push_back(c);
        }
    }
    return {first, second};
}
