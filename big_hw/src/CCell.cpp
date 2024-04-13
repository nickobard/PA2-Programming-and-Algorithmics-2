//
// Created by bardanik on 11/04/24.
//
#include "CCell.h"
#include "../expression.h"
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

CValue CCell::getValue() const {
    if (m_type == CellType::EXPRESSION) {
        try {
            CStackExpressionBuilder builder;
            parseExpression(get<string>(m_value), builder);
            return m_value;
        } catch (invalid_argument &e) {
            if (isQuoteExpression()) {
                return evaluateQuoteExpression();
            }
            return m_value;
        }
    }
    return m_value;
}

string CCell::reduceQuotationMarks(const string &contents) {
    string reduced;
    bool opened_quote = false;
    for (char c: contents) {
        if (c == '\"' && opened_quote) {
            opened_quote = false;
            continue;
        } else {
            opened_quote = true;
        }
        reduced.push_back(c);
    }
    return reduced;
}

CValue CCell::evaluateQuoteExpression() const {
    string content = get<string>(m_value);
    string reduced = reduceQuotationMarks(content.substr(3, content.size() - 5));
    return {reduced};
}

bool CCell::isQuoteExpression() const {
    try {
        string content = get<string>(m_value);
        if (content.at(1) != '\\' || content.at(2) != '\"') {
            return false;
        }
        if (content.at(content.size() - 2) != '\\' || content.at(content.size() - 1) != '\"') {
            return false;
        }
    } catch (out_of_range &e) {
        return false;
    }
    return true;
}


