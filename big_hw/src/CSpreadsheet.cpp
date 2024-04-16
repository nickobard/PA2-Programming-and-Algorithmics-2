//
// Created by bardanik on 11/04/24.
//

#include "CSpreadsheet.h"


bool CSpreadsheet::load(istream &is) {
    Cells cells;
    while (is) {
        vector<string> words;
        string current_word;
        while (is && words.size() < 5) {
            char c = (char) is.get();
            if (is.eof()) {
                break;
            }
            if (c == ',') {
                words.push_back(current_word);
                current_word.clear();
                continue;
            }
            current_word.push_back(c);
        }
        if (is.eof()) {
            break;
        }

        string value;
        size_t size = stoul(words[4]);
        for (size_t i = 0; i < size; i++) {
            char c = (char) is.get();
            value.push_back(c);
        }

        if (is.get() != ';') {
            return false;
        } else {
            unsigned int row_pos = stoul(words[0]);
            unsigned int col_pos = stoul(words[1]);
            unsigned int row_shift = stoul(words[2]);
            unsigned int col_shift = stoul(words[3]);
            CCell cell = CCell(value);
            cell.m_shift = {row_shift, col_shift};
            setCell(cells, CPos(row_pos, col_pos), cell);
        }
    }

    m_cells = cells;
    return true;
}

bool CSpreadsheet::save(ostream &os) const {
    for (auto &[row_pos, column]: m_cells) {
        for (auto &[col_pos, cell]: column) {
            os << to_string(row_pos) + ',' + to_string(col_pos) + ',';
            os << to_string(cell.m_shift.first) + ',' + to_string(cell.m_shift.second) + ',';
            string value;
            if (cell.m_type == CellType::DOUBLE) {
                value = to_string(get<double>(cell.m_value));
            } else {
                value = get<string>(cell.m_value);
            }
            os << to_string(value.size()) + ',' + value + ';' << flush;
        }
    }
    return true;
}

bool CSpreadsheet::setCell(CPos pos, string contents) {
    CCell cell = CCell(contents);
    return setCell(m_cells, pos, cell);
}


bool CSpreadsheet::setCell(Cells &cells, const CPos &pos, const CCell &cell) {
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
    return col_element->second.getValue(*this);
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
    auto row_end = m_cells.upper_bound(src_row + h);

    while (row_beg != row_end) {
        auto col_beg = row_beg->second.lower_bound(src_col);
        auto col_end = row_beg->second.upper_bound(src_col + w);
        while (col_beg != col_end) {
            CCell cell_copy = CCell(col_beg->second);
            cell_copy.setShift(offset);

            setCell(cells_shifted_copy, {row_beg->first, col_beg->first}, cell_copy);

            col_beg++;
        }
        row_beg++;
    }
    return cells_shifted_copy;
}

void CSpreadsheet::deleteCells(const CPos &dst, int w, int h) {
    auto [dst_row, dst_col] = dst.getCoords();
    auto row_beg = m_cells.lower_bound(dst_row);
    auto row_end = m_cells.upper_bound(dst_row + h);
    while (row_beg != row_end) {
        auto col_beg = row_beg->second.lower_bound(dst_col);
        auto col_end = row_beg->second.upper_bound(dst_col + w);
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
            auto &cell = col.second;
            setCell(m_cells, {row_pos, col_pos}, cell);
        }
    }
}


