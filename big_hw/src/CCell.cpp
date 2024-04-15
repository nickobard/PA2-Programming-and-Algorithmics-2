//
// Created by bardanik on 11/04/24.
//
#include "CCell.h"

CCell::CCell(const string &contents) : m_root(nullptr) {
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

CCell::~CCell() {
    delete m_root;
}


