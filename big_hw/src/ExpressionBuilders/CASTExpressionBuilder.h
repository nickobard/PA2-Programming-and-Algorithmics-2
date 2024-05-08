//
// Created by bardanik on 14/04/24.
//

#ifndef PA2_BIG_TASK_CASTEXPRESSIONBUILDER_H
#define PA2_BIG_TASK_CASTEXPRESSIONBUILDER_H

#include <stack>
#include "CExprBuilder.h"
#include "ASTNodes/CASTNode.h"
#include "ASTNodes/BinaryOperationNode.h"
#include "ASTNodes/UnaryOperationNode.h"
#include "ASTNodes/RelationalOperationNode.h"
#include "ASTNodes/FunctionNode.h"

class CCell;

/**
 * Expression builder which constructs Abstract Syntactic Tree in expression parsing process.
 */
class CASTExpressionBuilder : public CExprBuilder {
public:
    /**
     * Constructs AST expression builder.
     * @param spreadsheet - reference to a spreadsheet.
     * @param current_cell - cell with expression that is being parsed.
     */
    explicit CASTExpressionBuilder(CSpreadsheet &spreadsheet, const CCell *current_cell);

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

    /**
     * Returns the root of the AST tree as a result of the parsing process.
     * @return root of the AST tree.
     */
    CASTNode *getResult();

private:

    /**
     * Gets and removes top two AST nodes from the stack.
     * @return a pair of AST nodes stored on top of the stack.
     */
    pair<CASTNode *, CASTNode *> getNodesPairAndPop();

    /**
     * Get N number of AST nodes and removes them from top of the stack.
     * @tparam NArgs - number of nodes to extract.
     * @return vector with NArgs nodes.
     */
    template<size_t NArgs>
    vector<CASTNode *> getNodesAndPop();

    // Stack for storing intermediate nodes.
    stack<CASTNode *> m_stack;
    // Spreadsheet where the parsed cell is located.
    CSpreadsheet &m_spreadsheet;
    // Cell that is being under the parse process.
    const CCell *m_cell;
};

#endif //PA2_BIG_TASK_CASTEXPRESSIONBUILDER_H
