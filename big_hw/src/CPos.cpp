//
// Created by bardanik on 11/04/24.
//
#include "CPos.h"

string toUpperCase(const string_view &str) {
    string lower_cased;
    for (char c: str) {
        lower_cased.push_back((char) toupper(c));
    }
    return lower_cased;
}


CPos::CPos(string_view str) : m_row(0), m_absolute_row(false), m_col(0), m_absolute_col(false) {
    string upper_cased_pos = toUpperCase(str);
    splitPositionAndParse(upper_cased_pos);
}

CPos::CPos(unsigned int row, unsigned int col) : m_row(row), m_col(col) {

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
    m_row = stoul(number);
    m_col = convertColLabelToNumber(m_col_label);
}

unsigned int CPos::convertColLabelToNumber(const string &col_label) {
    unsigned int result = 0;
    for (char c: col_label) {
        result *= 26;
        result += (c - 'A' + 1);
    }
    result -= 1; // correction for indexing from 0
    return result;
}

pair<unsigned int, unsigned int> CPos::getCoords() const {
    return {m_row, m_col};
}

pair<unsigned int, unsigned int> CPos::getOffset(const CPos &src, const CPos &dst) {
    return {dst.m_row - src.m_row, dst.m_col - src.m_col};
}

void CPos::shiftPos(const pair<int, int> &shift) {
    auto [row_shift, col_shift] = shift;
    if (!m_absolute_row) {
        m_row += row_shift;
    }
    if (!m_absolute_col) {
        m_col += col_shift;
    }
}


