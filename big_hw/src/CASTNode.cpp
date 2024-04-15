//
// Created by bardanik on 14/04/24.
//

#include "CSpreadsheet.h"
#include "CASTNode.h"


CReferenceNode::CReferenceNode(const string &pos, CSpreadsheet &spreadsheet) : m_reference_position(CPos(pos)),
                                                                               m_spreadsheet(spreadsheet) {

}

CValue CReferenceNode::evaluate() {
    auto value = m_spreadsheet.getValue(m_reference_position);
    // TODO check if value is defined
    return value;
}

CStringNode::CStringNode(const string &parsed_value) : m_value({parsed_value}) {

}

CValue CStringNode::evaluate() {
    return m_value;
}

BinaryOperationNode::BinaryOperationNode(CASTNode *left_operand, CASTNode *right_operand) : m_left_operand(
        left_operand), m_right_operand(right_operand) {

}

BinaryOperationNode::~BinaryOperationNode() {
    delete m_left_operand;
    delete m_right_operand;
}

pair<double, double> BinaryOperationNode::getDoubleValues() const {
    double left = get<double>(m_left_operand->evaluate());
    double right = get<double>(m_right_operand->evaluate());
    return {left, right};
}

UnaryOperationNode::UnaryOperationNode(CASTNode *operand) : m_operand(operand) {

}

UnaryOperationNode::~UnaryOperationNode() {
    delete m_operand;
}

double UnaryOperationNode::getDoubleOperand() const {
    return get<double>(m_operand->evaluate());
}

CNumberNode::CNumberNode(double number) : m_number({number}) {

}

CValue CNumberNode::evaluate() {
    return m_number;
}

CValue AddNode::evaluate() {
    auto [left, right] = getDoubleValues();
    auto result = left + right;
    return result;
}

AddNode::AddNode(CASTNode *first_arg, CASTNode *second_arg) : BinaryOperationNode(first_arg, second_arg) {

}

CValue SubtractNode::evaluate() {
    auto [left, right] = getDoubleValues();
    auto result = left - right;
    return result;
}

SubtractNode::SubtractNode(CASTNode *first_arg, CASTNode *second_arg) : BinaryOperationNode(first_arg, second_arg) {

}

CValue MultiplicationNode::evaluate() {
    auto [left, right] = getDoubleValues();
    auto result = left * right;
    return result;
}

MultiplicationNode::MultiplicationNode(CASTNode *first_arg, CASTNode *second_arg) : BinaryOperationNode(first_arg,
                                                                                                        second_arg) {

}

CValue DivisionNode::evaluate() {
    auto [left, right] = getDoubleValues();
    auto result = left / right;
    return result;
}

DivisionNode::DivisionNode(CASTNode *first_arg, CASTNode *second_arg) : BinaryOperationNode(first_arg, second_arg) {

}

CValue PowerNode::evaluate() {
    auto [left, right] = getDoubleValues();
    auto result = pow(left, right);
    return result;
}

PowerNode::PowerNode(CASTNode *first_arg, CASTNode *second_arg) : BinaryOperationNode(first_arg, second_arg) {

}

CValue NegationNode::evaluate() {
    auto operand = getDoubleOperand();
    auto result = -operand;
    return result;
}

NegationNode::NegationNode(CASTNode *arg) : UnaryOperationNode(arg) {

}
