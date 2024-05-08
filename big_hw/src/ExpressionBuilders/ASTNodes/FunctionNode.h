//
// Created by bardanik on 06/05/24.
//

#ifndef PA2_BIG_TASK_FUNCTIONNODE_H
#define PA2_BIG_TASK_FUNCTIONNODE_H

#include "CASTNode.h"

/**
 * Represents an abstract function node class to evaluate some function on given arguments.
 * Concrete functions are defined in derived classes.
 */
class FunctionNode : public CASTNode {
public:
    /**
     * Constructs function node with parameter pack.
     * @tparam Args parameters type, CASTNodes types are expected as arguments.
     * @param args arguments of the function
     */
    template<typename... Args>
    explicit FunctionNode(Args... args);

    ~FunctionNode();

protected:
    // Stores arguments passed to this function node.
    vector<CASTNode *> m_args;
};

/**
 * Represents sum operation on given range of cells. Sums numeric values across each cell in range.
 */
class SumNode : public FunctionNode {
public:
    /**
     * Constructs sum node with a given range node.
     * @param m_range - node which evaluates range of cells.
     */
    explicit SumNode(CASTNode *m_range);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;
};

/**
 * Represents count operation. Counts all numeric values in the given range.
 */
class CountNode : public FunctionNode {
public:
    /**
     * Constructs count node with a given range node.
     * @param m_range - node which evaluates range of cells.
     */
    explicit CountNode(CASTNode *m_range);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;

};

/**
 * Represents min operation. Find the minimal numeric value in given range of values,
 * otherwise gives undefined value.
 */
class MinNode : public FunctionNode {
public:
    /**
     * Constructs min node with a given range node.
     * @param m_range - node which evaluates range of cells.
     */
    explicit MinNode(CASTNode *m_range);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;
};

/**
 * Represents max operation. Find the minimal numeric value in given range of values,
 * otherwise gives undefined value.
 */
class MaxNode : public FunctionNode {
public:
    /**
     * Constructs max node with a given range node.
     * @param m_range - node which evaluates range of cells.
     */
    explicit MaxNode(CASTNode *m_range);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;
};

/**
 * Node that represents value count function, which counts number of values in a given range of cells
 * that equal to a give reference value. The reference value can be double, string or undefined.
 */
class CountValNode : public FunctionNode {
public:
    /**
     * Constructs the count value node.
     * @param value - reference value to which compare each value in a range of other values.
     * @param range - node that evaluates range of selected values.
     */
    explicit CountValNode(CASTNode *value, CASTNode *range);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;

private:
    /**
     * Performs counting based on given type, for example double, string or monostate (undefined).
     * @tparam T - type of the reference value.
     * @param range - vector of CValues (result of range node evaluation).
     * @param value - represents a value to which to compare each value in a range to count it in.
     * @return count of values from range that match the specified value argument.
     */
    template<typename T>
    double countValRange(const vector<CValue> &range, const T &value) const;

};

/**
 * Node that represents control flow function - the if statement expression. If condition
 * argument is evaluated as non zero - returns evaluation of true argument node, otherwise returns
 * evaluation of false argument node.
 */
class ConditionalNode : public FunctionNode {
public:
    /**
     * Constructs the conditional node.
     * @param cond - condition to be assessed.
     * @param if_true - node to be evaluated if condition is evaluated as a non zero value.
     * @param if_false - node to be evaluated if condition is evaluated as a zero value.
     */
    explicit ConditionalNode(CASTNode *cond, CASTNode *if_true, CASTNode *if_false);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;
};


#endif //PA2_BIG_TASK_FUNCTIONNODE_H
