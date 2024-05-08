// Created by bardanik on 04/05/24.
//

#ifndef PA2_BIG_TASK_BINARYOPERATIONNODE_H
#define PA2_BIG_TASK_BINARYOPERATIONNODE_H

#include <cmath>
#include "CASTNode.h"

/**
 * Represents an abstract binary operation node class,
 * which stores two operands and applies binary operation defined in a derived class.
 */
class BinaryOperationNode : public CASTNode {
public:
    /**
     * Constructs binary operation node from two operands.
     * @param left_operand - left operand in operation.
     * @param right_operand - right operand in operation.
     */
    BinaryOperationNode(CASTNode *left_operand, CASTNode *right_operand);

    ~BinaryOperationNode();

    /**
     * Get values of specified type from the CValue.
     * Is used to extract certain types and values to make operations on them,
     * like double + double, etc.
     * @tparam L - left operand type.
     * @tparam R - right operand type.
     * @param values - CValue pair of values from which to extract values of specified types.
     * @return pair of values with specified type.
     */
    template<typename L, typename R>
    static pair<L, R> getValues(const pair<CValue, CValue> &values);

    /**
     * Same as getValues(const pair<CValue, CValue> &values), but with unpacked pair in params.
     */
    template<typename L, typename R>
    static pair<L, R> getValues(const CValue &first, const CValue &second);

    /**
     * Checks if CValues are of specified types. Is used to check types before extracting values
     * of specified types by getValues(...) functions.
     * @tparam L - left operand type.
     * @tparam R - right operand type.
     * @param values - pair of operands, first is left, second is right.
     * @return true if both values are of specified types, otherwise false.
     */
    template<typename L, typename R>
    static bool typesAre(const pair<CValue, CValue> &values);

    /**
     * Same as typesAre(const pair<CValue, CValue> &values), but with unpacked pair in params.
     */
    template<typename L, typename R>
    static bool typesAre(const CValue &first, const CValue &second);

protected:

    /**
     * Evaluates both operand nodes and returns a pair of their CValue evaluations.
     * @param visitor - to detect cycles in evaluation process.
     * @return pair of CValues, evaluated from both operands. First is left, second is right.
     */
    pair<CValue, CValue> evaluateValues(CCycleDetectionVisitor &visitor);

private:

    CASTNode *m_left_operand;
    CASTNode *m_right_operand;
};

/**
 * The node that performs addition operation on both operands.
 */
class AddNode : public BinaryOperationNode {
public:
    AddNode(CASTNode *left_operand, CASTNode *right_operand);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;
};

/**
 * The node that performs subtraction, where left operand is the minuend, right is subtrahend.
 */
class SubtractNode : public BinaryOperationNode {
public:
    SubtractNode(CASTNode *left_operand, CASTNode *right_operand);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;
};

/**
 * The node that performs multiplication on both operands.
 */
class MultiplicationNode : public BinaryOperationNode {
public:
    MultiplicationNode(CASTNode *left_operand, CASTNode *right_operand);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;
};

/**
 * The node that performs division, where left operand is the dividend, right operand is divisor.
 */
class DivisionNode : public BinaryOperationNode {
public:
    DivisionNode(CASTNode *left_operand, CASTNode *right_operand);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;
};


/**
 * The node that performs power operation, where left operand is a base, right operand is an exponent.
 */
class PowerNode : public BinaryOperationNode {
public:
    PowerNode(CASTNode *left_operand, CASTNode *right_operand);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;
};

#endif //PA2_BIG_TASK_BINARYOPERATIONNODE_H
