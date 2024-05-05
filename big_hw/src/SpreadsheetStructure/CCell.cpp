//
// Created by bardanik on 11/04/24.
//
#include "CCell.h"

#include <utility>

CCell::CCell(CValue value) : m_value(std::move(value)) {

}

CCell *CCell::createCell(const string &contents) {
    try {
        double number = stod(contents);
        return new CNumberCell(number);
    } catch (invalid_argument &e) {
        if (!contents.empty() && contents[0] == '=') {
            return new CExprCell(contents);
        } else {
            return new CStringCell(contents);
        }
    }
}


CNumberCell::CNumberCell(double value) : CCell(value) {
}

CNumberCell::CNumberCell() : CCell(0.0) {

}

CStringCell::CStringCell(const string &value) : CCell(value) {

}

CStringCell::CStringCell() : CCell("") {

}


CExprCell::CExprCell(const string &expression) : CCell(expression), m_shift({0, 0}) {

}


CExprCell::CExprCell() : CCell(""), m_shift({0, 0}) {

}


CCell *CStringCell::copy() const {
    return new CStringCell(get<string>(m_value));
}


CCell *CNumberCell::copy() const {
    return new CNumberCell(get<double>(m_value));
}

CCell *CExprCell::copy() const {
    auto *copy = new CExprCell(get<string>(m_value));
    copy->m_shift = m_shift;
    return copy;
}

string CStringCell::toString() const {
    string type = to_string(CCellType::STRING);
    string value = get<string>(m_value);
    string size = to_string(value.size());
    return type + ',' + size + ',' + value + ';';
}

istream &CStringCell::readCell(istream &is) {
    char sep;
    streamsize cell_value_size = 0;
    string cell_value;

    is >> cell_value_size >> sep;

    cell_value.resize(cell_value_size);
    is.read(cell_value.data(), cell_value_size);

    is >> sep;

    m_value = cell_value;
    return is;

}


string CNumberCell::toString() const {
    string type = to_string(CCellType::NUMBER);
    string value = to_string(get<double>(m_value));
    string size = to_string(value.size());
    return type + ',' + value + ';';
}

istream &CNumberCell::readCell(istream &is) {
    char sep;
    double value;
    is >> value >> sep;
    m_value = value;
    return is;

}


string CExprCell::toString() const {
    string type = to_string(CCellType::EXPRESSION);
    string shift = to_string(m_shift.first) + ',' + to_string(m_shift.second);
    string value = get<string>(m_value);
    string size = to_string(value.size());
    return type + ',' + shift + ',' + size + ',' + value + ';';
}


CValue CCell::getValue(CSpreadsheet &spreadsheet, CCycleDetectionVisitor &visitor) {
    return m_value;
}


CValue CExprCell::getValue(CSpreadsheet &spreadsheet, CCycleDetectionVisitor &visitor) {
    if (m_root == nullptr) {
        try {
            CASTExpressionBuilder builder(spreadsheet, this);
            parseExpression(get<string>(m_value), builder);
            m_root = unique_ptr<CASTNode>(builder.getResult());
        } catch (invalid_argument &e) {
            return m_value;
        }
    }
    visitor.visit(this);
    auto evaluation = m_root->evaluate(visitor);
    visitor.leave(this);
    return evaluation;
}

void CCell::shift(const pair<int, int> &shift) {
}

void CExprCell::shift(const pair<int, int> &shift) {
    m_shift.first += shift.first;
    m_shift.second += shift.second;
}

istream &CExprCell::readCell(istream &is) {

    char sep;
    int shift_row = 0, shift_col = 0;
    streamsize cell_value_size = 0;
    string cell_value;

    is >> shift_row >> sep
       >> shift_col >> sep
       >> cell_value_size >> sep;

    cell_value.resize(cell_value_size);
    is.read(cell_value.data(), cell_value_size);

    is >> sep;

    m_value = cell_value;
    m_shift = {shift_row, shift_col};

    return is;
}

pair<int, int> CCell::getShift() const {
    return {0, 0};
}

istream &operator>>(istream &is, CCell *cell) {
    return cell->readCell(is);
}

pair<int, int> CExprCell::getShift() const {
    return m_shift;
}

