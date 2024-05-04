//
// Created by bardanik on 14/04/24.
//

#ifndef PA2_BIG_TASK_CAST_H
#define PA2_BIG_TASK_CAST_H


#include <variant>
#include <string>
#include "../../SpreadsheetStructure/CPos.h"
#include "../CycleDetectionVisitor/CCycleDetectionVisitor.h"

class CSpreadsheet;

using namespace std;
using namespace literals;
using CValue = variant<monostate, double, string>;

class CASTNode {
public:
    virtual CValue evaluate(CCycleDetectionVisitor &visitor) = 0;

    virtual ~CASTNode() = default;
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


#endif //PA2_BIG_TASK_CAST_H

