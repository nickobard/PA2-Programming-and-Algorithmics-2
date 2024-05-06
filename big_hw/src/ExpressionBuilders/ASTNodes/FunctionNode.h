//
// Created by bardanik on 06/05/24.
//

#ifndef PA2_BIG_TASK_FUNCTIONNODE_H
#define PA2_BIG_TASK_FUNCTIONNODE_H

#include "CASTNode.h"

class FunctionNode : public CASTNode {
public:
    template<typename... Args>
    explicit FunctionNode(Args... args);

    ~FunctionNode();

protected:
    vector<CASTNode *> m_args;
};

class SumNode : public FunctionNode {
public:
    explicit SumNode(CASTNode *m_range);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;
};


class CountNode : public FunctionNode {
public:
    explicit CountNode(CASTNode *m_range);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;

};


class MinNode : public FunctionNode {
public:
    explicit MinNode(CASTNode *m_range);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;
};


class MaxNode : public FunctionNode {
public:
    explicit MaxNode(CASTNode *m_range);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;
};

class CountValNode : public FunctionNode {
public:
    explicit CountValNode(CASTNode *value, CASTNode *range);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;

};


class ConditionalNode : public FunctionNode {
public:
    explicit ConditionalNode(CASTNode *cond, CASTNode *if_true, CASTNode *if_false);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;
};


#endif //PA2_BIG_TASK_FUNCTIONNODE_H
