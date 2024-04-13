//
// Created by bardanik on 11/04/24.
//
#include "CCell.h"

CCell::CCell(const string &contents) {
    try {
        double number = stod(contents);
        m_value = number;
    } catch (invalid_argument &e) {
        m_value = contents;
    }
}

CValue CCell::getValue() const {
    return m_value;
}


