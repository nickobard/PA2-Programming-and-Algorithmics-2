//
// Created by bardanik on 11/04/24.
//

#include "CStackExpressionBuilder.h"

void CStackExpressionBuilder::opAdd() {

}

void CStackExpressionBuilder::opSub() {

}

void CStackExpressionBuilder::opMul() {

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

}

void CStackExpressionBuilder::valRange(string val) {

}

void CStackExpressionBuilder::funcCall(std::string fnName, int paramCount) {

}

CValue CStackExpressionBuilder::getResult() const {
    return m_expression_result.top();
}
