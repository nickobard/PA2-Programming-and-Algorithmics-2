//
// Created by bardanik on 14/04/24.
//
#include "../SpreadsheetStructure/CCell.h"

CASTExpressionBuilder::CASTExpressionBuilder(CSpreadsheet &spreadsheet, const CCell *current_cell) :
        m_spreadsheet(
                spreadsheet), m_cell(current_cell) {
}

CASTNode *CASTExpressionBuilder::getResult() {
    auto root = m_stack.top();
    m_stack.pop();
    return root;
}

void CASTExpressionBuilder::opAdd() {
    auto [first, second] = getNodesPairAndPop();
    CASTNode *node = new AddNode(first, second);
    m_stack.push(node);
}

void CASTExpressionBuilder::opSub() {
    auto [first, second] = getNodesPairAndPop();
    CASTNode *node = new SubtractNode(first, second);
    m_stack.push(node);
}

void CASTExpressionBuilder::opMul() {
    auto [first, second] = getNodesPairAndPop();
    CASTNode *node = new MultiplicationNode(first, second);
    m_stack.push(node);
}

void CASTExpressionBuilder::opDiv() {
    auto [first, second] = getNodesPairAndPop();
    CASTNode *node = new DivisionNode(first, second);
    m_stack.push(node);
}

void CASTExpressionBuilder::opPow() {
    auto [first, second] = getNodesPairAndPop();
    CASTNode *node = new PowerNode(first, second);
    m_stack.push(node);
}

void CASTExpressionBuilder::opNeg() {
    auto arg = m_stack.top();
    m_stack.pop();
    CASTNode *node = new NegationNode(arg);
    m_stack.push(node);
}

void CASTExpressionBuilder::opEq() {
    auto [first, second] = getNodesPairAndPop();
    CASTNode *node = new EqualNode(first, second);
    m_stack.push(node);
}

void CASTExpressionBuilder::opNe() {
    auto [first, second] = getNodesPairAndPop();
    CASTNode *node = new NotEqualNode(first, second);
    m_stack.push(node);
}

void CASTExpressionBuilder::opLt() {
    auto [first, second] = getNodesPairAndPop();
    CASTNode *node = new LessThanNode(first, second);
    m_stack.push(node);
}

void CASTExpressionBuilder::opLe() {
    auto [first, second] = getNodesPairAndPop();
    CASTNode *node = new LessThanOrEqualNode(first, second);
    m_stack.push(node);
}

void CASTExpressionBuilder::opGt() {
    auto [first, second] = getNodesPairAndPop();
    CASTNode *node = new GreaterThanNode(first, second);
    m_stack.push(node);
}

void CASTExpressionBuilder::opGe() {
    auto [first, second] = getNodesPairAndPop();
    CASTNode *node = new GreaterThanOrEqualNode(first, second);
    m_stack.push(node);
}

void CASTExpressionBuilder::valNumber(double val) {
    CASTNode *node = new CNumberNode(val);
    m_stack.push(node);
}

void CASTExpressionBuilder::valString(string val) {
    CASTNode *node = new CStringNode(val);
    m_stack.push(node);
}

void CASTExpressionBuilder::valReference(string val) {
    CASTNode *node = new CReferenceNode(val, m_spreadsheet, m_cell->getShift());
    m_stack.push(node);
}

void CASTExpressionBuilder::valRange(string val) {
}

void CASTExpressionBuilder::funcCall(std::string fnName, int paramCount) {
}

pair<CASTNode *, CASTNode *> CASTExpressionBuilder::getNodesPairAndPop() {
    auto *second_arg = m_stack.top();
    m_stack.pop();
    auto *first_arg = m_stack.top();
    m_stack.pop();
    return {first_arg, second_arg};
}

