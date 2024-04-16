//
// Created by bardanik on 14/04/24.
//

#include "CSpreadsheet.h"
#include "CASTNode.h"


CReferenceNode::CReferenceNode(const string &pos, CSpreadsheet &spreadsheet, const pair<int, int> &shift)
        : m_reference_position(CPos(pos)),
          m_spreadsheet(spreadsheet) {
    m_reference_position.shiftPos(shift);
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

pair<CValue, CValue> BinaryOperationNode::evaluateValues() {
    return {m_left_operand->evaluate(), m_right_operand->evaluate()};
}

template<typename L, typename R>
bool BinaryOperationNode::bothTypesAre(const pair<CValue, CValue> &values) const {
    return holds_alternative<L>(values.first) && holds_alternative<R>(values.second);
}


template<typename L, typename R>
pair<L, R> BinaryOperationNode::getValues(const pair<CValue, CValue> &values) const {
    L left = get<L>(values.first);
    R right = get<R>(values.second);
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
    auto [left, right] = getValues<double, double>(evaluateValues());
    auto result = left + right;
    return result;
}

AddNode::AddNode(CASTNode *first_arg, CASTNode *second_arg) : BinaryOperationNode(first_arg, second_arg) {

}

CValue SubtractNode::evaluate() {
    auto [left, right] = getValues<double, double>(evaluateValues());
    auto result = left - right;
    return result;
}

SubtractNode::SubtractNode(CASTNode *first_arg, CASTNode *second_arg) : BinaryOperationNode(first_arg, second_arg) {

}

CValue MultiplicationNode::evaluate() {
    auto [left, right] = getValues<double, double>(evaluateValues());
    auto result = left * right;
    return result;
}

MultiplicationNode::MultiplicationNode(CASTNode *first_arg, CASTNode *second_arg) : BinaryOperationNode(first_arg,
                                                                                                        second_arg) {

}

CValue DivisionNode::evaluate() {
    auto [left, right] = getValues<double, double>(evaluateValues());
    auto result = left / right;
    return result;
}

DivisionNode::DivisionNode(CASTNode *first_arg, CASTNode *second_arg) : BinaryOperationNode(first_arg, second_arg) {

}

CValue PowerNode::evaluate() {
    auto [left, right] = getValues<double, double>(evaluateValues());
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
