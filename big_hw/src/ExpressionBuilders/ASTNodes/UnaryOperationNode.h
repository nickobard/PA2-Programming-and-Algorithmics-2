//
// Created by bardanik on 04/05/24.
//

#ifndef PA2_BIG_TASK_UNARYOPERATIONNODE_H
#define PA2_BIG_TASK_UNARYOPERATIONNODE_H

#include "CASTNode.h"

/**
 * Represents an abstract unary operation node class, to apply unary operation on stored operands.
 */
class UnaryOperationNode : public CASTNode {
public:
    /**
     * Constructs unary operation node with some given operand.
     * @param operand - operand to which to apply unary operation.
     */
    explicit UnaryOperationNode(CASTNode *operand);

    ~UnaryOperationNode();

    /**
     * Evaluates the stored operand in the unary operation node.
     * @param visitor - cycle detection visitor to watch cycles in evaluation process.
     * @return value of the stored operand.
     */
    CValue evaluateValue(CCycleDetectionVisitor &visitor);

protected:

private:
    // Operand on which operation is applied.
    CASTNode *m_operand;
};

/**
 * Applies negation operation on the stored operand.
 */
class NegationNode : public UnaryOperationNode {
public:
    explicit NegationNode(CASTNode *operand);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;
};


#endif //PA2_BIG_TASK_UNARYOPERATIONNODE_H
