//
// Created by bardanik on 03/05/24.
//
#ifndef PA2_BIG_TASK_CCYCLEDETECTIONVISITOR_H
#define PA2_BIG_TASK_CCYCLEDETECTIONVISITOR_H

#include <unordered_set>
#include <exception>

using namespace std;

class CCell;

/**
 * Exception that is thrown when cycle detection visitor detects a cycle in AST tree evaluation.
 */
class CCycleDetectedException : public exception {
};

/**
 * Class that visits each cell while AST evaluation. The evaluation is performed in DFS manner,
 * the visitor is passed recursively to next node and every time it arrives at a new cell,
 * it remembers that it was opened, so when the visitor arrives at already opened cell, it means
 * that there is oriented cycle, and throws a cycle detection exception. When evaluation of the cell
 * is done, the visitor leaves the node and marks it as closed/fresh. The node can be again opened from other
 * reference or range node.
 */
class CCycleDetectionVisitor {
public:
    /**
     * Visits cell and marks it as opened.
     * @param cell - cell that is being visited.
     */
    void visit(const CCell *cell);

    /**
     * Leaves cell and marks it as closed/fresh.
     * @param cell - cell that is being left.
     */
    void leave(const CCell *cell);

private:
    // Stores cells that is currently visited and marked as opened.
    unordered_set<const CCell *> m_opened;
};

#endif //PA2_BIG_TASK_CCYCLEDETECTIONVISITOR_H