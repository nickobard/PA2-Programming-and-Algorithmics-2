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


SubtractNode::SubtractNode(CASTNode *first_arg, CASTNode *second_arg) : BinaryOperationNode(first_arg, second_arg) {

}

CValue SubtractNode::evaluate(CCycleDetectionVisitor &visitor) {
    auto values = evaluateValues(visitor);
    if (typesAre<double, double>(values)) {
        auto [left, right] = getValues<double, double>(values);
        auto result = left - right;
        return result;
    }
    return {};
}


MultiplicationNode::MultiplicationNode(CASTNode *first_arg, CASTNode *second_arg) : BinaryOperationNode(first_arg,
                                                                                                        second_arg) {

}


CValue MultiplicationNode::evaluate(CCycleDetectionVisitor &visitor) {
    auto values = evaluateValues(visitor);
    if (typesAre<double, double>(values)) {
        auto [left, right] = getValues<double, double>(values);
        auto result = left * right;
        return result;
    }
    return {};
}


DivisionNode::DivisionNode(CASTNode *first_arg, CASTNode *second_arg) : BinaryOperationNode(first_arg, second_arg) {

}


CValue DivisionNode::evaluate(CCycleDetectionVisitor &visitor) {
    auto values = evaluateValues(visitor);
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


PowerNode::PowerNode(CASTNode *first_arg, CASTNode *second_arg) : BinaryOperationNode(first_arg, second_arg) {

}

CValue PowerNode::evaluate(CCycleDetectionVisitor &visitor) {
    auto values = evaluateValues(visitor);
    if (typesAre<double, double>(values)) {
        auto [left, right] = getValues<double, double>(values);
        auto result = pow(left, right);
        return result;
    }
    return {};
}


EqualNode::EqualNode(CASTNode *first_arg, CASTNode *second_arg) : BinaryOperationNode(first_arg, second_arg) {

}

CValue EqualNode::evaluate(CCycleDetectionVisitor &visitor) {
    auto values = evaluateValues(visitor);
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

CValue LessThanNode::evaluate(CCycleDetectionVisitor &visitor) {
    auto values = evaluateValues(visitor);
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

CValue NotEqualNode::evaluate(CCycleDetectionVisitor &visitor) {
    auto values = evaluateValues(visitor);
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

CValue GreaterThanNode::evaluate(CCycleDetectionVisitor &visitor) {
    auto values = evaluateValues(visitor);
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

CValue LessThanOrEqualNode::evaluate(CCycleDetectionVisitor &visitor) {
    auto values = evaluateValues(visitor);
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

CValue GreaterThanOrEqualNode::evaluate(CCycleDetectionVisitor &visitor) {
    auto values = evaluateValues(visitor);
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

