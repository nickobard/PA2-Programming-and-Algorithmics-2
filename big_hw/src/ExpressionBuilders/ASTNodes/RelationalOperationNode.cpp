//
// Created by bardanik on 04/05/24.
//

#include "RelationalOperationNode.h"


RelationalOperationNode::RelationalOperationNode(CASTNode *first_arg, CASTNode *second_arg) : BinaryOperationNode(
        first_arg, second_arg) {

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


EqualNode::EqualNode(CASTNode *first_arg, CASTNode *second_arg) : RelationalOperationNode(first_arg, second_arg) {

}

bool EqualNode::compare(double lhs, double rhs) {
    return lhs == rhs;
}

bool EqualNode::compare(const string &lhs, const string &rhs) {
    return lhs == rhs;
}


LessThanNode::LessThanNode(CASTNode *first_arg, CASTNode *second_arg) : RelationalOperationNode(first_arg, second_arg) {

}

bool LessThanNode::compare(double lhs, double rhs) {
    return lhs < rhs;
}

bool LessThanNode::compare(const string &lhs, const string &rhs) {
    return lhs < rhs;
}


NotEqualNode::NotEqualNode(CASTNode *first_arg, CASTNode *second_arg) : RelationalOperationNode(first_arg, second_arg) {

}

bool NotEqualNode::compare(double lhs, double rhs) {
    return lhs != rhs;
}

bool NotEqualNode::compare(const string &lhs, const string &rhs) {
    return lhs != rhs;
}


GreaterThanNode::GreaterThanNode(CASTNode *first_arg, CASTNode *second_arg) : RelationalOperationNode(first_arg,
                                                                                                      second_arg) {
}

bool GreaterThanNode::compare(double lhs, double rhs) {
    return lhs > rhs;
}

bool GreaterThanNode::compare(const string &lhs, const string &rhs) {
    return lhs > rhs;
}


LessThanOrEqualNode::LessThanOrEqualNode(CASTNode *first_arg, CASTNode *second_arg) : RelationalOperationNode(first_arg,
                                                                                                              second_arg) {

}

bool LessThanOrEqualNode::compare(double lhs, double rhs) {
    return lhs <= rhs;
}

bool LessThanOrEqualNode::compare(const string &lhs, const string &rhs) {
    return lhs <= rhs;
}


GreaterThanOrEqualNode::GreaterThanOrEqualNode(CASTNode *first_arg, CASTNode *second_arg) : RelationalOperationNode(
        first_arg, second_arg) {

}

bool GreaterThanOrEqualNode::compare(double lhs, double rhs) {
    return lhs >= rhs;
}

bool GreaterThanOrEqualNode::compare(const string &lhs, const string &rhs) {
    return lhs >= rhs;
}


