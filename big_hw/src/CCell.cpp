//
// Created by bardanik on 11/04/24.
//
#include "CCell.h"


CCell::CCell(const CCell &src) : m_value(src.m_value), m_type(src.m_type), m_root(nullptr), m_shift(src.m_shift) {

}

CCell &CCell::operator=(const CCell &other) {
    if (this == &other) {
        return *this;
    }
    delete m_root;
    m_root = nullptr;
    m_value = other.m_value;
    m_type = other.m_type;
    m_shift = other.m_shift;
    return *this;
}

CCell::CCell(const string &contents) : m_root(nullptr), m_shift({0, 0}) {
    try {
        double number = stod(contents);
        m_value = number;
        m_type = CellType::DOUBLE;
    } catch (invalid_argument &e) {
        if (!contents.empty() && contents.starts_with('=')) {
            m_type = CellType::EXPRESSION;
        } else {
            m_type = CellType::STRING;
        }
        m_value = contents;
    }
}


CCell::~CCell() {
    delete m_root;
}

CValue CCell::getValue(CSpreadsheet &spreadsheet) {
    if (m_type == CellType::EXPRESSION) {
        if (m_root == nullptr) {
            try {
                CASTExpressionBuilder builder(spreadsheet);
                parseExpression(get<string>(m_value), builder);
                m_root = builder.getResult();
            } catch (invalid_argument &e) {
                return m_value;
            }
        }
        return m_root->evaluate();
    }
    return m_value;
}



