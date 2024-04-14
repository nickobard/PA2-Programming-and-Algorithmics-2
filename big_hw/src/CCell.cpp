//
// Created by bardanik on 11/04/24.
//
#include "CCell.h"
#include "ExpressionParser.h"
#include "CStackExpressionBuilder.h"

CCell::CCell(const string &contents) {
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

CValue CCell::getValue(const CSpreadsheet &spreadsheet) const {
    if (m_type == CellType::EXPRESSION) {
        try {
            CStackExpressionBuilder builder(spreadsheet);
            parseExpression(get<string>(m_value), builder);
            return builder.evaluate();
        } catch (invalid_argument &e) {
            return m_value;
        }
    }
    return m_value;
}


