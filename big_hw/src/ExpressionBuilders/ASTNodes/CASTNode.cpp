//
// Created by bardanik on 14/04/24.
//

#include "../../CSpreadsheet.h"
#include "CASTNode.h"


CReferenceNode::CReferenceNode(const string &pos, CSpreadsheet &spreadsheet, const pair<int, int> &offset)
        : m_reference_position(CPos(pos)),
          m_spreadsheet(spreadsheet) {
    m_reference_position.shift(offset);
}

CValue CReferenceNode::evaluate(CCycleDetectionVisitor &visitor) {
    auto value = m_spreadsheet.getValue(m_reference_position, visitor);
    return value;
}

CStringNode::CStringNode(const string &parsed_value) : m_value({parsed_value}) {

}

CValue CStringNode::evaluate(CCycleDetectionVisitor &visitor) {
    return m_value;
}


CNumberNode::CNumberNode(double number) : m_number({number}) {

}

CValue CNumberNode::evaluate(CCycleDetectionVisitor &visitor) {
    return m_number;
}


CRangeNode::CRangeNode(const string &from,
                       const string &to,
                       CSpreadsheet &spreadsheet,
                       const pair<int, int> &offset) : m_from_position(from), m_to_position(to),
                                                       m_spreadsheet(spreadsheet) {
    m_from_position.shift(offset);
    m_to_position.shift(offset);

}

CValue CRangeNode::evaluate(CCycleDetectionVisitor &visitor) {
    return {};
}

vector<CValue> CRangeNode::evaluateRange(CCycleDetectionVisitor &visitor) {
    CRange range(m_spreadsheet);
    range.select(m_from_position, m_to_position);
    return range.evaluate(visitor);
}

vector<CValue> CASTNode::evaluateRange(CCycleDetectionVisitor &visitor) {
    return {evaluate(visitor)};
}
