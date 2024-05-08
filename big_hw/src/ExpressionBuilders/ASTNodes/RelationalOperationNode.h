//
// Created by bardanik on 04/05/24.
//

#ifndef PA2_BIG_TASK_RELATIONALOPERATIONNODE_H
#define PA2_BIG_TASK_RELATIONALOPERATIONNODE_H

#include "BinaryOperationNode.h"

/**
 * Represents an abstract relational binary operation class,
 * which stores two operands and compare them.
 */
class RelationalOperationNode : public BinaryOperationNode {
public:
    /**
     * Constructs relational operation node with two operands
     * @param left_operand - left operand in operation.
     * @param right_operand - right operand in operation.
     */
    RelationalOperationNode(CASTNode *left_operand, CASTNode *right_operand);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;

protected:
    /**
     * Compares left operand and right operand.
     * @param lhs - double left operand.
     * @param rhs - double right operand.
     * @return true if comparison operation is true, for example if both operands are equal
     * in case of equals operation.
     */
    virtual bool compare(double lhs, double rhs) = 0;

    /**
     * Compares left operand and right operand.
     * @param lhs - string left operand.
     * @param rhs - string right operand.
     * @return true if comparison operation is true, for example if both operands are equal
     * in case of equals operation.
     */
    virtual bool compare(const string &lhs, const string &rhs) = 0;
};

/**
 * Represents equals (==) comparison between operands.
 */
class EqualNode : public RelationalOperationNode {
public:
    EqualNode(CASTNode *left_operand, CASTNode *right_operand);

private:
    bool compare(double lhs, double rhs) override;

    bool compare(const string &lhs, const string &rhs) override;
};

/**
 * Represents not equals (!=) comparison between operands.
 */
class NotEqualNode : public RelationalOperationNode {
public:
    NotEqualNode(CASTNode *left_operand, CASTNode *right_operand);

private:
    bool compare(double lhs, double rhs) override;

    bool compare(const string &lhs, const string &rhs) override;
};

/**
 * Represents less than (<) comparison between operands.
 */
class LessThanNode : public RelationalOperationNode {
public:
    LessThanNode(CASTNode *left_operand, CASTNode *right_operand);

private:
    bool compare(double lhs, double rhs) override;

    bool compare(const string &lhs, const string &rhs) override;
};

/**
 * Represents greater than (>) comparison between operands.
 */
class GreaterThanNode : public RelationalOperationNode {
public:
    GreaterThanNode(CASTNode *left_operand, CASTNode *right_operand);

private:
    bool compare(double lhs, double rhs) override;

    bool compare(const string &lhs, const string &rhs) override;
};

/**
 * Represents less than or equals (<=) comparison between operands.
 */
class LessThanOrEqualNode : public RelationalOperationNode {
public:
    LessThanOrEqualNode(CASTNode *left_operand, CASTNode *right_operand);

private:
    bool compare(double lhs, double rhs) override;

    bool compare(const string &lhs, const string &rhs) override;
};

/**
 * Represents greater than or equals (>=) comparison between operands.
 */
class GreaterThanOrEqualNode : public RelationalOperationNode {
public:
    GreaterThanOrEqualNode(CASTNode *left_operand, CASTNode *right_operand);

private:
    bool compare(double lhs, double rhs) override;

    bool compare(const string &lhs, const string &rhs) override;
};


#endif //PA2_BIG_TASK_RELATIONALOPERATIONNODE_H
