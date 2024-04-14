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
    virtual CValue evaluate() = 0;

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

    CValue evaluate() override;

private:
    CValue m_value;
};

class CReferenceNode : public CASTNode {
public:
    CReferenceNode(const string &pos, CSpreadsheet &spreadsheet);

    CValue evaluate() override;

private:
    CPos m_reference_position;
    CSpreadsheet &m_spreadsheet;
};

class CNumberNode : CASTNode {
public:
    CNumberNode(double number);

    CValue evaluate() override;

private:
    CValue m_number;
};

class AddNode : public BinaryOperationNode {
public:
    CValue evaluate() override;
};

class SubtractNode : public BinaryOperationNode {
public:
    CValue evaluate() override;
};

class MultiplicationNode : public BinaryOperationNode {
public:
    CValue evaluate() override;
};

class DivisionNode : public BinaryOperationNode {
public:
    CValue evaluate() override;
};

class PowerNode : public BinaryOperationNode {
public:
    CValue evaluate() override;
};

class NegationNode : public UnaryOperationNode {
public:
    CValue evaluate() override;
};