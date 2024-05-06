//
// Created by bardanik on 04/05/24.
//

#include "BinaryOperationNode.h"

BinaryOperationNode::BinaryOperationNode(CASTNode *left_operand, CASTNode *right_operand) : m_left_operand(
        left_operand), m_right_operand(right_operand) {

}

BinaryOperationNode::~BinaryOperationNode() {
    delete m_left_operand;
    delete m_right_operand;
}

pair<CValue, CValue> BinaryOperationNode::evaluateValues(CCycleDetectionVisitor &visitor) {
    return {m_left_operand->evaluate(visitor), m_right_operand->evaluate(visitor)};
}

template<typename L, typename R>
bool BinaryOperationNode::typesAre(const CValue &first, const CValue &second) {
    return holds_alternative<L>(first) && holds_alternative<R>(second);
}

template<typename L, typename R>
bool BinaryOperationNode::typesAre(const pair<CValue, CValue> &values) {
    return typesAre<L, R>(values.first, values.second);
}

template<typename L, typename R>
pair<L, R> BinaryOperationNode::getValues(const CValue &first, const CValue &second) {
    L left = get<L>(first);
    R right = get<R>(second);
    return {left, right};
}


template<typename L, typename R>
pair<L, R> BinaryOperationNode::getValues(const pair<CValue, CValue> &values) {
    return getValues<L, R>(values.first, values.second);
}


AddNode::AddNode(CASTNode *first_arg, CASTNode *second_arg) : BinaryOperationNode(first_arg, second_arg) {

}

CValue AddNode::evaluate(CCycleDetectionVisitor &visitor) {
    auto values = evaluateValues(visitor);
    CValue result;
    if (typesAre<double, double>(values)) {
        auto [left, right] = getValues<double, double>(values);
        result = left + right;
    } else if (typesAre<double, string>(values)) {
        auto [left, right] = getValues<double, string>(values);
        result = to_string(left) + right;
    } else if (typesAre<string, double>(values)) {
        auto [left, right] = getValues<string, double>(values);
        result = left + to_string(right);
    } else if (typesAre<string, string>(values)) {
        auto [left, right] = getValues<string, string>(values);
        result = left + right;
    }
    return result;
}


SubtractNode::SubtractNode(CASTNode *first_arg, CASTNode *second_arg) : BinaryOperationNode(first_arg, second_arg) {

}

CValue SubtractNode::evaluate(CCycleDetectionVisitor &visitor) {
    auto values = evaluateValues(visitor);
    CValue result;
    if (typesAre<double, double>(values)) {
        auto [left, right] = getValues<double, double>(values);
        result = left - right;
    }
    return result;
}


MultiplicationNode::MultiplicationNode(CASTNode *first_arg, CASTNode *second_arg) : BinaryOperationNode(first_arg,
                                                                                                        second_arg) {

}


CValue MultiplicationNode::evaluate(CCycleDetectionVisitor &visitor) {
    auto values = evaluateValues(visitor);
    CValue result;
    if (typesAre<double, double>(values)) {
        auto [left, right] = getValues<double, double>(values);
        result = left * right;
    }
    return result;
}


DivisionNode::DivisionNode(CASTNode *first_arg, CASTNode *second_arg) : BinaryOperationNode(first_arg, second_arg) {

}


CValue DivisionNode::evaluate(CCycleDetectionVisitor &visitor) {
    auto values = evaluateValues(visitor);
    CValue result;
    if (typesAre<double, double>(values)) {
        auto [left, right] = getValues<double, double>(values);
        if (right == 0) {
            return {};
        }
        result = left / right;
    }
    return result;
}


PowerNode::PowerNode(CASTNode *first_arg, CASTNode *second_arg) : BinaryOperationNode(first_arg, second_arg) {

}

CValue PowerNode::evaluate(CCycleDetectionVisitor &visitor) {
    auto values = evaluateValues(visitor);
    CValue result;
    if (typesAre<double, double>(values)) {
        auto [left, right] = getValues<double, double>(values);
        result = pow(left, right);
    }
    return result;
}

