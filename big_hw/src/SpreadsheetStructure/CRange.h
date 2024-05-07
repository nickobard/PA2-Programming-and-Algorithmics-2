//
// Created by bardanik on 05/05/24.
//

#ifndef PA2_BIG_TASK_CRANGE_H
#define PA2_BIG_TASK_CRANGE_H

#include <vector>
#include "CCell.h"

// Container to store selection - array of position and defined cells.
using Range = vector<pair<pair<int, int>, shared_ptr<CCell>>>;

/**
 * Represents rectangular selection of cells in the spreadsheet cells container.
 * Used for selecting rectangular selection of cells, copying and pasting the selection.
 */
class CRange {
public:

    /**
     * Construct empty range, associated with some spreadsheet.
     * @param spreadsheet - represents a spreadsheet where cells should be selected.
     */
    explicit CRange(CSpreadsheet &spreadsheet);

    /**
     * Select cells.
     * @param src - upper left corner of the rectangular selection.
     * @param w - w >= 1, width of the rectangular.
     * @param h - h >= 1, height of the rectangular.
     */
    void select(const CPos &src, int w = 1, int h = 1);

    /**
     * Select cells.
     * @param from - upper left corner of the rectangular selection.
     * @param to - bottom right corner of the rectangular selection.
     */
    void select(const CPos &from, const CPos &to);

    /**
     * Paste current selection to some another point.
     * Pasting without using select() - with empty selection - is undefined behaviour.
     * @param dst represents upper left corner from which to paste current selection.
     */
    void paste(const CPos &dst);

    /**
     * Evaluates cells in the selection.
     * @param visitor - cycle detection object for evaluation.
     * @return vector of evaluated cells in the selection.
     */
    vector<CValue> evaluate(CCycleDetectionVisitor &visitor);

    /**
     * Parses range of cells.
     * i.e. A1:F10 will be parsed to A1 and F10 tokens.
     * @param range - represents range literal to be parsed.
     * @return left upper and right bottom position tokens.
     */
    static pair<string, string> splitRange(const string &range);

private:

    /**
     * Delete cells stored in the rectangle form from some position in the spreadsheet.
     * @param dst upper left corner of the rectangular.
     */
    void deleteCells(const CPos &dst);

    /**
     * Shifts current cells positions in selection.
     * Is used for relative references to be shifted and to update selection positions.
     * @param offset - offset by which the rectangular selection is moved from one position to another.
     */
    void shiftSelection(const pair<int, int> &offset);

    /**
     * Assigns current selection non-empty cells to their current positions in the selection.
     */
    void pasteCells();

    // Reference to a spreadsheet.
    CSpreadsheet &m_spreadsheet;
    // Current selection container. Stored cells and their current positions.
    Range m_selection;
    // Upper left corner position - pivot -  where the last selection was made.
    CPos m_selection_position;
    // Width and height of the selection.
    int m_w, m_h;
};


#endif //PA2_BIG_TASK_CRANGE_H
