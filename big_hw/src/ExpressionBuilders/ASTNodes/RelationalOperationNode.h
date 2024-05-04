//
// Created by bardanik on 04/05/24.
//

#ifndef PA2_BIG_TASK_RELATIONALOPERATIONNODE_H
#define PA2_BIG_TASK_RELATIONALOPERATIONNODE_H

#include "BinaryOperationNode.h"

class RelationalOperationNode : public BinaryOperationNode {
public:
    RelationalOperationNode(CASTNode *first_arg, CASTNode *second_arg);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;

protected:
    virtual bool compare(double lhs, double rhs) = 0;

    virtual bool compare(const string &lhs, const string &rhs) = 0;
};


class EqualNode : public RelationalOperationNode {
public:
    EqualNode(CASTNode *first_arg, CASTNode *second_arg);

private:
    bool compare(double lhs, double rhs) override;

    bool compare(const string &lhs, const string &rhs) override;
};


class NotEqualNode : public RelationalOperationNode {
public:
    NotEqualNode(CASTNode *first_arg, CASTNode *second_arg);

private:
    bool compare(double lhs, double rhs) override;

    bool compare(const string &lhs, const string &rhs) override;
};

class LessThanNode : public RelationalOperationNode {
public:
    LessThanNode(CASTNode *first_arg, CASTNode *second_arg);

private:
    bool compare(double lhs, double rhs) override;

    bool compare(const string &lhs, const string &rhs) override;
};

class GreaterThanNode : public RelationalOperationNode {
public:
    GreaterThanNode(CASTNode *first_arg, CASTNode *second_arg);

private:
    bool compare(double lhs, double rhs) override;

    bool compare(const string &lhs, const string &rhs) override;
};

class LessThanOrEqualNode : public RelationalOperationNode {
public:
    LessThanOrEqualNode(CASTNode *first_arg, CASTNode *second_arg);

private:
    bool compare(double lhs, double rhs) override;

    bool compare(const string &lhs, const string &rhs) override;
};

class GreaterThanOrEqualNode : public RelationalOperationNode {
public:
    GreaterThanOrEqualNode(CASTNode *first_arg, CASTNode *second_arg);

private:
    bool compare(double lhs, double rhs) override;

    bool compare(const string &lhs, const string &rhs) override;
};


#endif //PA2_BIG_TASK_RELATIONALOPERATIONNODE_H
