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
bool BinaryOperationNode::typesAre(const pair<CValue, CValue> &values) const {
    return holds_alternative<L>(values.first) && holds_alternative<R>(values.second);
}


template<typename L, typename R>
pair<L, R> BinaryOperationNode::getValues(const pair<CValue, CValue> &values) const {
    L left = get<L>(values.first);
    R right = get<R>(values.second);
    return {left, right};
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


EqualNode::EqualNode(CASTNode *first_arg, CASTNode *second_arg) : BinaryOperationNode(first_arg, second_arg) {

}

CValue EqualNode::evaluate(CCycleDetectionVisitor &visitor) {
    auto values = evaluateValues(visitor);
    CValue result;
    if (typesAre<double, double>(values)) {
        auto [left, right] = getValues<double, double>(values);
        result = static_cast<double>(left == right);
    } else if (typesAre<string, string>(values)) {
        auto [left, right] = getValues<string, string>(values);
        result = static_cast<double>(left == right);
    }
    return result;
}

LessThanNode::LessThanNode(CASTNode *first_arg, CASTNode *second_arg) : BinaryOperationNode(first_arg, second_arg) {

}

CValue LessThanNode::evaluate(CCycleDetectionVisitor &visitor) {
    auto values = evaluateValues(visitor);
    CValue result;
    if (typesAre<double, double>(values)) {
        auto [left, right] = getValues<double, double>(values);
        result = static_cast<double>(left < right);
    } else if (typesAre<string, string>(values)) {
        auto [left, right] = getValues<string, string>(values);
        result = static_cast<double>(left < right);
    }
    return result;
}

NotEqualNode::NotEqualNode(CASTNode *first_arg, CASTNode *second_arg) : BinaryOperationNode(first_arg, second_arg) {

}

CValue NotEqualNode::evaluate(CCycleDetectionVisitor &visitor) {
    auto values = evaluateValues(visitor);
    CValue result;
    if (typesAre<double, double>(values)) {
        auto [left, right] = getValues<double, double>(values);
        result = static_cast<double>(left != right);
    } else if (typesAre<string, string>(values)) {
        auto [left, right] = getValues<string, string>(values);
        result = static_cast<double>(left != right);
    }
    return result;
}

GreaterThanNode::GreaterThanNode(CASTNode *first_arg, CASTNode *second_arg) : BinaryOperationNode(first_arg,
                                                                                                  second_arg) {
}

CValue GreaterThanNode::evaluate(CCycleDetectionVisitor &visitor) {
    auto values = evaluateValues(visitor);
    CValue result;
    if (typesAre<double, double>(values)) {
        auto [left, right] = getValues<double, double>(values);
        result = static_cast<double>(left > right);
    } else if (typesAre<string, string>(values)) {
        auto [left, right] = getValues<string, string>(values);
        result = static_cast<double>(left > right);
    }
    return result;
}

LessThanOrEqualNode::LessThanOrEqualNode(CASTNode *first_arg, CASTNode *second_arg) : BinaryOperationNode(first_arg,
                                                                                                          second_arg) {

}

CValue LessThanOrEqualNode::evaluate(CCycleDetectionVisitor &visitor) {
    auto values = evaluateValues(visitor);
    CValue result;
    if (typesAre<double, double>(values)) {
        auto [left, right] = getValues<double, double>(values);
        result = static_cast<double>(left <= right);
    } else if (typesAre<string, string>(values)) {
        auto [left, right] = getValues<string, string>(values);
        result = static_cast<double>(left <= right);
    }
    return result;
}

GreaterThanOrEqualNode::GreaterThanOrEqualNode(CASTNode *first_arg, CASTNode *second_arg) : BinaryOperationNode(
        first_arg, second_arg) {

}

CValue GreaterThanOrEqualNode::evaluate(CCycleDetectionVisitor &visitor) {
    auto values = evaluateValues(visitor);
    CValue result;
    if (typesAre<double, double>(values)) {
        auto [left, right] = getValues<double, double>(values);
        result = static_cast<double>(left >= right);
    } else if (typesAre<string, string>(values)) {
        auto [left, right] = getValues<string, string>(values);
        result = static_cast<double>(left >= right);
    }
    return {};
}

