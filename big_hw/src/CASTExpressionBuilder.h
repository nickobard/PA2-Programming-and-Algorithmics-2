//
// Created by bardanik on 14/04/24.
//

#ifndef PA2_BIG_TASK_CASTEXPRESSIONBUILDER_H
#define PA2_BIG_TASK_CASTEXPRESSIONBUILDER_H

#endif //PA2_BIG_TASK_CASTEXPRESSIONBUILDER_H

#include "ExpressionBuilder/CExprBuilder.h"
#include "CASTNode.h"

class CCell;

class CASTExpressionBuilder : public CExprBuilder {
public:
    explicit CASTExpressionBuilder(CSpreadsheet &spreadsheet, const CCell* current_cell );

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

    CASTNode *getResult();

private:

    pair<CASTNode *, CASTNode *> getNodesPairAndPop();

    stack<CASTNode *> m_stack;
    CSpreadsheet &m_spreadsheet;
    const CCell *m_cell;
};