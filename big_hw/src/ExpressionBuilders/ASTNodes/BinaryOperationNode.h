// Created by bardanik on 04/05/24.
//

#ifndef PA2_BIG_TASK_BINARYOPERATIONNODE_H
#define PA2_BIG_TASK_BINARYOPERATIONNODE_H

#include <cmath>
#include "CASTNode.h"

class BinaryOperationNode : public CASTNode {
public:
    BinaryOperationNode(CASTNode *left_operand, CASTNode *right_operand);

    ~BinaryOperationNode();

protected:

    template<typename L, typename R>
    pair<L, R> getValues(const pair<CValue, CValue> &values) const;

    template<typename L, typename R>
    bool typesAre(const pair<CValue, CValue> &values) const;

    pair<CValue, CValue> evaluateValues(CCycleDetectionVisitor &visitor);

private:

    CASTNode *m_left_operand;
    CASTNode *m_right_operand;
};


class AddNode : public BinaryOperationNode {
public:
    AddNode(CASTNode *first_arg, CASTNode *second_arg);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;
};

class SubtractNode : public BinaryOperationNode {
public:
    SubtractNode(CASTNode *first_arg, CASTNode *second_arg);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;
};

class MultiplicationNode : public BinaryOperationNode {
public:
    MultiplicationNode(CASTNode *first_arg, CASTNode *second_arg);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;
};

class DivisionNode : public BinaryOperationNode {
public:
    DivisionNode(CASTNode *first_arg, CASTNode *second_arg);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;
};

class PowerNode : public BinaryOperationNode {
public:
    PowerNode(CASTNode *first_arg, CASTNode *second_arg);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;
};

#endif //PA2_BIG_TASK_BINARYOPERATIONNODE_H
