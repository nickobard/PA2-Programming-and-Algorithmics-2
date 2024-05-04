//
// Created by bardanik on 14/04/24.
//

#ifndef PA2_BIG_TASK_CAST_H
#define PA2_BIG_TASK_CAST_H


#include <variant>
#include <string>
#include "CPos.h"
#include "CCycleDetectionVisitor.h"

class CSpreadsheet;

using namespace std;
using namespace literals;
using CValue = variant<monostate, double, string>;

class CASTNode {
public:
    virtual CValue evaluate(CCycleDetectionVisitor &visitor) = 0;

    virtual ~CASTNode() = default;
};

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

class UnaryOperationNode : public CASTNode {
public:
    explicit UnaryOperationNode(CASTNode *operand);

    ~UnaryOperationNode();

    CValue evaluateValue(CCycleDetectionVisitor &visitor);

protected:

private:
    CASTNode *m_operand;
};

class CStringNode : public CASTNode {
public:
    CStringNode(const string &parsed_value);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;

private:
    CValue m_value;
};

class CReferenceNode : public CASTNode {
public:
    CReferenceNode(const string &pos, CSpreadsheet &spreadsheet, const pair<int, int> &shift);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;

private:
    CPos m_reference_position;
    CSpreadsheet &m_spreadsheet;
};

class CNumberNode : public CASTNode {
public:
    CNumberNode(double number);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;

private:
    CValue m_number;
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

class EqualNode : public BinaryOperationNode {
public:
    EqualNode(CASTNode *first_arg, CASTNode *second_arg);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;

};


class NotEqualNode : public BinaryOperationNode {
public:
    NotEqualNode(CASTNode *first_arg, CASTNode *second_arg);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;

};

class LessThanNode : public BinaryOperationNode {
public:
    LessThanNode(CASTNode *first_arg, CASTNode *second_arg);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;

};

class GreaterThanNode : public BinaryOperationNode {
public:
    GreaterThanNode(CASTNode *first_arg, CASTNode *second_arg);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;

};

class LessThanOrEqualNode : public BinaryOperationNode {
public:
    LessThanOrEqualNode(CASTNode *first_arg, CASTNode *second_arg);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;

};

class GreaterThanOrEqualNode : public BinaryOperationNode {
public:
    GreaterThanOrEqualNode(CASTNode *first_arg, CASTNode *second_arg);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;

};


class NegationNode : public UnaryOperationNode {
public:
    explicit NegationNode(CASTNode *arg);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;
};

#endif //PA2_BIG_TASK_CAST_H

