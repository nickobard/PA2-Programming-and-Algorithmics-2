//
// Created by bardanik on 11/04/24.
//

#ifndef BARDANIK_CSTACKEXPRESSIONBUILDER_H
#define BARDANIK_CSTACKEXPRESSIONBUILDER_H

#include <iostream>
#include <stack>
#include <variant>
#include <vector>

#include "../expression.h"

using namespace std;
using namespace literals;
using CValue = variant<monostate, double, string>;

class CSpreadsheet;

class CStackExpressionBuilder : public CExprBuilder {
public:
    explicit CStackExpressionBuilder(const CSpreadsheet &spreadsheet) : m_spreadsheet(spreadsheet) {}

    void opAdd() override;

    void opSub() override;

    void opMul() override;

    void opDiv() override;

    void opPow() override;

    void opNeg() override;

    void opEq() override;

    void opNe() override;

    void opLt() override;

    void opLe() override;

    void opGt() override;

    void opGe() override;

    void valNumber(double val) override;

    void valString(string val) override;

    void valReference(string val) override;

    void valRange(string val) override;

    void funcCall(std::string fnName,
                  int paramCount) override;

    CValue getResult() const;

private:

    stack<CValue> m_expression_result;
    const CSpreadsheet &m_spreadsheet;

};

#endif //BARDANIK_CSTACKEXPRESSIONBUILDER_H
