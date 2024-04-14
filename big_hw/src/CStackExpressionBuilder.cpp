//
// Created by bardanik on 11/04/24.
//

#include "CStackExpressionBuilder.h"
#include "CSpreadsheet.h"

CStackExpressionBuilder::CStackExpressionBuilder(CSpreadsheet &spreadsheet) : m_spreadsheet(spreadsheet) {}

void CStackExpressionBuilder::opAdd() {
    auto [first, second] = getTwoDoubleArgumentsAndPop();
    auto result = first + second;
    m_expression_result.emplace(result);
}

void CStackExpressionBuilder::opSub() {
    auto [first, second] = getTwoDoubleArgumentsAndPop();
    auto result = second - first;
    m_expression_result.emplace(result);
}

void CStackExpressionBuilder::opMul() {
    auto [first, second] = getTwoDoubleArgumentsAndPop();
    auto result = first * second;
    m_expression_result.emplace(result);
}

void CStackExpressionBuilder::opDiv() {
    auto [first, second] = getTwoDoubleArgumentsAndPop();
    auto result = second / first;
    m_expression_result.emplace(result);
}

void CStackExpressionBuilder::opPow() {
    auto [first, second] = getTwoDoubleArgumentsAndPop();
    auto result = pow(second, first);
    m_expression_result.emplace(result);
}

void CStackExpressionBuilder::opNeg() {
    auto arg = getTopAndPop();
    auto result = -arg;
    m_expression_result.emplace(result);
}

void CStackExpressionBuilder::opEq() {

}

void CStackExpressionBuilder::opNe() {

}

void CStackExpressionBuilder::opLt() {

}

void CStackExpressionBuilder::opLe() {

}

void CStackExpressionBuilder::opGt() {

}

void CStackExpressionBuilder::opGe() {

}

void CStackExpressionBuilder::valNumber(double val) {
    m_expression_result.emplace(val);
}

void CStackExpressionBuilder::valString(string val) {
    m_expression_result.emplace(val);
}

void CStackExpressionBuilder::valReference(string val) {
    CValue value = m_spreadsheet.getValue(CPos(val));
    m_expression_result.push(value);
}

void CStackExpressionBuilder::valRange(string val) {

}

void CStackExpressionBuilder::funcCall(std::string fnName, int paramCount) {

}

CValue CStackExpressionBuilder::evaluate() {
    return m_expression_result.top();
}

pair<double, double> CStackExpressionBuilder::getTwoDoubleArgumentsAndPop() {
    double first_arg = get<double>(m_expression_result.top());
    m_expression_result.pop();
    double second_arg = get<double>(m_expression_result.top());
    m_expression_result.pop();
    return {first_arg, second_arg};

}

double CStackExpressionBuilder::getTopAndPop() {
    double top = get<double>(m_expression_result.top());
    m_expression_result.pop();
    return top;
}


