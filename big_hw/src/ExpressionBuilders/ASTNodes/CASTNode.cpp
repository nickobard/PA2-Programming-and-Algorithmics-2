//
// Created by bardanik on 14/04/24.
//

#include "../../CSpreadsheet.h"
#include "CASTNode.h"


CReferenceNode::CReferenceNode(const string &pos, CSpreadsheet &spreadsheet, const pair<int, int> &shift)
        : m_reference_position(CPos(pos)),
          m_spreadsheet(spreadsheet) {
    m_reference_position.shiftPos(shift);
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






