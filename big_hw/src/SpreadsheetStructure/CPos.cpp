//
// Created by bardanik on 11/04/24.
//
#include "CPos.h"

CPos::CPos() : m_row(0), m_absolute_row(false), m_col(0), m_absolute_col(false) {

}


CPos::CPos(string_view str) : m_row(0), m_absolute_row(false), m_col(0), m_absolute_col(false) {
    string upper_cased_pos = toUpperCase(str);
    splitPositionAndParse(upper_cased_pos);
}

CPos::CPos(int row, int col) : m_row(row), m_absolute_row(false), m_col(col), m_absolute_col(false) {

}

string CPos::toUpperCase(const string_view &str) {
    string lower_cased;
    for (char c: str) {
        lower_cased.push_back((char) toupper(c));
    }
    return lower_cased;
}


void CPos::splitPositionAndParse(const string &position) {
    string label, number;

    bool readingColumnLabel = true;

    for (size_t i = 0; i < position.size(); i++) {
        char c = position[i];
        if (readingColumnLabel) {
            if (i == 0 && c == '$') {
                m_absolute_col = true;
            } else if (isalpha(c)) {
                label.push_back(c);
            } else if (isdigit(c)) {
                readingColumnLabel = false;
                number.push_back(c);
            } else if (!label.empty() && c == '$') {
                readingColumnLabel = false;
                m_absolute_row = true;
            } else {
                throw invalid_argument("Non valid character in column label.");
            }
        } else {
            if (!isdigit(c)) {
                throw invalid_argument("Non valid character in row number.");
            }
            number.push_back(c);
        }
    }

    m_col_label = label;
    m_row = stoi(number);
    m_col = convertColLabelToNumber(m_col_label);
}

int CPos::convertColLabelToNumber(const string &col_label) {
    int result = 0;
    for (char c: col_label) {
        result *= 26;
        result += (c - 'A' + 1);
    }
    result -= 1; // correction for indexing from 0
    return result;
}

pair<int, int> CPos::getCoords() const {
    return {m_row, m_col};
}

pair<int, int> CPos::getOffset(const CPos &src, const CPos &dst) {
    return {dst.m_row - src.m_row, dst.m_col - src.m_col};
}

void CPos::shift(const pair<int, int> &offset) {
    auto [row_shift, col_shift] = offset;
    if (!m_absolute_row) {
        m_row += row_shift;
    }
    if (!m_absolute_col) {
        m_col += col_shift;
    }
}


