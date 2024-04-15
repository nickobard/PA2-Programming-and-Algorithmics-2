//
// Created by bardanik on 11/04/24.
//

#include "CSpreadsheet.h"


bool CSpreadsheet::load(istream &is) {
    CSpreadsheet sheet;
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
//        unsigned int row_shift = stoul(words[2]);
//        unsigned int col_shift = stoul(words[3]);

            sheet.setCell(CPos(row_pos, col_pos), value);
        }
    }

    m_cells = sheet.m_cells;
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

