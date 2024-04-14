//
// Created by bardanik on 14/04/24.
//

#ifndef PA2_BIG_TASK_CASTEXPRESSIONBUILDER_H
#define PA2_BIG_TASK_CASTEXPRESSIONBUILDER_H

#endif //PA2_BIG_TASK_CASTEXPRESSIONBUILDER_H

#include "ExpressionParser.h"
#include "CExprBuilder.h"

class CASTNode;
class CSpreadsheet;

class CASTExpressionBuilder : public CExprBuilder {
public:
    explicit CASTExpressionBuilder(CSpreadsheet &spreadsheet, CASTNode *&root);

    ~CASTExpressionBuilder();

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

    CValue evaluate() override;

private:
    CASTNode *&m_root;
    CSpreadsheet &m_spreadsheet;
};