//
// Created by bardanik on 04/05/24.
//

#include "RelationalOperationNode.h"


RelationalOperationNode::RelationalOperationNode(CASTNode *left_operand, CASTNode *right_operand) : BinaryOperationNode(
        left_operand, right_operand) {

}

CValue RelationalOperationNode::evaluate(CCycleDetectionVisitor &visitor) {
    auto values = evaluateValues(visitor);
    CValue result;
    if (typesAre<double, double>(values)) {
        auto [left, right] = getValues<double, double>(values);
        result = static_cast<double>(compare(left, right));
    } else if (typesAre<string, string>(values)) {
        auto [left, right] = getValues<string, string>(values);
        result = static_cast<double>(compare(left, right));
    }
    return result;
}


EqualNode::EqualNode(CASTNode *left_operand, CASTNode *right_operand) : RelationalOperationNode(left_operand, right_operand) {

}

bool EqualNode::compare(double lhs, double rhs) {
    return lhs == rhs;
}

bool EqualNode::compare(const string &lhs, const string &rhs) {
    return lhs == rhs;
}


LessThanNode::LessThanNode(CASTNode *left_operand, CASTNode *right_operand) : RelationalOperationNode(left_operand, right_operand) {

}

bool LessThanNode::compare(double lhs, double rhs) {
    return lhs < rhs;
}

bool LessThanNode::compare(const string &lhs, const string &rhs) {
    return lhs < rhs;
}


NotEqualNode::NotEqualNode(CASTNode *left_operand, CASTNode *right_operand) : RelationalOperationNode(left_operand, right_operand) {

}

bool NotEqualNode::compare(double lhs, double rhs) {
    return lhs != rhs;
}

bool NotEqualNode::compare(const string &lhs, const string &rhs) {
    return lhs != rhs;
}


GreaterThanNode::GreaterThanNode(CASTNode *left_operand, CASTNode *right_operand) : RelationalOperationNode(left_operand,
                                                                                                            right_operand) {
}

bool GreaterThanNode::compare(double lhs, double rhs) {
    return lhs > rhs;
}

bool GreaterThanNode::compare(const string &lhs, const string &rhs) {
    return lhs > rhs;
}


LessThanOrEqualNode::LessThanOrEqualNode(CASTNode *left_operand, CASTNode *right_operand) : RelationalOperationNode(left_operand,
                                                                                                                    right_operand) {

}

bool LessThanOrEqualNode::compare(double lhs, double rhs) {
    return lhs <= rhs;
}

bool LessThanOrEqualNode::compare(const string &lhs, const string &rhs) {
    return lhs <= rhs;
}


GreaterThanOrEqualNode::GreaterThanOrEqualNode(CASTNode *left_operand, CASTNode *right_operand) : RelationalOperationNode(
        left_operand, right_operand) {

}

bool GreaterThanOrEqualNode::compare(double lhs, double rhs) {
    return lhs >= rhs;
}

bool GreaterThanOrEqualNode::compare(const string &lhs, const string &rhs) {
    return lhs >= rhs;
}


