//
// Created by bardanik on 11/04/24.
//

#include "CSpreadsheet.h"


bool CSpreadsheet::load(istream &is) {
    Cells cells;
    string line;
    while (getline(is, line)) {sa
        vector<string> words;
        string current_word;
        for (size_t i = 0; i < line.size(); i++) {
            if (words.size() == 5) {
                auto size = (size_t) stoul(words[4]);

            } else {
                char current = line[i];
                if (current == ';') {
                    words.push_back(current_word);
                    current_word.clear();
                    continue;
                }
                current_word += current;
            }
        }

    }
    return false;
}

bool CSpreadsheet::save(ostream &os) const {
    for (auto &[row_pos, column]: m_cells) {
        for (auto &[col_pos, cell]: column) {
            os << to_string(row_pos) + ',' + to_string(col_pos) + ',';
            os << cell.m_shift.first + ',' + cell.m_shift.second + ',';
            string value;
            if (cell.m_type == CellType::DOUBLE) {
                value = to_string(get<double>(cell.m_value));
            } else {
                value = get<string>(cell.m_value);
            }
            os << to_string(value.size()) + ',' + value + ';' << endl;
        }
    }
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

