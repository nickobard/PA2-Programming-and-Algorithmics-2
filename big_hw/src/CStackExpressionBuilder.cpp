//
// Created by bardanik on 11/04/24.
//

#include "CStackExpressionBuilder.h"
#include "CSpreadsheet.h"

void CStackExpressionBuilder::opAdd() {
    CValue first_arg = m_expression_result.top();
    m_expression_result.pop();
    CValue second_arg = m_expression_result.top();
    m_expression_result.top();
    CValue result = get<double>(first_arg) + get<double>(second_arg);
    m_expression_result.push(result);
}

void CStackExpressionBuilder::opSub() {

}

void CStackExpressionBuilder::opMul() {
    CValue first_arg = m_expression_result.top();
    m_expression_result.pop();
    CValue second_arg = m_expression_result.top();
    m_expression_result.pop();
    CValue result = get<double>(first_arg) * get<double>(second_arg);
    m_expression_result.push(result);
}

void CStackExpressionBuilder::opDiv() {

}

void CStackExpressionBuilder::opPow() {

}

void CStackExpressionBuilder::opNeg() {

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

CValue CStackExpressionBuilder::getResult() const {
    return m_expression_result.top();
}
