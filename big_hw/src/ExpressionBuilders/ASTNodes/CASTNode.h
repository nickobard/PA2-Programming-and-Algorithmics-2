//
// Created by bardanik on 14/04/24.
//

#ifndef PA2_BIG_TASK_CAST_H
#define PA2_BIG_TASK_CAST_H


#include <variant>
#include <vector>
#include "../../SpreadsheetStructure/CPos.h"
#include "../CycleDetectionVisitor/CCycleDetectionVisitor.h"

class CSpreadsheet;

class CRange;

using namespace literals;

// Value type that is stored in each cell - double, string or monostate (undefined).
using CValue = variant<monostate, double, string>;

/**
 * Represents an abstract node in the AST tree.
 */
class CASTNode {
public:
    /**
     * Evaluates the node and returns the evaluation value.
     * @param visitor - cycle detection visitor that is propagated to check for cycles.
     * @return evaluation of the node.
     */
    virtual CValue evaluate(CCycleDetectionVisitor &visitor) = 0;

    /**
     * Evaluates range of nodes.
     * Is used by CRangeNode to evaluate ranges. In case of other nodes
     * returns just a vector with a single value.
     * @param visitor - cycle detection visitor that is propagated to check for cycles.
     * @return evaluations of each cell in range. In case of a non range node, returns
     * a vector with a single element in it.
     */
    virtual vector<CValue> evaluateRange(CCycleDetectionVisitor &visitor);

    /**
     * Returns the size of the rectangular selection of the range,
     * considering empty cells too.
     * @return the size of the rectangualr selection.
     */
    virtual size_t rangeCapacity() const;

    virtual ~CASTNode() = default;
};

/**
 * Represents a node that stores a string literal value.
 */
class CStringNode : public CASTNode {
public:
    /**
     * Constructs a string node which stores parsed string literal value.
     * @param parsed_value - parsed string literal, given by the parser.
     */
    CStringNode(const string &parsed_value);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;

private:
    // Value of the node.
    CValue m_value;
};

/**
 * Represents a node that stores reference to another cell and also a spreadsheet,
 * where that cell is expected to be located. In evaluation finds that cell and gets value from the cell.
 */
class CReferenceNode : public CASTNode {
public:
    /**
     * Constructs a reference node.
     * @param pos - position of the referenced cell.
     * @param spreadsheet - spreadsheet where the referenced cell is expected.
     * @param offset - offset of the cell, that is used to construct reference node to point to
     * the correct place, considering the global shift of the cell. Look CCell documentation for details
     * of how it is implemented.
     */
    CReferenceNode(const string &pos, CSpreadsheet &spreadsheet, const pair<int, int> &offset);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;

private:
    // Reference cell position.
    CPos m_reference_position;
    // Spreadsheet where the cell is expected to be.
    CSpreadsheet &m_spreadsheet;
};

/**
 * Represents a node that stores a range of cells. The selection and cells aggregation
 * is computed only in evaluation stage. The node stores only two positions - from position and
 * to position, which together are used to make a rectangular selection - CRange object.
 */
class CRangeNode : public CASTNode {
public:
    /**
     * Constucts a range node.
     * @param from - upper left corner position of the range.
     * @param to - bottom right corner position of the range.
     * @param spreadsheet - spreadsheet where selection is made.
     * @param offset - offset to consider shift if some cells were copied and shifted.
     * Look at CCell documentation for details how it works.
     */
    CRangeNode(const string &from, const string &to, CSpreadsheet &spreadsheet, const pair<int, int> &offset);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;

    vector<CValue> evaluateRange(CCycleDetectionVisitor &visitor) override;

    size_t rangeCapacity() const override;

private:
    // Upper left corner position of the range.
    CPos m_from_position;
    // Bottom right corner position of the range.
    CPos m_to_position;
    // Spreadsheet where the selection of range will be made in evaluation.
    CSpreadsheet &m_spreadsheet;
};

/**
 * Represents a Number node.
 */
class CNumberNode : public CASTNode {
public:
    CNumberNode(double number);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;

private:
    CValue m_number;
};


#endif //PA2_BIG_TASK_CAST_H

