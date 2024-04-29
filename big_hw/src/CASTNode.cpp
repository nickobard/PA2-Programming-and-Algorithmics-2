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
bool BinaryOperationNode::typesAre(const pair<CValue, CValue> &values) const {
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

CValue UnaryOperationNode::evaluateValue() {
    return m_operand->evaluate();
}

CNumberNode::CNumberNode(double number) : m_number({number}) {

}

CValue CNumberNode::evaluate() {
    return m_number;
}

CValue AddNode::evaluate() {
    auto values = evaluateValues();
    if (typesAre<double, double>(values)) {
        auto [left, right] = getValues<double, double>(values);
        auto result = left + right;
        return result;
    }
    if (typesAre<double, string>(values)) {
        auto [left, right] = getValues<double, string>(values);
        auto result = to_string(left) + right;
        return result;
    }
    if (typesAre<string, double>(values)) {
        auto [left, right] = getValues<string, double>(values);
        auto result = left + to_string(right);
        return result;
    }
    if (typesAre<string, string>(values)) {
        auto [left, right] = getValues<string, string>(values);
        auto result = left + right;
        return result;
    }
    return {};
}

AddNode::AddNode(CASTNode *first_arg, CASTNode *second_arg) : BinaryOperationNode(first_arg, second_arg) {

}

CValue SubtractNode::evaluate() {
    auto values = evaluateValues();
    if (typesAre<double, double>(values)) {
        auto [left, right] = getValues<double, double>(values);
        auto result = left - right;
        return result;
    }
    return {};
}

SubtractNode::SubtractNode(CASTNode *first_arg, CASTNode *second_arg) : BinaryOperationNode(first_arg, second_arg) {

}

CValue MultiplicationNode::evaluate() {
    auto values = evaluateValues();
    if (typesAre<double, double>(values)) {
        auto [left, right] = getValues<double, double>(values);
        auto result = left * right;
        return result;
    }
    return {};
}

MultiplicationNode::MultiplicationNode(CASTNode *first_arg, CASTNode *second_arg) : BinaryOperationNode(first_arg,
                                                                                                        second_arg) {

}

CValue DivisionNode::evaluate() {
    auto values = evaluateValues();
    if (typesAre<double, double>(values)) {
        auto [left, right] = getValues<double, double>(values);
        if (right == 0) {
            return {};
        }
        auto result = left / right;
        return result;
    }
    return {};
}

DivisionNode::DivisionNode(CASTNode *first_arg, CASTNode *second_arg) : BinaryOperationNode(first_arg, second_arg) {

}

CValue PowerNode::evaluate() {
    auto values = evaluateValues();
    if (typesAre<double, double>(values)) {
        auto [left, right] = getValues<double, double>(values);
        auto result = pow(left, right);
        return result;
    }
    return {};
}

PowerNode::PowerNode(CASTNode *first_arg, CASTNode *second_arg) : BinaryOperationNode(first_arg, second_arg) {

}

CValue NegationNode::evaluate() {
    auto value = evaluateValue();
    if (holds_alternative<double>(value)) {
        auto operand = get<double>(value);
        auto result = -operand;
        return result;
    }
    return {};
}

NegationNode::NegationNode(CASTNode *arg) : UnaryOperationNode(arg) {

}


EqualNode::EqualNode(CASTNode *first_arg, CASTNode *second_arg) : BinaryOperationNode(first_arg, second_arg) {

}

CValue EqualNode::evaluate() {
    auto values = evaluateValues();
    if (typesAre<double, double>(values)) {
        auto [left, right] = getValues<double, double>(values);
        auto result = left == right;
        return {static_cast<double>(result)};
    }
    if (typesAre<string, string>(values)) {
        auto [left, right] = getValues<string, string>(values);
        auto result = left == right;
        return {static_cast<double>(result)};
    }
    return {};
}

LessThanNode::LessThanNode(CASTNode *first_arg, CASTNode *second_arg) : BinaryOperationNode(first_arg, second_arg) {

}

CValue LessThanNode::evaluate() {
    auto values = evaluateValues();
    if (typesAre<double, double>(values)) {
        auto [left, right] = getValues<double, double>(values);
        auto result = left < right;
        return {static_cast<double>(result)};
    }
    if (typesAre<string, string>(values)) {
        auto [left, right] = getValues<string, string>(values);
        auto result = left < right;
        return {static_cast<double>(result)};
    }
    return {};
}

NotEqualNode::NotEqualNode(CASTNode *first_arg, CASTNode *second_arg) : BinaryOperationNode(first_arg, second_arg) {

}

CValue NotEqualNode::evaluate() {
    auto values = evaluateValues();
    if (typesAre<double, double>(values)) {
        auto [left, right] = getValues<double, double>(values);
        auto result = left != right;
        return {static_cast<double>(result)};
    }
    if (typesAre<string, string>(values)) {
        auto [left, right] = getValues<string, string>(values);
        auto result = left != right;
        return {static_cast<double>(result)};
    }
    return {};
}

GreaterThanNode::GreaterThanNode(CASTNode *first_arg, CASTNode *second_arg) : BinaryOperationNode(first_arg,
                                                                                                  second_arg) {
}

CValue GreaterThanNode::evaluate() {
    auto values = evaluateValues();
    if (typesAre<double, double>(values)) {
        auto [left, right] = getValues<double, double>(values);
        auto result = left > right;
        return {static_cast<double>(result)};
    }
    if (typesAre<string, string>(values)) {
        auto [left, right] = getValues<string, string>(values);
        auto result = left > right;
        return {static_cast<double>(result)};
    }
    return {};
}

LessThanOrEqualNode::LessThanOrEqualNode(CASTNode *first_arg, CASTNode *second_arg) : BinaryOperationNode(first_arg,
                                                                                                          second_arg) {

}

CValue LessThanOrEqualNode::evaluate() {
    auto values = evaluateValues();
    if (typesAre<double, double>(values)) {
        auto [left, right] = getValues<double, double>(values);
        auto result = left <= right;
        return {static_cast<double>(result)};
    }
    if (typesAre<string, string>(values)) {
        auto [left, right] = getValues<string, string>(values);
        auto result = left <= right;
        return {static_cast<double>(result)};
    }
    return {};
}

GreaterThanOrEqualNode::GreaterThanOrEqualNode(CASTNode *first_arg, CASTNode *second_arg) : BinaryOperationNode(
        first_arg, second_arg) {

}

CValue GreaterThanOrEqualNode::evaluate() {
    auto values = evaluateValues();
    if (typesAre<double, double>(values)) {
        auto [left, right] = getValues<double, double>(values);
        auto result = left >= right;
        return {static_cast<double>(result)};
    }
    if (typesAre<string, string>(values)) {
        auto [left, right] = getValues<string, string>(values);
        auto result = left >= right;
        return {static_cast<double>(result)};
    }
    return {};
}

}
