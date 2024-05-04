//
// Created by bardanik on 11/04/24.
//
#include "CCell.h"

#include <utility>

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
        if (!contents.empty() && contents[0] == '=') {
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

CValue CCell::getValue(CSpreadsheet &spreadsheet, CCycleDetectionVisitor &visitor) {
    if (m_type == CellType::EXPRESSION) {
        if (m_root == nullptr) {
            try {
                CASTExpressionBuilder builder(spreadsheet, *this);
                parseExpression(get<string>(m_value), builder);
                m_root = builder.getResult();
            } catch (invalid_argument &e) {
                return m_value;
            }
        }
        visitor.visit(this);
        auto evaluation = m_root->evaluate(visitor);
        visitor.leave(this);
        return evaluation;
    }
    return m_value;
}

void CCell::shift(const pair<int, int> &shift) {
    m_shift.first += shift.first;
    m_shift.second += shift.second;
}

pair<int, int> CCell::getShift() const {
    return m_shift;
}



