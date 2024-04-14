//
// Created by bardanik on 14/04/24.
//

#ifndef PA2_BIG_TASK_CAST_H
#define PA2_BIG_TASK_CAST_H

#endif //PA2_BIG_TASK_CAST_H

#include <variant>
#include <string>
#include "CPos.h"

class CSpreadsheet;

using namespace std;
using namespace literals;
using CValue = variant<monostate, double, string>;

class CASTNode {
public:
    virtual CValue evaluate() const = 0;

    virtual ~CASTNode() = default;
};

class BinaryOperationNode : public CASTNode {
public:
    BinaryOperationNode(CASTNode *left_operand, CASTNode *right_operand);

    ~BinaryOperationNode();

protected:
    pair<double, double> getDoubleValues() const;

private:

    CASTNode *m_left_operand;
    CASTNode *m_right_operand;
};

class UnaryOperationNode : public CASTNode {
public:
    explicit UnaryOperationNode(CASTNode *operand);

    ~UnaryOperationNode();
protected:
    double getDoubleOperand() const;
private:
    CASTNode *m_operand;
};

class CStringNode : public CASTNode {
public:
    CStringNode(const string &parsed_value);

    CValue evaluate() const override;

private:
    CValue m_value;
};

class CReferenceNode : public CASTNode {
public:
    CReferenceNode(const string &pos, const CSpreadsheet &spreadsheet);

    CValue evaluate() const override;

private:
    CPos m_reference_position;
    const CSpreadsheet &m_spreadsheet;
};

class CNumberNode : CASTNode {
public:
    CNumberNode(double number);

    CValue evaluate() const override;

private:
    CValue m_number;
};

class AddNode : public BinaryOperationNode {
public:
    CValue evaluate() const override;
};

class SubtractNode : public BinaryOperationNode {
public:
    CValue evaluate() const override;
};

class MultiplicationNode : public BinaryOperationNode {
public:
    CValue evaluate() const override;
};

class DivisionNode : public BinaryOperationNode {
public:
    CValue evaluate() const override;
};

class PowerNode : public BinaryOperationNode {
public:
    CValue evaluate() const override;
};

class NegationNode : public UnaryOperationNode {
public:
    CValue evaluate() const override;
};