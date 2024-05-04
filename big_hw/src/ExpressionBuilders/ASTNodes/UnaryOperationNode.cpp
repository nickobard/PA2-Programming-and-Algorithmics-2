//
// Created by bardanik on 04/05/24.
//

#include "UnaryOperationNode.h"

UnaryOperationNode::UnaryOperationNode(CASTNode *operand) : m_operand(operand) {

}

UnaryOperationNode::~UnaryOperationNode() {
    delete m_operand;
}

CValue UnaryOperationNode::evaluateValue(CCycleDetectionVisitor &visitor) {
    return m_operand->evaluate(visitor);
}


NegationNode::NegationNode(CASTNode *arg) : UnaryOperationNode(arg) {

}



CValue NegationNode::evaluate(CCycleDetectionVisitor &visitor) {
    auto value = evaluateValue(visitor);
    if (holds_alternative<double>(value)) {
        auto operand = get<double>(value);
        auto result = -operand;
        return result;
    }
    return {};
}