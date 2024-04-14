//
// Created by bardanik on 14/04/24.
//
#include "CASTExpressionBuilder.h"

CASTExpressionBuilder::CASTExpressionBuilder(const CSpreadsheet &spreadsheet) : m_root(nullptr),
                                                                                m_spreadsheet(spreadsheet) {

}

CASTExpressionBuilder::~CASTExpressionBuilder() {
    delete m_root;
}

CValue CASTExpressionBuilder::evaluate() const {
    return m_root->evaluate();
}

void CASTExpressionBuilder::opAdd() {

}

void CASTExpressionBuilder::opSub() {

}

void CASTExpressionBuilder::opMul() {

}

void CASTExpressionBuilder::opDiv() {

}

void CASTExpressionBuilder::opPow() {

}

void CASTExpressionBuilder::opNeg() {

}

void CASTExpressionBuilder::opEq() {

}

void CASTExpressionBuilder::opNe() {

}

void CASTExpressionBuilder::opLt() {

}

void CASTExpressionBuilder::opLe() {

}

void CASTExpressionBuilder::opGt() {

}

void CASTExpressionBuilder::opGe() {

}

void CASTExpressionBuilder::valNumber(double val) {

}

void CASTExpressionBuilder::valString(string val) {

}

void CASTExpressionBuilder::valReference(string val) {

}

void CASTExpressionBuilder::valRange(string val) {

}

void CASTExpressionBuilder::funcCall(std::string fnName, int paramCount) {

}

