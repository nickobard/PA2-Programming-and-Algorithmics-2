//
// Created by bardanik on 11/04/24.
//
#include "CPos.h"

string toUpperCase(const string_view &str) {
    string lower_cased;
    for (char c: str) {
        lower_cased.push_back((char) tolower(c));
    }
    return lower_cased;
}


CPos::CPos(string_view str) {
    string upper_cased_pos = toUpperCase(str);
    auto [col_label, row_number] = splitPosition(upper_cased_pos);
    m_col_label = col_label;
    m_row = row_number;
    m_col = convertColLabelToNumber(m_col_label);
}

pair<string, unsigned int> CPos::splitPosition(const string &position) {
    string label;
    string number;

    bool readingColumnLabel = true;

    for (char c: position) {
        if (readingColumnLabel) {
            if (isalpha(c)) {
                label.push_back(c);
            } else if (isdigit(c)) {
                readingColumnLabel = false;
                number.push_back(c);
            } else {
                throw invalid_argument("Non alphabetic character in column label.");
            }
        } else {
            if (!isdigit(c)) {
                throw invalid_argument("Non digit character in row number.");
            }
        }
    }
    return {label, stoul(number)};
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
