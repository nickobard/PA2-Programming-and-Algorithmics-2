//
// Created by bardanik on 04/05/24.
//

#ifndef PA2_BIG_TASK_UNARYOPERATIONNODE_H
#define PA2_BIG_TASK_UNARYOPERATIONNODE_H

#include "CASTNode.h"

class UnaryOperationNode : public CASTNode {
public:
    explicit UnaryOperationNode(CASTNode *operand);

    ~UnaryOperationNode();

    CValue evaluateValue(CCycleDetectionVisitor &visitor);

protected:

private:
    CASTNode *m_operand;
};


class NegationNode : public UnaryOperationNode {
public:
    explicit NegationNode(CASTNode *arg);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;
};


#endif //PA2_BIG_TASK_UNARYOPERATIONNODE_H
