//
// Created by bardanik on 14/04/24.
//
#include "CASTExpressionBuilder.h"

CASTExpressionBuilder::CASTExpressionBuilder(const CSpreadsheet &spreadsheet) : m_root(nullptr),
                                                                                m_spreadsheet(spreadsheet) {

}

CASTExpressionBuilder::~CASTExpressionBuilder() {
    delete m_root;
}

CValue CASTExpressionBuilder::evaluate() const {
    return CValue();
}

